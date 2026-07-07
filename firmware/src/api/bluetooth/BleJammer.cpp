#include "BleJammer.h"
#include "core/LedManager.h"

int BleJammer::_powerLevel = 4;
TaskHandle_t BleJammer::_jammerTaskHandle = nullptr;

void BleJammer::start() {
    xTaskCreatePinnedToCore(BleJammer::jammerTask, "BLE_Jam_Task", 4096, nullptr, 5, &_jammerTaskHandle, 1);
    LedManager::setStatus(LedStatus::Info);
}

void BleJammer::stop() {
    release();
    LedManager::setStatus(LedStatus::Successful);
}

void BleJammer::release() {
    if (_jammerTaskHandle != nullptr) { 
        vTaskDelete(_jammerTaskHandle); 
        _jammerTaskHandle = nullptr; 
    }
    NimBLEDevice::getAdvertising()->stop();
}

void BleJammer::setPower(int powerLevel) {
    _powerLevel = constrain(powerLevel, 0, 9);
    if (_jammerTaskHandle) {
        NimBLEDevice::setPower((esp_power_level_t)_powerLevel);
    }
}

int BleJammer::getPower() { return _powerLevel; }

void BleJammer::jammerTask(void* pvParameters) {
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    NimBLEDevice::setPower((esp_power_level_t)_powerLevel);
    
    while (true) {
        uint8_t dummyData[31];
        for(int i = 0; i < 31; i++) {
            dummyData[i] = rand() % 256;
        }

        NimBLEAdvertisementData advData;
        advData.addData(dummyData, 31);
        
        pAdvertising->setAdvertisementData(advData);
        pAdvertising->start();
        
        vTaskDelay(pdMS_TO_TICKS(10)); 
        pAdvertising->stop();
    }
}