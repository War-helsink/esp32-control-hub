#include "WiFi.h"
#include "Config.h"
#include "WiFiManager.h"

void WiFiManager::initAP() {
  WiFi.mode(WIFI_MODE_AP);

  WiFi.softAPConfig(Config::LOCAL_IP, Config::GATEWAY, Config::SUBNET);
  WiFi.softAP(Config::AP_SSID, Config::AP_PASS, 1, 0, 1);
}