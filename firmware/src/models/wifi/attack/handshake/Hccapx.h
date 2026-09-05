#pragma once

#include "models/wifi/types/frame.h"
#include "models/wifi/types/hccapx.h"

class HccapxSerializer {
public:
    static bool ready();
    static void init(const char* ssid);

    static const Hccapx& get();
    static void clear();
    static void serializerAddFrame(DataFrame *dataFrame);

private:
    static Hccapx _hccapx;
    static unsigned _messageAp;
    static unsigned _messageSta;
    static unsigned _eapolSource;

    static void apMessage(DataFrame *frame, EapolPacket* eapol_packet, EapolKeyPacket *eapol_key_packet);
    static void apMessageM1(EapolKeyPacket *eapolKeyPacket);
    static void apMessageM3(EapolPacket* eapolPacket, EapolKeyPacket *eapolKeyPacket);
	static void staMessage(DataFrame *frame, EapolPacket* eapol_packet, EapolKeyPacket *eapol_key_packet);
    static void staMessageM2(EapolPacket* eapolPacket, EapolKeyPacket *eapolKeyPacket);
    static void staMessageM4(EapolPacket* eapolPacket, EapolKeyPacket *eapolKeyPacket);
    static unsigned saveEapol(EapolPacket *eapolPacket, EapolKeyPacket *eapolKeyPacket);
};