#pragma once
#include "Arduino.h"

struct WifiClientInfo {
    uint8_t bssid[6];
    uint8_t mac[6];    
    int8_t rssi;
};

struct WifiNetworkInfo {
    uint8_t bssid[6];
    char ssid[33];
    uint8_t channel;
    int8_t rssi;
    char encryptionLabel[16];
};