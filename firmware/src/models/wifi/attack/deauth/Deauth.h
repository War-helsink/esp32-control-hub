#pragma once
#include "Arduino.h"
#include "models/wifi/types/frame.h"

class WifiAttackDeauth {
public:
	static void start();
	static void stop();

	static void startTimer(unsigned periodSec);
	static void stopTimer();

	
private:
	static esp_timer_handle_t _timerHandle;

	static void timerSendDeauthFrame(void *arg);
	static esp_err_t sendDeauthAllFrame(const uint8_t* bssid);
	static esp_err_t sendDeauthClientFrame(const uint8_t* bssid, const uint8_t* target);
};