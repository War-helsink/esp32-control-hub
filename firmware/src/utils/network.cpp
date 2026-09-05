#include "network.h"

namespace utils {
    String macToString(const uint8_t* mac) {
        char buf[18];
        snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return String(buf);
    }

    void macStringToUint8(const String& macStr, uint8_t* outMac) {
        sscanf(macStr.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
            &outMac[0], &outMac[1], &outMac[2], &outMac[3], &outMac[4], &outMac[5]);
    }

    bool matchesTargetBssid(const uint8_t* dest, const uint8_t* src, const uint8_t* bssid, const uint8_t* targetBssid) {
        return memcmp(dest, targetBssid, 6) == 0 ||
            memcmp(src, targetBssid, 6) == 0 ||
            memcmp(bssid, targetBssid, 6) == 0;
    }
}