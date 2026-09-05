#pragma once
#include "Arduino.h"

enum class BleDeviceStatus : uint8_t {
    Unknown = 0,
    Connectable = 1, 
    NonConnectable = 2, 
    Directed = 3      
};

struct BleRawEvent {
    char address[18];
    int rssi;
    char name[65];
    BleDeviceStatus status;
};