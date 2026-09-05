#include "WifiSelect.h"
#include "esp_wifi.h"
#include "api/event/EventsApi.h"
#include "models/wifi/utils/utils.h"

static WifiNetworkInfo networkStorage = {};
static WifiClientInfo clientStorage = {};

WifiNetworkInfo* WifiSelect::_network = nullptr;
WifiClientInfo* WifiSelect::_client = nullptr;

void WifiSelect::selectNetwork(const WifiNetworkInfo& network) {
    networkStorage = network;
    _network = &networkStorage;
    EventsApi::sendNetworkSelect();

    wifi::switchChannelSafelyWithoutDisconnect(_network->channel);

    deselectClient();
}

void WifiSelect::deselectNetwork() {
    _network = nullptr;
    EventsApi::sendNetworkSelect();

    deselectClient();
}

void WifiSelect::selectClient(const WifiClientInfo& client) {
    if (_network == nullptr) {
        return;
    }

    clientStorage = client;
    _client = &clientStorage;
    EventsApi::sendClientSelect();
}

void WifiSelect::deselectClient() {
    _client = nullptr;
    EventsApi::sendClientSelect();
}

bool WifiSelect::hasNetwork() {
    return _network != nullptr;
}

bool WifiSelect::hasClient() {
    return _client != nullptr;
}

const WifiNetworkInfo* WifiSelect::getNetwork() {
    return _network;
}

const WifiClientInfo* WifiSelect::getClient() {
    return _client;
}