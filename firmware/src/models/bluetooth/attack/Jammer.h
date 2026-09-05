#pragma once
#include "Arduino.h"
#include "NimBLEDevice.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class BleJammer {
public:
    static void start();
    static void stop();
    static void setPower(int powerLevel);
    static int getPower();

private:
    static int _powerLevel;
    static TaskHandle_t _jammerTaskHandle;
    static void jammerTask(void* pvParameters);
};