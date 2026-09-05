#include "Scanner.h"
#include "api/event/EspEvent.h"
#include "api/event/EventsApi.h"
#include "core/Logger.h"
#include "core/LedManager.h"
#include "models/bluetooth/types/info.h"

static const char* TAG = "Bluetooth-Scanner";
 
static esp_ble_scan_params_t scanParams = {
    .scan_type = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,

    // 160 * 0.625 ms = 100 ms
    .scan_interval = 160,

    // 64 * 0.625 ms = 40 ms
    .scan_window = 64,

    .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE
};

void BleScanner::start() {
    esp_bt_controller_config_t config =
        BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    esp_bt_controller_init(&config);

    esp_bt_controller_enable(ESP_BT_MODE_BLE);

    esp_bluedroid_init();
    esp_bluedroid_enable();

    esp_ble_gap_register_callback(&gapCallback);
    esp_ble_gap_set_scan_params(&scanParams);

    esp_event_handler_register(
        ESP_FRAME_EVENT,
        BLE_SCANNER_DEVICE_FOUND,
        &deviceFoundHandler,
        nullptr
    );

    LedManager::setStatus(LedStatus::Info);
    LOG_I(TAG, "Bluetooth scanner initialized");
}

void BleScanner::stop() {
    esp_ble_gap_stop_scanning();

    esp_event_handler_unregister(
        ESP_FRAME_EVENT,
        BLE_SCANNER_DEVICE_FOUND,
        &deviceFoundHandler
    );

    if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_ENABLED) {
        esp_bluedroid_disable();
    }

    if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_INITIALIZED) {
        esp_bluedroid_deinit();
    }

    if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_ENABLED) {
        esp_bt_controller_disable();
    }

    if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_INITED) {
        esp_bt_controller_deinit();
    }

    LedManager::setStatus(LedStatus::Successful);
    LOG_I(TAG, "Bluetooth scanner stopped");
}


void BleScanner::gapCallback(
    esp_gap_ble_cb_event_t event,
    esp_ble_gap_cb_param_t* param
) {
    switch (event) {

        case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT: {
            esp_err_t err = esp_ble_gap_start_scanning(0);

            if (err != ESP_OK) {
                LOG_E(
                    TAG,
                    "Start scanning failed: %s",
                    esp_err_to_name(err)
                );
            }

            break;
        }

        case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT: {
            if (
                param->scan_start_cmpl.status ==
                ESP_BT_STATUS_SUCCESS
            ) {
                LOG_I(TAG, "Bluetooth scan started");
            } else {
                LOG_E(TAG, "Bluetooth scan start failed");
            }

            break;
        }

        case ESP_GAP_BLE_SCAN_RESULT_EVT: {
            if (
                param->scan_rst.search_evt !=
                ESP_GAP_SEARCH_INQ_RES_EVT
            ) {
                return;
            }

            sendDeviceEvent(
                param->scan_rst
            );

            break;
        }

        case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT: {
            LOG_I(TAG, "Bluetooth scan stopped");
            break;
        }

        default:
            break;
    }
}

void BleScanner::sendDeviceEvent(
    const esp_ble_gap_cb_param_t::ble_scan_result_evt_param& result
) {
    BleRawEvent device = {};

    formatAddress(
        result.bda,
        device.address,
        sizeof(device.address)
    );

    device.rssi = result.rssi;

    parseName(
        result.ble_adv,
        result.adv_data_len + result.scan_rsp_len,
        device.name,
        sizeof(device.name)
    );

    switch (result.ble_evt_type) {
        case ESP_BLE_EVT_CONN_ADV:
            device.status =
                BleDeviceStatus::Connectable;
            break;
        case ESP_BLE_EVT_NON_CONN_ADV:
            device.status =
                BleDeviceStatus::NonConnectable;
            break;
        case ESP_BLE_EVT_DISC_ADV:
            device.status =
                BleDeviceStatus::Directed;
            break;
        default:
            device.status =
                BleDeviceStatus::Unknown;
            break;
    }

    esp_event_post(
        ESP_FRAME_EVENT,
        BLE_SCANNER_DEVICE_FOUND,
        &device,
        sizeof(device),
        0
    );
}

void BleScanner::deviceFoundHandler(
    void* handlerArgs,
    esp_event_base_t base,
    int32_t eventId,
    void* eventData
) {
    if (eventData == nullptr) {
        return;
    }

    BleRawEvent* device =static_cast<BleRawEvent*>(eventData);

    EventsApi::sendDeviceFound(*device);
}

void BleScanner::formatAddress(
    const esp_bd_addr_t address,
    char* output,
    size_t outputSize
) {
    snprintf(
        output,
        outputSize,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        address[0],
        address[1],
        address[2],
        address[3],
        address[4],
        address[5]
    );
}

void BleScanner::parseName(
    const uint8_t* data,
    uint8_t length,
    char* output,
    size_t outputSize
) {
    strncpy(
        output,
        "Unknown",
        outputSize - 1
    );

    output[outputSize - 1] = '\0';

    size_t offset = 0;

    while (offset < length) {
        uint8_t fieldLength =data[offset];

        if (fieldLength == 0) {
            break;
        }

        if (offset + fieldLength >= length) {
            break;
        }

        uint8_t type = data[offset + 1];

        if (type == ESP_BLE_AD_TYPE_NAME_CMPL || type == ESP_BLE_AD_TYPE_NAME_SHORT) {
            size_t nameLength =
                fieldLength - 1;

            if (nameLength >= outputSize) {
                nameLength = outputSize - 1;
            }

            memcpy(
                output,
                &data[offset + 2],
                nameLength
            );

            output[nameLength] = '\0';

            return;
        }

        offset += fieldLength + 1;
    }
}