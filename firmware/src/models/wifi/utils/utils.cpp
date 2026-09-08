#include "Arduino.h"
#include "WiFi.h"
#include "esp_wifi.h"
#include "utils.h"

static PmkidItem *parsePmkidFromKeyData(uint8_t *keyData, const uint16_t length){
    uint8_t *keyDataIndex = keyData;
    uint8_t *keyDataMaxIndex = keyData + length;

    PmkidItem *pmkid_item_head = NULL;
    KeyDataField *keyDataField;
    do{
        keyDataField = (KeyDataField *) keyDataIndex;

        ESP_LOGV(TAG, "EAPOL-Key -> Key-Data -> type=%x; length=%x; oui=%x; data_type=%x",
                    keyDataField->type, 
                    keyDataField->length, 
                    keyDataField->oui,
                    keyDataField->data_type);
        
        if(keyDataField->type != KEY_DATA_TYPE){
            ESP_LOGD(TAG, "Wrong type %x (expected %x)", keyDataField->type, KEY_DATA_TYPE);
            continue;
        }

        if(ntohl(keyDataField->oui) != KEY_DATA_OUI_IEEE80211){
            ESP_LOGD(TAG, "Wrong OUI %x (expected %x)", keyDataField->oui, KEY_DATA_OUI_IEEE80211);
            continue;
        }

        if(keyDataField->data_type != KEY_DATA_DATA_TYPE_PMKID_KDE){
            ESP_LOGD(TAG, "Wrong data type %x (expected %x)", keyDataField->data_type, KEY_DATA_DATA_TYPE_PMKID_KDE);
            continue;
        }

        ESP_LOGI(TAG, "Found PMKID: ");
        PmkidItem *pmkid_item = (PmkidItem *) malloc(sizeof(PmkidItem));
        pmkid_item->next = pmkid_item_head;
        pmkid_item_head = pmkid_item;
        for(unsigned i = 0; i < 16; i++){
            pmkid_item->pmkid[i] = keyDataField->data[i];
            printf("%02x", pmkid_item->pmkid[i]);
        }
        printf("\n");

    } while((keyDataIndex = keyDataField->data + keyDataField->length - 4 + 1) < keyDataMaxIndex); 

    return pmkid_item_head;
}

namespace wifi {
    void turnModeAPSTA(){
        wifi_mode_t currentMode = WiFi.getMode();
        if (currentMode == WIFI_MODE_AP) {
            WiFi.mode(WIFI_MODE_APSTA);
        } else if (currentMode == WIFI_MODE_NULL) {
            WiFi.mode(WIFI_MODE_STA);
        }
    }

    void disableModeAPSTA(){
        wifi_mode_t currentMode = WiFi.getMode();
        
        if (currentMode == WIFI_MODE_APSTA) {
            WiFi.mode(WIFI_MODE_AP);
        } else if(currentMode == WIFI_MODE_STA){
            WiFi.mode(WIFI_MODE_NULL);
        }
    }

    EapolPacket *parseEapolPacket(DataFrame *frame) {
        uint8_t *frameBuffer = frame->body;

        if(frame->header.frameControl.protectedFrame == 1) {
            return NULL;
        }
        
        if(frame->header.frameControl.subtype > 7) {
            frameBuffer += 2;
        }
        frameBuffer += sizeof(LlcSnapHeader);

        if(ntohs(*(uint16_t *) frameBuffer) == ETHER_TYPE_EAPOL) {
            frameBuffer += 2;
            return (EapolPacket *) frameBuffer; 
        }
        return NULL;
    }

    EapolKeyPacket *parseEapolKeyPacket(EapolPacket *eapolPacket){
        if(eapolPacket->header.packetType != EAPOL_KEY){
            return NULL;
        }
        return (EapolKeyPacket *) eapolPacket->packetBody;
    }

    PmkidItem *parsePmkid(EapolKeyPacket *eapolKey) {
        if(eapolKey->keyDataLength == 0){
            ESP_LOGD(TAG, "Empty Key Data");
            return NULL;
        }

        if(encryptedKeyData(eapolKey)){
            ESP_LOGD(TAG, "Key Data encrypted");
            return NULL;
        }

        return parsePmkidFromKeyData(eapolKey->keyData, ntohs(eapolKey->keyDataLength));
    }

    bool isArrayZero(uint8_t *array, unsigned size){
        for(unsigned i = 0; i < size; i++){
            if(array[i] != 0){
                return false;
            }
        }
        return true;
    }

