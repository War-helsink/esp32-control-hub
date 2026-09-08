#pragma once
#include "Arduino.h"
#include "models/wifi/types/frame.h"

namespace wifi {
    void turnModeAPSTA();
    void disableModeAPSTA();
    HandshakeType parseHandshakeType(const char* value);

    EapolPacket *parseEapolPacket(DataFrame *frame);
    EapolKeyPacket *parseEapolKeyPacket(EapolPacket *eapolPacket);
    PmkidItem *parsePmkid(EapolKeyPacket *eapolKey);
    bool isArrayZero(uint8_t *array, unsigned size);
    bool isFrameBssidMatching(const wifi_promiscuous_pkt_t *frame, const uint8_t *bssid);
    const char* encryptionTypeToLabel(wifi_auth_mode_t type);
    void wifiFilterFrameTypes(bool data, bool mgmt, bool ctrl);

    bool switchChannelSafelyWithoutDisconnect(uint8_t new_channel);

    uint16_t keyInfo(const EapolKeyPacket* packet);
    uint8_t keyDescriptorVersion(const EapolKeyPacket* packet);
    bool keyMic(const EapolKeyPacket* packet);
    bool keyAck(const EapolKeyPacket* packet);
    bool install(const EapolKeyPacket* packet);
    bool secure(const EapolKeyPacket* packet);
    bool encryptedKeyData(const EapolKeyPacket* packet);

    bool isM1(const EapolKeyPacket* packet);
    bool isM2(const EapolKeyPacket* packet);
    bool isM3(const EapolKeyPacket* packet);
    bool isM4(const EapolKeyPacket* packet);
}