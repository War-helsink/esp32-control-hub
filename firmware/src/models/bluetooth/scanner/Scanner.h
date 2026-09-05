#pragma once
#include "Arduino.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_event.h"

class BleScanner {
public:
    static void start();
    static void stop();

private:
    static void gapCallback(
        esp_gap_ble_cb_event_t event,
        esp_ble_gap_cb_param_t* param
    );

    static void sendDeviceEvent(
        const esp_ble_gap_cb_param_t::ble_scan_result_evt_param& result
    );

    static void deviceFoundHandler(
        void* handlerArgs,
        esp_event_base_t base,
        int32_t eventId,
        void* eventData
    );

    static void parseName(
        const uint8_t* data,
        uint8_t length,
        char* output,
        size_t outputSize
    );

    static void formatAddress(
        const esp_bd_addr_t address,
        char* output,
        size_t outputSize
    );
};