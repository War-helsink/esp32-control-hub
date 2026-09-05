#pragma once

#include "Arduino.h"

class WifiScanner {
public:
    static void start();
    static void stop();

private:
    static TaskHandle_t _task;

    static void scanTask(void* parameter);

    static void onPacketReceived(void* buf, wifi_promiscuous_pkt_type_t type);
    static void sendNetworkHandler(void *args, esp_event_base_t eventBase, int32_t eventId, void *eventData);
    static void sendClientHandler(void *args, esp_event_base_t eventBase, int32_t eventId, void *eventData);
};