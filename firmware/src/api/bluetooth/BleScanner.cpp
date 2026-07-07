#include "BleScanner.h"
#include "api/EventsApi.h"
#include "core/LedManager.h"

NimBLEScan* BleScanner::_scan = nullptr;
QueueHandle_t BleScanner::_deviceQueue = nullptr;
TaskHandle_t BleScanner::_processingTaskHandle = nullptr;
BleScanner::ScanCallbacks BleScanner::_scanCallbacks;
std::unordered_map<std::string, std::string> BleScanner::_nameCache;

void BleScanner::start() {
     if (_scan != nullptr) {
        return;
    }

    _scan = NimBLEDevice::getScan();
    _scan->setScanCallbacks(&_scanCallbacks, true); 
    _scan->setActiveScan(true);            
    _scan->setInterval(150);                    
    _scan->setWindow(90);                       
    _scan->setMaxResults(0);

    _scan->setDuplicateFilter(false);

    _nameCache.clear();
    _deviceQueue = xQueueCreate(50, sizeof(BleRawEvent));
    
    xTaskCreatePinnedToCore(BleScanner::bleProcessingTask, "BLE_Scan_Task", 4096, nullptr, 1, &_processingTaskHandle, 0);

    _scan->clearResults();
    _scan->start(0, false, false);

    LedManager::setStatus(LedStatus::Info);
}

void BleScanner::stop() {
    release();
    LedManager::setStatus(LedStatus::Successful);
}

void BleScanner::release() {
    if (_scan) { 
        _scan->stop();
        
        vTaskDelay(pdMS_TO_TICKS(50));
        _scan->clearResults(); 
        _scan = nullptr; 
    }

    if (_processingTaskHandle != nullptr) {
        vTaskDelete(_processingTaskHandle); 
        _processingTaskHandle = nullptr; 
    }
    
    if (_deviceQueue != nullptr) { 
        vQueueDelete(_deviceQueue); 
        _deviceQueue = nullptr; 
    }

    _nameCache.clear();
}

void BleScanner::ScanCallbacks::onResult(const NimBLEAdvertisedDevice* device) {
    if (!_deviceQueue) return;

    std::string addr = device->getAddress().toString();
    std::string nameStr = device->getName();

    if (!nameStr.empty()) {
        _nameCache[addr] = nameStr;
    } else {
        auto it = _nameCache.find(addr);
        if (it != _nameCache.end()) {
            nameStr = it->second;
        }
    }

    BleRawEvent rawEvt = {};

    strncpy(rawEvt.address, device->getAddress().toString().c_str(), sizeof(rawEvt.address) - 1);
    rawEvt.rssi = device->getRSSI();

    strncpy(rawEvt.name, !nameStr.empty() ? nameStr.c_str() : "Unknown", sizeof(rawEvt.name) - 1);
    
    uint8_t advType = device->getAdvType();
    switch (advType) {
        case BLE_HCI_ADV_TYPE_ADV_IND:
        case BLE_HCI_ADV_TYPE_ADV_SCAN_IND:
            rawEvt.status = BleDeviceStatus::Connectable;
            break;
        case BLE_HCI_ADV_TYPE_ADV_NONCONN_IND:
            rawEvt.status = BleDeviceStatus::NonConnectable;
            break;
        case BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD:
        case BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD:
            rawEvt.status = BleDeviceStatus::Directed;
            break;
        default:
            rawEvt.status = BleDeviceStatus::Unknown;
            break;
    }

    xQueueSendToBack(_deviceQueue, &rawEvt, 0);
}

void BleScanner::ScanCallbacks::onScanEnd(const NimBLEScanResults& results, int reason) {
    if (_scan) { _scan->clearResults(); }
}

void BleScanner::bleProcessingTask(void* pvParameters) {
    BleRawEvent dev;
    while (true) {
        if (xQueueReceive(_deviceQueue, &dev, portMAX_DELAY) == pdTRUE) {
            EventsApi::sendDeviceFound(dev);
        }
    }
}