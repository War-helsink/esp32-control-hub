#include "Scanner.h"
#include "core/ServerManager.h"
#include "models/wifi/scanner/Network.h"
#include "models/wifi/select/WifiSelect.h"
#include "models/wifi/types/frame.h"
#include "models/status/Manager.h"
#include "utils/utils.h"

void HttpApiWifiScanner::registerRoutes() {
    registerSelect();
    registerScannerNetwork();
}

void HttpApiWifiScanner::registerScannerNetwork(){
    ServerManager::server.on("/api/wifi/scanner/start", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            const bool started = StatusManager::start(
                StatusModule::Wifi,
                StatusAction::WifiScan
            );

            if (!started) {
                return utils::sendStatusResponse(
                    request,
                    false,
                    200
                );
            }

            WifiScanner::start();
            WifiSelect::deselectNetwork();

            utils::sendStatusResponse(
                request,
                true,
                200
            );
        }
    );

    ServerManager::server.on("/api/wifi/scanner/stop", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            if (!StatusManager::isRunning(StatusModule::Wifi, StatusAction::WifiScan)) {
                return utils::sendStatusResponse(
                    request,
                    false,
                    200
                );
            }
            
            WifiScanner::stop();

            StatusManager::stop(
                StatusModule::Wifi,
                StatusAction::WifiScan
            );

            utils::sendStatusResponse(
                request,
                true,
                200
            );
        }
    );
}


void HttpApiWifiScanner::registerSelect(){
     ServerManager::server.on("/api/wifi/network/select/clear", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            WifiSelect::deselectNetwork();

            utils::sendStatusResponse(
                request,
                true,
                200
            );
        }
    );

    ServerManager::server.on("/api/wifi/client/select/clear", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            WifiSelect::deselectClient();

            utils::sendStatusResponse(
                request,
                true,
                200
            );
        }
    );

    AsyncCallbackJsonWebHandler* selectNetworkHandler = new AsyncCallbackJsonWebHandler(
        "/api/wifi/network/select",
        [](AsyncWebServerRequest *request, JsonVariant &json) {
            JsonObject body = json.as<JsonObject>();

            if (!body["bssid"].is<const char*>() ||
                !body["ssid"].is<const char*>() ||
                !body["channel"].is<int>() ||
                !body["rssi"].is<int>() ||
                !body["encryption"].is<const char*>()
            ) {
                return request->send(400, "application/json", "{\"success\":false,\"error\":\"invalid network\"}");
            }

            WifiNetworkInfo network = {};

            utils::macStringToUint8(body["bssid"].as<String>(), network.bssid);
            strlcpy(network.ssid, body["ssid"].as<const char*>(), sizeof(network.ssid));

            network.channel = body["channel"].as<uint8_t>();
            network.rssi = body["rssi"].as<int8_t>();

            strlcpy(network.encryptionLabel, body["encryption"].as<const char*>(), sizeof(network.encryptionLabel));

            WifiSelect::selectNetwork(network);
            
            utils::sendStatusResponse(
                request,
                true,
                200
            );
        }
    );
    ServerManager::server.addHandler(selectNetworkHandler);

    AsyncCallbackJsonWebHandler* selectClientHandler = new AsyncCallbackJsonWebHandler(
        "/api/wifi/client/select",
        [](AsyncWebServerRequest* request, JsonVariant& json) {
            JsonObject body = json.as<JsonObject>();

            if (
                !body["mac"].is<const char*>() ||
                !body["bssid"].is<const char*>() ||
                !body["rssi"].is<int>()
            ) {
                return request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"invalid client\"}"
                );
            }

            WifiClientInfo client = {};

            utils::macStringToUint8(body["mac"].as<String>(), client.mac);
            utils::macStringToUint8(body["bssid"].as<String>(), client.bssid);
            client.rssi = body["rssi"].as<int8_t>();

            WifiSelect::selectClient(client);

            utils::sendStatusResponse(
                request,
                true,
                200
            );
        }
    );
    ServerManager::server.addHandler(selectClientHandler);
}