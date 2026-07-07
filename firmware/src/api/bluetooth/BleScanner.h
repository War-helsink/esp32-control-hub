#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <unordered_map>

enum class BleDeviceStatus : uint8_t {
    Unknown = 0,
    Connectable = 1, 
    NonConnectable = 2, 
    Directed = 3      
};

struct BleRawEvent {
    char address[18];
    int rssi;
    char name[65];
    BleDeviceStatus status;
};

class BleScanner {
public:
    static void start();
    static void stop();
    static void release();

private:
    static NimBLEScan* _scan;
    static QueueHandle_t _deviceQueue;
    static TaskHandle_t _processingTaskHandle;
    static std::unordered_map<std::string, std::string> _nameCache;

    class ScanCallbacks : public NimBLEScanCallbacks {
        void onResult(const NimBLEAdvertisedDevice* device) override;
        void onScanEnd(const NimBLEScanResults& results, int reason) override;
    };
    static ScanCallbacks _scanCallbacks;
    static void bleProcessingTask(void* pvParameters);
};