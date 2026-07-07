#include "BleManager.h"
#include "BleScanner.h"
#include "BleJammer.h"
#include "core/Logger.h"

BleMode BleManager::_currentMode = BleMode::Idle;

static bool _stackInitialized = false; 
static const char* TAG = "BLE_MANAGER";

void BleManager::initStack() {
    if (!_stackInitialized) {
        NimBLEDevice::init("");
        _stackInitialized = true;
    }
}

void BleManager::deinitStack() {
    if (_stackInitialized) {
        NimBLEDevice::deinit(true);
        _stackInitialized = false;
    }
}

void BleManager::startScan() {
    LOG_W(TAG, "Forcing cleanup before starting scanner...");
    BleScanner::release();
    BleJammer::release();
    deinitStack();

    initStack();
    _currentMode = BleMode::Scanning;
    BleScanner::start();
    LOG_I(TAG, "Scanner is now running.");
}

void BleManager::stopScan() {
    BleScanner::stop();
    deinitStack();
    _currentMode = BleMode::Idle;
    LOG_I(TAG, "Scanner stopped. System returned to Idle.");
}

void BleManager::startJammer() {
    LOG_W(TAG, "Forcing cleanup before starting jammer...");
    BleScanner::release();
    BleJammer::release();
    deinitStack();

    initStack();
    _currentMode = BleMode::Jamming;
    BleJammer::start();
    LOG_I(TAG, "Jammer is now running.");
}

void BleManager::stopJammer() {
    BleJammer::stop();
    deinitStack();
    _currentMode = BleMode::Idle;
    LOG_I(TAG, "Jammer stopped. System returned to Idle.");
}

void BleManager::setJammerPower(int power) {
    std::string msg = "Set jammer power " + std::to_string(power) + ".";
    LOG_I(TAG, msg.c_str());
    BleJammer::setPower(power);
}

int BleManager::getJammerPower() {
     LOG_I(TAG, "Get jammer power.");
    return BleJammer::getPower();
}