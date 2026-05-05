#include <Arduino.h>

#include "core/WiFiManager.h"
#include "core/ServerManager.h"
#include "core/Config.h"

#include "api/HttpApi.h"
#include "api/EventsApi.h"

unsigned long lastPing = 0;

void setup() {
  Serial.begin(115200);

  WiFiManager::initAP();

  ServerManager::init();
  HttpApi::registerRoutes();
  EventsApi::begin();
  ServerManager::begin();

  Serial.println("Server started at 192.168.1.1");
}

void loop() {
  if (millis() - lastPing > Config::SSE_HEARTBEAT_MS) {
    EventsApi::pushMessage("{\"ping\":true}");
    lastPing = millis();
  }
}