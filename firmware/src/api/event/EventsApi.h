#pragma once

#include "ArduinoJson.h"
#include "models/status/types.h"
#include "models/wifi/types/info.h"
#include "models/bluetooth/types/info.h"

class EventsApi {
  public:
    static void begin();
    static void sendStatus(const StatusState& state);
    static void sendDeviceFound(const BleRawEvent &dev);
    static void sendNetworkFound(const WifiNetworkInfo &network);
    static void sendClientFound(const WifiClientInfo &client);
    static void sendHccapxStatus(const boolean isAvailable);

    static void sendNetworkSelect();
    static void sendClientSelect();
  private:
    static String serializeStatus(const StatusState* state);
    static String serializeNetwork(const WifiNetworkInfo* network);
    static String serializeClient(const WifiClientInfo* client);
};