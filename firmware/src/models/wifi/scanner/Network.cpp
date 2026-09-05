#include "Network.h"
#include "WiFi.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "api/event/EspEvent.h"
#include "api/event/EventsApi.h"
#include "core/LedManager.h"
#include "core/Logger.h"
#include "utils/utils.h"
#include "models/wifi/select/WifiSelect.h"
#include "models/wifi/utils/utils.h"
#include "models/wifi/types/frame.h"
#include "models/wifi/types/info.h"

static const char* TAG = "Wifi-Scanner";

TaskHandle_t WifiScanner::_task = nullptr;

void WifiScanner::start() {
	wifi::turnModeAPSTA();

	esp_wifi_set_promiscuous(true);
	wifi::wifiFilterFrameTypes(true, true, false);
	esp_wifi_set_promiscuous_rx_cb(&onPacketReceived);

	esp_event_handler_register(
        ESP_FRAME_EVENT,
        EVENT_SEND_NETWORK,
        &sendNetworkHandler,
        nullptr
    );
	esp_event_handler_register(
        ESP_FRAME_EVENT,
        EVENT_SEND_CLIENT_INFO,
        &sendClientHandler,
        nullptr
    );

	xTaskCreate(scanTask, "Wifi_Scanner_Task", 6144, nullptr, 1, &_task);

	LedManager::setStatus(LedStatus::Info);
	LOG_I(TAG, "WiFi scanner started");
}

void WifiScanner::stop() {
    esp_wifi_scan_stop();

	esp_wifi_set_promiscuous(false);
	esp_wifi_set_promiscuous_rx_cb(nullptr);

	vTaskDelete(_task);
	_task = nullptr;

	WiFi.scanDelete();

	esp_event_handler_unregister(ESP_FRAME_EVENT, EVENT_SEND_NETWORK, &sendNetworkHandler);
	esp_event_handler_unregister(ESP_FRAME_EVENT, EVENT_SEND_CLIENT_INFO, &sendClientHandler);

    wifi::disableModeAPSTA();

	LedManager::setStatus(LedStatus::Successful);
	LOG_I(TAG, "WiFi scanner stopped");
}

void WifiScanner::sendNetworkHandler(void *args, esp_event_base_t eventBase, int32_t eventId, void *eventData) {
	const WifiNetworkInfo* network = static_cast<const WifiNetworkInfo*>(eventData);
	EventsApi::sendNetworkFound(*network);
}

void WifiScanner::sendClientHandler(void *args, esp_event_base_t eventBase, int32_t eventId, void *eventData) {
	const WifiClientInfo* client = static_cast<const WifiClientInfo*>(eventData);
	EventsApi::sendClientFound(*client);
}

void WifiScanner::scanTask(void* parameter) {
    LOG_I(TAG, "Scanner task started, stack watermark: %u", uxTaskGetStackHighWaterMark(nullptr));

    while (true) {
		WiFi.scanDelete();

        const int count = WiFi.scanNetworks(false, true, false, 120, 0);

        if (count < 0) {
            LOG_E(TAG, "WiFi scan failed: %d", count);
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        LOG_I(TAG, "Networks found: %d", count);

		for (int index = 0; index < count; index++) {
			WifiNetworkInfo info{};

			const uint8_t* bssid = WiFi.BSSID(index);
			if (bssid) memcpy(info.bssid, bssid, 6);

			strncpy(info.ssid, WiFi.SSID(index).c_str(), sizeof(info.ssid) - 1);
			info.channel = WiFi.channel(index);
			info.rssi = WiFi.RSSI(index);

			const char* label = wifi::encryptionTypeToLabel(WiFi.encryptionType(index));
			strncpy(info.encryptionLabel, label, sizeof(info.encryptionLabel) - 1);

			esp_event_post(
				ESP_FRAME_EVENT,
				EVENT_SEND_NETWORK,
				&info,
				sizeof(info),
				portMAX_DELAY
			);
			vTaskDelay(pdMS_TO_TICKS(10));
		}
        WiFi.scanDelete();
		vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void WifiScanner::onPacketReceived(void* buf, wifi_promiscuous_pkt_type_t type) {
	if(!WifiSelect::hasNetwork()){
		return;
	}

	wifi_promiscuous_pkt_t* frame = (wifi_promiscuous_pkt_t*) buf;
	HeaderFrame* header = (HeaderFrame*) frame->payload;

	const uint8_t* clientMac = nullptr;
	const bool toDs = header->frameControl.toDs;
    const bool fromDs = header->frameControl.fromDs;

	const WifiNetworkInfo* selectNetwork = WifiSelect::getNetwork();

	if (toDs && !fromDs) {
        if (memcmp(header->addr1, selectNetwork->bssid, 6) != 0) return;
        clientMac = header->addr2;
    } else if (!toDs && fromDs) {
        if (memcmp(header->addr2, selectNetwork->bssid, 6) != 0) return;
        clientMac = header->addr1;
    }

	if (!clientMac) return;
	if (memcmp(clientMac, BROADCAST_ADDRESS, 6) == 0) return;

	WifiClientInfo info{};
	memcpy(info.bssid, selectNetwork->bssid, 6);
	memcpy(info.mac, clientMac, 6);
	info.rssi = frame->rx_ctrl.rssi;

	esp_event_post(
        ESP_FRAME_EVENT,
        EVENT_SEND_CLIENT_INFO,
        &info,
        sizeof(info),
        portMAX_DELAY
    );
}

