#pragma once
#include "Arduino.h"

struct BeaconNetwork {
    String ssid;
    uint8_t bssid[6];
    uint8_t channel;
};

struct BeaconBuffer {
    uint8_t data[256];
    size_t size; 
};

class  WifiAttackBeacon {
public:
    static void start(uint8_t channel);
    static void stop();

private:
    static BeaconBuffer* _frame;
	static esp_timer_handle_t _timerHandle;

    static void generateFrames(uint8_t channel);
    static String generateRandomSSID();
    static void generateRandomMac(uint8_t* mac);
    static void generateBeaconFrame(BeaconNetwork* network, BeaconBuffer* buffer);
  
    static void sendBeacons(void *arg);
};