#pragma once
#include "Arduino.h"
#include "models/wifi/types/frame.h"

class WifiAttackHandshake {
public:
	static void start(const char* ssid, const HandshakeType type);
	static void stop();

private:
	static HandshakeType _type;

	static void attackMethodStart();
	static void attackMethodStop();
	static void onPacketReceived(void* buf, wifi_promiscuous_pkt_type_t type);
	static void eapolKeyFrameHandler(void *args, esp_event_base_t eventBase, int32_t eventId, void *eventData);
};