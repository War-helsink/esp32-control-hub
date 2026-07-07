#pragma once
#include "core/ServerManager.h"
#include "api/bluetooth/BleManager.h"

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

    // ================= SCANNER API =================
    ServerManager::server.on("/api/ble/status", HTTP_GET, [](AsyncWebServerRequest *request) {
      bool isScanning = (BleManager::getMode() == BleMode::Scanning);
      String status = isScanning ? "{\"scanning\":true}" : "{\"scanning\":false}";
      request->send(200, "application/json", status);
    });

    ServerManager::server.on("/api/ble/scanner/start", HTTP_POST, [](AsyncWebServerRequest *request) {
      BleManager::startScan();
      request->send(200, "application/json", "{\"success\":true,\"status\":\"scanning\"}");
    });

    ServerManager::server.on("/api/ble/scanner/stop", HTTP_POST, [](AsyncWebServerRequest *request) {
      BleManager::stopScan();
      request->send(200, "application/json", "{\"success\":true,\"status\":\"idle\"}");
    });

    // ================= JAMMER API =================
    ServerManager::server.on("/api/ble/jammer/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        bool isActive = (BleManager::getMode() == BleMode::Jamming);
        String json = "{\"active\":" + String(isActive ? "true" : "false") + 
                      ",\"power\":" + String(BleManager::getJammerPower()) + "}";
        request->send(200, "application/json", json);
    });

    ServerManager::server.on("/api/ble/jammer/start", HTTP_POST, [](AsyncWebServerRequest *request) {
        BleManager::startJammer();
        request->send(200, "application/json", "{\"success\":true,\"status\":\"jamming\"}");
    });

    ServerManager::server.on("/api/ble/jammer/stop", HTTP_POST, [](AsyncWebServerRequest *request) {
        BleManager::stopJammer();
        request->send(200, "application/json", "{\"success\":true,\"status\":\"idle\"}");
    });

    ServerManager::server.on("/api/ble/jammer/config", HTTP_PUT, [](AsyncWebServerRequest *request) {
        if (request->hasParam("power")) {
            int power = request->getParam("power")->value().toInt();
            BleManager::setJammerPower(power);
        }
        request->send(200, "application/json", "{\"success\":true}");
    });
  }
};