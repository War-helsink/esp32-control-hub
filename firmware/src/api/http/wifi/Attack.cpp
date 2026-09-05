#include "Attack.h"
#include "core/ServerManager.h"
#include "models/wifi/attack/deauth/Deauth.h"
#include "models/wifi/attack/beacon/Beacon.h"
#include "models/wifi/attack/handshake/Handshake.h"
#include "models/wifi/attack/handshake/Hccapx.h"
#include "models/wifi/attack/handshake/Pcap.h"
#include "models/status/Manager.h"
#include "utils/utils.h"
#include "models/wifi/utils/utils.h"

void HttpApiWifiAttack::registerRoutes() {
	registerDeauthRoutes();
	registerBeaconRoutes();
	registerHandshakeRoutes();
}

void HttpApiWifiAttack::registerDeauthRoutes() {	
    ServerManager::server.on("/api/wifi/attack/deauth/start", HTTP_POST,
		[](AsyncWebServerRequest *request) {
            const bool started = StatusManager::start(
                StatusModule::Wifi,
                StatusAction::WifiDeauth
            );

            if (!started) {
                return utils::sendStatusResponse(
                    request,
                    false,
                    200
                );
            }
        
            WifiAttackDeauth::start();
        
            utils::sendStatusResponse(
                request,
                true,
                200
            );
		}
	);

	ServerManager::server.on("/api/wifi/attack/deauth/stop", HTTP_POST,
		[](AsyncWebServerRequest *request) {
			 if (!StatusManager::isRunning(StatusModule::Wifi, StatusAction::WifiDeauth)) {
                return utils::sendStatusResponse(
                    request,
                    false,
                    200
                );
            }

			WifiAttackDeauth::stop();

			StatusManager::stop(
                StatusModule::Wifi,
                StatusAction::WifiDeauth
            );
			utils::sendStatusResponse(
                request,
                true,
                200
            );
		}
	);
};

void HttpApiWifiAttack::registerBeaconRoutes(){
	AsyncCallbackJsonWebHandler* startBeaconHandler = new AsyncCallbackJsonWebHandler(
		"/api/wifi/attack/beacon/start",
		[](AsyncWebServerRequest *request, JsonVariant &json) {
			JsonObject body = json.as<JsonObject>();

			if (!body["channel"].is<int>()) {
				request->send(400, "application/json", "{\"success\":false,\"error\":\"missing channel\"}");
				return;
			}

			const bool started = StatusManager::start(
                StatusModule::Wifi,
                StatusAction::WifiBeacon
            );

			if (!started) {
                return utils::sendStatusResponse(
                    request,
                    false,
                    200
                );
            }

			uint8_t channel = body["channel"].as<uint8_t>();

			WifiAttackBeacon::start(channel);
			utils::sendStatusResponse(
                request,
                true,
                200
            );
		}
	);
	ServerManager::server.addHandler(startBeaconHandler);

	ServerManager::server.on("/api/wifi/attack/beacon/stop", HTTP_POST,
		[](AsyncWebServerRequest *request) {
			if (!StatusManager::isRunning(StatusModule::Wifi, StatusAction::WifiBeacon)) {
                return utils::sendStatusResponse(
                    request,
                    false,
                    200
                );
            }
			WifiAttackBeacon::stop();

			StatusManager::stop(
                StatusModule::Wifi,
                StatusAction::WifiBeacon
            );
            utils::sendStatusResponse(
                request,
                true,
                200
            );
		}
	);
}

void HttpApiWifiAttack::registerHandshakeRoutes(){
	AsyncCallbackJsonWebHandler* startBeaconHandler = new AsyncCallbackJsonWebHandler(
		"/api/wifi/attack/handshake/start",
		[](AsyncWebServerRequest *request, JsonVariant &json) {
            JsonObject body = json.as<JsonObject>();

			if (!body["type"].is<const char*>() || !body["ssid"].is<const char*>()) {
				request->send(400, "application/json", "{\"success\":false,\"error\":\"missing bssid or channel\"}");
				return;
			}


			const bool started = StatusManager::start(
                StatusModule::Wifi,
                StatusAction::WifiHandshake
            );

			if (!started) {
                return utils::sendStatusResponse(
                    request,
                    false,
                    200
                );
            }

            const char* ssid = body["ssid"];
            const char* typeValue = body["type"].as<const char*>();
            const HandshakeType type = wifi::parseHandshakeType(typeValue);

			WifiAttackHandshake::start(ssid, type);
			utils::sendStatusResponse(
                request,
                true,
                200
            );
		}
	);
	ServerManager::server.addHandler(startBeaconHandler);

	ServerManager::server.on("/api/wifi/attack/handshake/stop", HTTP_POST,
		[](AsyncWebServerRequest *request) {
			if (!StatusManager::isRunning(StatusModule::Wifi, StatusAction::WifiHandshake)) {
                return utils::sendStatusResponse(
                    request,
                    false,
                    200
                );
            }
			WifiAttackHandshake::stop();

			StatusManager::stop(
                StatusModule::Wifi,
                StatusAction::WifiHandshake
            );
            utils::sendStatusResponse(
                request,
                true,
                200
            );
		}
	);

    ServerManager::server.on("/api/wifi/attack/handshake/hccapx/download", HTTP_POST,
        [](AsyncWebServerRequest* request) {
            if (!HccapxSerializer::ready()) {
                request->send(404, "application/json", R"({"success":false,"error":"handshake_not_found"})");
                return;
            }

            const Hccapx& handshake = HccapxSerializer::get();

            AsyncResponseStream* response =
                request->beginResponseStream("application/octet-stream");

            response->addHeader(
                "Content-Disposition",
                "attachment; filename=\"ready.hccapx\""
            );

            response->addHeader(
                "Cache-Control",
                "no-store"
            );

            response->write(
                reinterpret_cast<const uint8_t*>(&handshake),
                sizeof(Hccapx)
            );

            request->send(response);
        }
    );
    ServerManager::server.on("/api/wifi/attack/handshake/pcap/download", HTTP_POST,
        [](AsyncWebServerRequest* request) {
            if (!PcapSerializer::ready()) {
                request->send(
                    404,
                    "application/json",
                    "{\"success\":false,\"error\":\"pcap_not_ready\"}"
                );
                return;
            }
            
            const uint8_t* buffer = PcapSerializer::getBuffer();
            const size_t size = PcapSerializer::getSize();

            AsyncResponseStream* response = request->beginResponseStream("application/vnd.tcpdump.pcap");

            response->addHeader(
                "Content-Disposition",
                "attachment; filename=\"ready.pcap\""
            );

            response->addHeader(
                "Cache-Control",
                "no-store"
            );

            response->write(buffer, size);

            request->send(response);
        }
    );
}