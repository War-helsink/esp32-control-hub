#pragma once
#include "Arduino.h"

#define ETHER_TYPE_EAPOL 0x888e
#define KEY_DATA_TYPE 0xdd
#define KEY_DATA_OUI_IEEE80211 0x00fac00
#define KEY_DATA_DATA_TYPE_PMKID_KDE 4

const uint8_t EMPTY_BSSID[6] = {0};
const uint8_t BROADCAST_ADDRESS[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct FrameControl{
  uint8_t protocolVersion:2;
  uint8_t type:2;
  uint8_t subtype:4;
  uint8_t toDs:1;
  uint8_t fromDs:1;
  uint8_t moreFragments:1;
  uint8_t retry:1;
  uint8_t powerManagement:1;
  uint8_t moreData:1;
  uint8_t protectedFrame:1;
  uint8_t htcOrder:1;
};

struct HeaderFrame {
  FrameControl frameControl;
  uint16_t duration;
  uint8_t addr1[6];
  uint8_t addr2[6];
  uint8_t addr3[6];
  uint16_t sequenceCtrl;
};

struct EapolPacketHeader {
	uint8_t version;
	uint8_t packetType;
	uint16_t packetBodyLength;
};

struct EapolPacket {
	EapolPacketHeader header;
	uint8_t packetBody[];
};

struct __attribute__((__packed__)) EapolKeyPacket {
  uint8_t descriptorType;
  uint16_t keyInformation;
  uint16_t keyLength;
  uint8_t keyReplayCounter[8];
  uint8_t keyNonce[32];
  uint8_t keyIv[16];
  uint8_t keyRsc[8];
  uint8_t reserved[8];
  uint8_t keyMic[16];
  uint16_t keyDataLength;
  uint8_t keyData[];
};

struct DataFrame {
  HeaderFrame header;
  uint8_t body[];
};

struct LlcSnapHeader {
  uint8_t snapDsap;
  uint8_t snapSsap;
  uint8_t control;
  uint8_t encapsulation[3];
};

struct DeauthFrame {
  uint8_t frameControl[2] = { 0xC0, 0x00 };
  uint8_t duration[2] = { 0x3a, 0x01 };
  uint8_t station[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  uint8_t sender[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t accessPoint[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t fragmentSequence[2] = { 0xF0, 0xFF };
  uint16_t reason[2] = { 0x02, 0x00 };
};

struct __attribute__((packed)) BeaconHeader {
  uint16_t frameControl      = 0x0080; 
  uint16_t duration          = 0x0000;
  uint8_t  dest[6]           = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; 
  uint8_t  src[6]            = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 
  uint8_t  bssid[6]          = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 
  uint16_t fragmentSequence  = 0x0000;

  uint64_t timestamp         = 0;   
  uint16_t beacon_interval   = 100;   
  uint16_t capability_info   = 0x0001; 
  
  uint8_t  tag_ssid_type     = 0x00;   
  uint8_t  tag_ssid_len      = 0;
};

enum EapolPacketTypes{
  EAPOL_EAP_PACKET = 0,
  EAPOL_START,
  EAPOL_LOGOFF,
  EAPOL_KEY,
  EAPOL_ENCAPSULATED_ASF_ALERT,
  EAPOL_MKA,
  EAPOL_ANNOUNCEMENT_GENERIC,
  EAPOL_ANNOUNCEMENT_SPECIFIC,
  EAPOL_ANNOUNCEMENT_REQ
};

typedef struct PmkidItem {
  uint8_t pmkid[16];
  struct PmkidItem *next;
} PmkidItem;

struct __attribute__((__packed__)) KeyDataField {
  uint8_t type;
  uint8_t length;
  uint32_t oui:24;
  uint32_t data_type:8;
  uint8_t data[];
};

enum class HandshakeType {
  Passive,
  Deauth,
  Broadcast,
  Unknown
};