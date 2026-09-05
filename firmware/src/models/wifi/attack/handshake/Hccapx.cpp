#include "Hccapx.h"
#include "core/Logger.h"
#include "models/wifi/utils/utils.h"

static const char* TAG = "Hccapx-Serializer";

#define HCCAPX_SIGNATURE 0x58504348
#define HCCAPX_KEYVER_WPA2 2
#define HCCAPX_MAX_EAPOL_SIZE 256

unsigned HccapxSerializer::_messageAp = 0;
unsigned HccapxSerializer::_messageSta = 0;
unsigned HccapxSerializer::_eapolSource = 0;

Hccapx HccapxSerializer::_hccapx = { 
    .signature = HCCAPX_SIGNATURE, 
    .version = 4, 
    .messagePair = 255,
    .keyver = HCCAPX_KEYVER_WPA2
};

bool HccapxSerializer::ready() {
    if(_hccapx.messagePair == 255){
        return false;
    }
    if (_hccapx.signature != HCCAPX_SIGNATURE) {
        return false;
    }
    if (_hccapx.version != 4) {
        return false;
    }
    if (_hccapx.messagePair > 5) {
        return false;
    }
    if (_hccapx.essidLen == 0 ||
        _hccapx.essidLen > sizeof(_hccapx.essid)) {
        return false;
    }
    if (_hccapx.eapolLen == 0 ||
        _hccapx.eapolLen > sizeof(_hccapx.eapol)) {
        return false;
    }
    if (wifi::isArrayZero(_hccapx.macAp, sizeof(_hccapx.macAp))) {
        return false;
    }
    if (wifi::isArrayZero(_hccapx.macSta, sizeof(_hccapx.macSta))) {
        return false;
    }
    if (wifi::isArrayZero(_hccapx.nonceAp, sizeof(_hccapx.nonceAp))) {
        return false;
    }
    if (wifi::isArrayZero(_hccapx.nonceSta, sizeof(_hccapx.nonceSta))) {
        return false;
    }
    if (wifi::isArrayZero(_hccapx.keymic, sizeof(_hccapx.keymic))) {
        return false;
    }
    return true;
}

const Hccapx& HccapxSerializer::get() {
    return _hccapx;
}

void HccapxSerializer::init(const char* ssid){
    const uint8_t* ssidBytes = reinterpret_cast<const uint8_t*>(ssid);
	size_t ssidLength = strlen(ssid);

    _hccapx.essidLen = ssidLength;
    memcpy(_hccapx.essid, ssidBytes, ssidLength);
    _hccapx.messagePair = 255;
}

void HccapxSerializer::clear() {
    _hccapx = Hccapx{ 
        .signature = HCCAPX_SIGNATURE, 
        .version = 4, 
        .messagePair = 255,
        .keyver = HCCAPX_KEYVER_WPA2
    };

    _messageAp = 0;
    _messageSta = 0;
    _eapolSource = 0;
}


void HccapxSerializer::serializerAddFrame(DataFrame *dataFrame){
	EapolPacket *eapolPacket = wifi::parseEapolPacket((DataFrame *) dataFrame);
	EapolKeyPacket *eapolKeyPacket = wifi::parseEapolKeyPacket(eapolPacket);

    if(memcmp(dataFrame->header.addr2, dataFrame->header.addr3, 6) == 0){
    	apMessage(dataFrame, eapolPacket, eapolKeyPacket);
    } 
    else if(memcmp(dataFrame->header.addr1, dataFrame->header.addr3, 6) == 0){
    	staMessage(dataFrame, eapolPacket, eapolKeyPacket);
    } 
    else {
        LOG_E(TAG, "Unknown frame format. BSSID is not source nor destionation.");
    }
};


void HccapxSerializer::apMessage(DataFrame *frame, EapolPacket* eapolPacket, EapolKeyPacket *eapolKeyPacket){
    if((!wifi::isArrayZero(_hccapx.macSta, 6)) && (memcmp(frame->header.addr1, _hccapx.macSta, 6) != 0)){
        return;
    }
    if(_messageAp == 0){
        memcpy(_hccapx.macAp, frame->header.addr2, 6);
    }
    if(wifi::isM1(eapolKeyPacket)){
        apMessageM1(eapolKeyPacket);
    }
    if(wifi::isM3(eapolKeyPacket)){
        apMessageM3(eapolPacket, eapolKeyPacket);
    }
}

