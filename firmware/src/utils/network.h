#pragma once
#include "Arduino.h"

namespace utils {
    String macToString(const uint8_t* bssid);
    void macStringToUint8(const String& macStr, uint8_t* outMac);
    bool matchesTargetBssid(const uint8_t* dest, const uint8_t* src, const uint8_t* bssid, const uint8_t* targetBssid);
}