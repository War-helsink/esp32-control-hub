#include "Bluetooth.h"
#include "core/ServerManager.h"
#include "models/bluetooth/attack/Jammer.h"
#include "models/bluetooth/scanner/Scanner.h"
#include "models/status/Manager.h"
#include "utils/utils.h"

void HttpApiBluetooth::registerRoutes() {
    registerJammerRoutes();
    registerScannerRoutes();
}

void HttpApiBluetooth::registerScannerRoutes() {
    ServerManager::server.on("/api/ble/scanner/start", HTTP_POST, [](AsyncWebServerRequest *request) {
        const bool started = StatusManager::start(
            StatusModule::Bluetooth,
            StatusAction::BluetoothScan
        );

        if (!started) {
            return utils::sendStatusResponse(
                request,
                false,
                200
            );
        }

        BleScanner::start();
        utils::sendStatusResponse(
            request,
            true,
            200
        );
    });

    ServerManager::server.on("/api/ble/scanner/stop", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (!StatusManager::isRunning(StatusModule::Bluetooth, StatusAction::BluetoothScan)) {
            return utils::sendStatusResponse(
                request,
                false,
                200
            );
        }

        BleScanner::stop();

        StatusManager::stop(StatusModule::Bluetooth, StatusAction::BluetoothScan);
        utils::sendStatusResponse(
            request,
            true,
            200
        );
    });
};


void HttpApiBluetooth::registerJammerRoutes() {
    ServerManager::server.on("/api/ble/jammer/start", HTTP_POST, [](AsyncWebServerRequest *request) {
        const bool started = StatusManager::start(
            StatusModule::Bluetooth,
            StatusAction::BluetoothJammer
        );

        if (!started) {
            return utils::sendStatusResponse(
                request,
                false,
                200
            );
        }

        BleJammer::start();
        
        utils::sendStatusResponse(
            request,
            true,
            200
        );
    });

    ServerManager::server.on("/api/ble/jammer/stop", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (!StatusManager::isRunning(StatusModule::Bluetooth, StatusAction::BluetoothJammer)) {
            return utils::sendStatusResponse(
                request,
                false,
                200
            );
        }

        BleJammer::stop();

        StatusManager::stop(StatusModule::Bluetooth, StatusAction::BluetoothJammer);
        utils::sendStatusResponse(
            request,
            true,
            200
        );
    });

    ServerManager::server.on("/api/ble/jammer/config", HTTP_PUT, [](AsyncWebServerRequest *request) {
        if (request->hasParam("power")) {
            int power = request->getParam("power")->value().toInt();
            BleJammer::setPower(power);
        }
        request->send(200, "application/json", "{\"success\":true}");
    });
};