void HccapxSerializer::staMessage(DataFrame *frame, EapolPacket* eapolPacket, EapolKeyPacket *eapolKeyPacket){
     if(wifi::isArrayZero(_hccapx.macSta, 6)){
        memcpy(_hccapx.macSta, frame->header.addr2, 6);
    }
    else if(memcmp(frame->header.addr2, _hccapx.macSta, 6) != 0){
        return;
    }

    if(wifi::isM2(eapolKeyPacket)){
        staMessageM2(eapolPacket, eapolKeyPacket);
    } 
    if(wifi::isM4(eapolKeyPacket)){
        staMessageM4(eapolPacket, eapolKeyPacket);
    }
}

void HccapxSerializer::apMessageM1(EapolKeyPacket *eapolKeyPacket){
    LOG_I(TAG, "From AP M1");
    _messageAp = 1;
    memcpy(_hccapx.nonceAp, eapolKeyPacket->keyNonce, 32);
}

void HccapxSerializer::apMessageM3(EapolPacket* eapolPacket, EapolKeyPacket *eapolKeyPacket){
    LOG_I(TAG, "From AP M3");
    memcpy(_hccapx.nonceAp, eapolKeyPacket->keyNonce, sizeof(_hccapx.nonceAp));

    _messageAp = 3;

    if (_eapolSource == 2) {
        _hccapx.messagePair = 2;
        return;
    }

    if (saveEapol(eapolPacket, eapolKeyPacket) != 0) {
        return;
    }

    _eapolSource = 3;

    if (_messageSta == 2) {
        _hccapx.messagePair = 3;
    }
}

void HccapxSerializer::staMessageM2(EapolPacket* eapolPacket, EapolKeyPacket *eapolKeyPacket){
    LOG_I(TAG, "From STA M2");
    _messageSta = 2;
    memcpy(_hccapx.nonceSta, eapolKeyPacket->keyNonce, 32);
    if(saveEapol(eapolPacket, eapolKeyPacket) != 0){
        return;
    }
    _eapolSource = 2;
    if(_messageAp == 1){
        _hccapx.messagePair = 0;
        return;
    }
}

void HccapxSerializer::staMessageM4(EapolPacket* eapolPacket, EapolKeyPacket *eapolKeyPacket){
    LOG_I(TAG, "From STA M4");
    if((_messageSta == 2) && (_eapolSource != 0)){
        LOG_I(TAG, "Already have M2, not worth");
        return;
    }
    if(_messageAp == 0){
        LOG_I(TAG, "Not enought handshake messages received.");
        return;
    }
    if(_eapolSource == 3){
        _hccapx.messagePair = 4;
        return;
    }
    if(saveEapol(eapolPacket, eapolKeyPacket) != 0){
        return;
    }
    _eapolSource = 4;
    if(_messageAp == 1){
        _hccapx.messagePair = 1;
    }
    if(_messageAp == 3){
        _hccapx.messagePair = 5;
    }
}

unsigned HccapxSerializer::saveEapol(EapolPacket *eapolPacket, EapolKeyPacket *eapolKeyPacket){
    const unsigned eapolLen = sizeof(EapolPacketHeader) + ntohs(eapolPacket->header.packetBodyLength);

    if (eapolLen > sizeof(_hccapx.eapol)) {
        LOG_E(TAG, "EAPOL is too long: %u", eapolLen);
        return 1;
    }

    _hccapx.eapolLen = eapolLen;
    _hccapx.keyver = wifi::keyDescriptorVersion(eapolKeyPacket);

    const size_t micOffset = sizeof(EapolPacketHeader) + offsetof(EapolKeyPacket, keyMic);

    memcpy(_hccapx.eapol, eapolPacket, eapolLen);
    memcpy(_hccapx.keymic, eapolKeyPacket->keyMic, sizeof(_hccapx.keymic));
    memset(&_hccapx.eapol[micOffset], 0, sizeof(_hccapx.keymic));
    return 0;
}