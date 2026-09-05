#pragma once
#include "Arduino.h"

class WifiAttackPmkid {
public:
	static void start(const char* ssid);
	static void stop();

private:
	static void onPacketReceived(void* buf, wifi_promiscuous_pkt_type_t type);
	static void pmkidExitConditionHandler(void *args, esp_event_base_t eventBase, int32_t eventId, void *eventData);
};