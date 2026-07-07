// BleManager.h
#pragma once
#include <Arduino.h>

enum class BleMode : uint8_t { Idle = 0, Scanning = 1, Jamming = 2 };

class BleManager {
public:
    static void startScan();
    static void stopScan();
    static void startJammer();
    static void stopJammer();
    static void setJammerPower(int power);
    
    static BleMode getMode() { return _currentMode; }
    static int getJammerPower();

private:
    static void initStack();
    static void deinitStack();
    static BleMode _currentMode;
};