    bool isFrameBssidMatching(const wifi_promiscuous_pkt_t *frame, const uint8_t *targetBssid) {
        HeaderFrame* header = (HeaderFrame *) frame->payload;
        return memcmp(header->addr3, targetBssid, 6) == 0;
    }

    const char* encryptionTypeToLabel(wifi_auth_mode_t type) {
        switch (type) {
            case WIFI_AUTH_OPEN: return "OPEN";
            case WIFI_AUTH_WEP: return "WEP";
            case WIFI_AUTH_WPA_PSK: return "WPA";
            case WIFI_AUTH_WPA2_PSK: return "WPA2";
            case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
            case WIFI_AUTH_WPA3_PSK: return "WPA3";
            case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
            default: return "UNKNOWN";
        }
    }

    void wifiFilterFrameTypes(bool data, bool mgmt, bool ctrl) {
        wifi_promiscuous_filter_t filter = { .filter_mask = 0 };
        if(data) {
            filter.filter_mask |= WIFI_PROMIS_FILTER_MASK_DATA;
        }
        if(mgmt) {
            filter.filter_mask |= WIFI_PROMIS_FILTER_MASK_MGMT;
        }
        if(ctrl) {
            filter.filter_mask |= WIFI_PROMIS_FILTER_MASK_CTRL;
        }
        esp_wifi_set_promiscuous_filter(&filter);
    }

    HandshakeType parseHandshakeType(const char* value) {
        if (value == nullptr) {
            return HandshakeType::Unknown;
        }

        if (strcmp(value, "passive") == 0) {
            return HandshakeType::Passive;
        }

        if (strcmp(value, "deauth") == 0) {
            return HandshakeType::Deauth;
        }

        if (strcmp(value, "broadcast") == 0) {
            return HandshakeType::Broadcast;
        }

        return HandshakeType::Unknown;
    }

    bool switchChannelSafelyWithoutDisconnect(uint8_t new_channel) {
        bool was_promiscuous = false;
        esp_wifi_get_promiscuous(&was_promiscuous);

        esp_wifi_set_promiscuous(false);
        delay(10);

        wifi_config_t ap_config;
        esp_err_t err = esp_wifi_get_config(WIFI_IF_AP, &ap_config);
        
        if (err != ESP_OK) {
            Serial.println("❌ Failed to get AP configuration");
            if (was_promiscuous) esp_wifi_set_promiscuous(true);
            return false;
        }

        ap_config.ap.channel = new_channel;

        err = esp_wifi_set_config(WIFI_IF_AP, &ap_config);

        if (err == ESP_OK) {
            Serial.printf("✅ Configuration updated. New channel.: %d\n", WiFi.channel());
        } else {
            Serial.printf("❌ Configuration change error: %d\n", err);
        }

        esp_wifi_set_promiscuous(was_promiscuous);

        return (err == ESP_OK);
    }

    uint16_t keyInfo(const EapolKeyPacket* packet) {
        return ntohs(packet->keyInformation);
    }

    uint8_t keyDescriptorVersion(const EapolKeyPacket* packet) {
        return keyInfo(packet) & 0x07;
    }

    bool keyMic(const EapolKeyPacket* packet) {
        return keyInfo(packet) & (1 << 8);
    }

    bool keyAck(const EapolKeyPacket* packet) {
        return keyInfo(packet) & (1 << 7);
    }

    bool install(const EapolKeyPacket* packet) {
        return keyInfo(packet) & (1 << 6);
    }

    bool secure(const EapolKeyPacket* packet) {
        return keyInfo(packet) & (1 << 9);
    }

    bool encryptedKeyData(const EapolKeyPacket* packet) {
        return (keyInfo(packet) & (1 << 12)) != 0;
    }

    bool isM1(const EapolKeyPacket* packet) {
        return keyAck(packet)
            && !keyMic(packet)
            && !install(packet);
    }

    bool isM2(const EapolKeyPacket* packet) {
        return !keyAck(packet)
            && keyMic(packet)
            && !secure(packet);
    }

    bool isM3(const EapolKeyPacket* packet) {
        return keyAck(packet)
            && keyMic(packet)
            && install(packet);
    }

    bool isM4(const EapolKeyPacket* packet) {
        return !keyAck(packet)
            && keyMic(packet)
            && secure(packet);
    }
}
  
  
  