#pragma once

#include "models/wifi/types/pcap.h"

class PcapSerializer {
public:
    static bool ready();
    static uint8_t* init();

    static const unsigned getSize();
    static const uint8_t* getBuffer();
    static void clear();
    static void serializerAddFrame(const uint8_t *buffer, unsigned size, unsigned tsUsec);

private:
    static unsigned pcapSize;
    static uint8_t *pcapBuffer;
};