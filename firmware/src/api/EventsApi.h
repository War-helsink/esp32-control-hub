#pragma once

#include <ArduinoJson.h>
#include "core/ServerManager.h"
#include "api/bluetooth/BleScanner.h"

class EventsApi {
public:
  static void begin() {
    ServerManager::events.onConnect([](AsyncEventSourceClient *client) {
      client->send("connected", "init", millis());
    });
  }

  static void sendDeviceFound(const BleRawEvent& dev) {
    if (ServerManager::events.count() == 0) return;

    JsonDocument doc;
    doc["type"] = "device_found";
    
    JsonObject payload = doc["payload"].to<JsonObject>();
    payload["address"] = dev.address;
    payload["rssi"] = dev.rssi;
    payload["name"] = strlen(dev.name) > 0 ? dev.name : "Unknown BLE Device";
    payload["timestamp"] = millis();

    switch (dev.status) {
        case BleDeviceStatus::Connectable:
            payload["status"] = "connectable";
            break;
        case BleDeviceStatus::NonConnectable:
            payload["status"] = "non_connectable";
            break;
        case BleDeviceStatus::Directed:
            payload["status"] = "directed";
            break;
        case BleDeviceStatus::Unknown:
        default:
            payload["status"] = "unknown";
            break;
    }

    String eventJson;
    serializeJson(doc, eventJson);

    ServerManager::events.send(
      eventJson.c_str(),
      "ble_general_update",
      millis()
    );
  }
};