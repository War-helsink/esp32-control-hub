#include "ArduinoJson.h"
#include "EventsApi.h"

#include "models/wifi/select/WifiSelect.h"
#include "models/status/Manager.h"
#include "core/ServerManager.h"
#include "utils/utils.h"

void EventsApi::begin(){
    ServerManager::events.onConnect([](AsyncEventSourceClient *client) { 
        client->send("connected", "init", millis());

        const StatusState* status = StatusManager::getState();
        const WifiNetworkInfo* network = WifiSelect::getNetwork();
        const WifiClientInfo* clientInfo = WifiSelect::getClient();

        String statusJson = serializeStatus(status);
        String networkJson = serializeNetwork(network);
        String clientJson = serializeClient(clientInfo);
        
        client->send(statusJson.c_str(), "status", millis());
        client->send(networkJson.c_str(), "wifi:network:select", millis());
        client->send(clientJson.c_str(), "wifi:client:select", millis());
    });
}

void EventsApi::sendStatus(const StatusState& state){
    if (ServerManager::events.count() == 0) return;

    String json = serializeStatus(&state);
    ServerManager::events.send(json.c_str(), "status", millis());
}

void EventsApi::sendNetworkSelect(){
    if (ServerManager::events.count() == 0) return;

    const WifiNetworkInfo* network = WifiSelect::getNetwork();

    String json = serializeNetwork(network);
    ServerManager::events.send(json.c_str(), "wifi:network:select", millis());
};

void EventsApi::sendClientSelect(){
    if (ServerManager::events.count() == 0) return;

    const WifiClientInfo* client = WifiSelect::getClient();

    String json = serializeClient(client);
    ServerManager::events.send(json.c_str(), "wifi:client:select",millis());
};

void EventsApi::sendDeviceFound(const BleRawEvent &dev) {
    if (ServerManager::events.count() == 0) return;

    JsonDocument doc;

    JsonObject payload = doc["payload"].to<JsonObject>();
    payload["address"] = dev.address;
    payload["rssi"] = dev.rssi;
    payload["name"] = strlen(dev.name) > 0 ? dev.name : "Unknown BLE Device";

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

    ServerManager::events.send(eventJson.c_str(), "ble:general:update", millis());
}

void EventsApi::sendNetworkFound(const WifiNetworkInfo &network) {
    if (ServerManager::events.count() == 0) return;

    String json = serializeNetwork(&network);
    ServerManager::events.send(json.c_str(), "wifi:network:found", millis());
}

void EventsApi::sendClientFound(const WifiClientInfo &client) {
    if (ServerManager::events.count() == 0) return;

    String json = serializeClient(&client);
    ServerManager::events.send(json.c_str(), "wifi:client:found",millis());
}

void EventsApi::sendHccapxStatus(const boolean isReady){
    if (ServerManager::events.count() == 0) return;
    
    JsonDocument doc;
    doc["isReady"] = isReady;

    String eventJson;
    serializeJson(doc, eventJson);

    ServerManager::events.send(eventJson.c_str(), "wifi:handshake:status", millis());
}


String EventsApi::serializeStatus(const StatusState* state) {
    JsonDocument doc;

    if (!state->running) {
        doc["status"] = "normal";
        doc["activeProcess"] = nullptr;
    } else {
        doc["status"] = "running";

        JsonObject activeProcess =doc["activeProcess"].to<JsonObject>();

        activeProcess["module"] = StatusManager::moduleToString(state->activeProcess.module);
        activeProcess["action"] = StatusManager::actionToString(state->activeProcess.action);
    }

    String json;
    serializeJson(doc, json);

    return json;
}

String EventsApi::serializeNetwork(const WifiNetworkInfo* network) {
    if(network == nullptr){
        return "null";
    }

    JsonDocument doc;

    JsonObject payload = doc["payload"].to<JsonObject>();
    payload["bssid"] = utils::macToString(network->bssid);
    payload["ssid"] = strlen(network->ssid) > 0 ? network->ssid : "(hidden)";
    payload["channel"] = network->channel;
    payload["rssi"] = network->rssi;
    payload["encryption"] = network->encryptionLabel;

    String json;
    serializeJson(doc, json);

    return json;
}

String EventsApi::serializeClient(const WifiClientInfo* client) {
    if(client == nullptr){
        return "null";
    }

    JsonDocument doc;

    JsonObject payload = doc["payload"].to<JsonObject>();
    payload["mac"] = utils::macToString(client->mac);
    payload["bssid"] = utils::macToString(client->bssid);
    payload["rssi"] = client->rssi;

    String json;
    serializeJson(doc, json);

    return json;
}