#pragma once

#include "models/wifi/types/info.h"

class WifiSelect {
public:
    static void selectNetwork(const WifiNetworkInfo& network);
    static void deselectNetwork();

    static void selectClient(const WifiClientInfo& client);
    static void deselectClient();

    static bool hasNetwork();
    static bool hasClient();

    static const WifiNetworkInfo* getNetwork();
    static const WifiClientInfo* getClient();

private:
    static WifiNetworkInfo* _network;
    static WifiClientInfo* _client;
};