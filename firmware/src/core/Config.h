#pragma once
#include <Arduino.h>
#include <IPAddress.h>

namespace Config {

  // ---------- WiFi AP ----------
  static const char* AP_SSID = "ESP32-Control";
  static const char* AP_PASS = "12345678";

  static const IPAddress LOCAL_IP(192, 168, 1, 1);
  static const IPAddress GATEWAY (192, 168, 1, 1);
  static const IPAddress SUBNET  (255, 255, 255, 0);

  // ---------- SSE ----------
  static const uint32_t SSE_HEARTBEAT_MS = 2000;

}