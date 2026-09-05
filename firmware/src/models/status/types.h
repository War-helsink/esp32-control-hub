#pragma once
#include "Arduino.h"

enum class StatusModule {
    None,
    Wifi,
    Bluetooth
};

enum class StatusAction {
    None,

    WifiScan,
    WifiDeauth,
    WifiBeacon,
    WifiHandshake,

    BluetoothScan,
    BluetoothJammer
};

struct ActiveProcess {
    StatusModule module;
    StatusAction action;
};

struct StatusState {
    bool running;
    ActiveProcess activeProcess;
};