#pragma once
#include "../core/ServerManager.h"

class HttpApi {
public:
  static void registerRoutes() {
    ServerManager::server.on("/api/device", HTTP_GET,
      [](AsyncWebServerRequest *request) {
        request->send(
          200,
          "application/json",
          "{\"device\":\"ESP32-S3\",\"status\":\"running\"}"
        );
      }
    );

  }
};