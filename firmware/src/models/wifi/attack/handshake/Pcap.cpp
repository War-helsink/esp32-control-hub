#include "Pcap.h"
#include "models/wifi/types/frame.h"

#define SNAPLEN 65535
#define PCAP_MAGIC_NUMBER 0xa1b2c3d4
#define LINKTYPE_IEEE802_11 105


unsigned PcapSerializer::pcapSize=0;
uint8_t * PcapSerializer::pcapBuffer=NULL;

bool PcapSerializer::ready() {
    return pcapBuffer != nullptr && pcapSize > sizeof(PcapGlobalHeader);
}

uint8_t *PcapSerializer::init(){
    free(pcapBuffer);
    PcapGlobalHeader pcapGlobalHeader = {
        .magicNumber = PCAP_MAGIC_NUMBER,
        .versionMajor = 2,
        .versionMinor = 4,
        .thiszone = 0,
        .sigfigs = 0,
        .snaplen = SNAPLEN,
        .network = LINKTYPE_IEEE802_11
    };
    pcapBuffer = (uint8_t *)malloc(sizeof(PcapGlobalHeader));
    pcapSize = sizeof(PcapGlobalHeader);
    memcpy(pcapBuffer, &pcapGlobalHeader, sizeof(PcapGlobalHeader));
    return pcapBuffer;
}

void PcapSerializer::clear(){
    free(pcapBuffer);
    pcapBuffer = NULL;
    pcapSize = 0;
}

const unsigned PcapSerializer::getSize(){
    return pcapSize;
}

const uint8_t* PcapSerializer::getBuffer(){
    return pcapBuffer;
}

void PcapSerializer::serializerAddFrame(const uint8_t *buffer, unsigned size, unsigned tsUsec){
    if(size == 0){
        return;
    }
  
    PcapRecordHeader pcapRecordHeader = {
        .tsSec = tsUsec / 1000000,
        .tsUsec = tsUsec % 1000000,
        .inclLen = size,
        .origLen = size,
    };

    if(size > SNAPLEN){
        size = SNAPLEN;
        pcapRecordHeader.inclLen = SNAPLEN;
    }

    uint8_t *reallocatedPcapBuffer = static_cast<uint8_t*>(realloc((void *) pcapBuffer, pcapSize + sizeof(PcapRecordHeader) + size));
    if(reallocatedPcapBuffer == NULL){
        return;
    }
    memcpy(&reallocatedPcapBuffer[pcapSize], &pcapRecordHeader, sizeof(PcapRecordHeader));
    memcpy(&reallocatedPcapBuffer[pcapSize + sizeof(PcapRecordHeader)], buffer, size);
    pcapBuffer = reallocatedPcapBuffer;
    pcapSize += sizeof(PcapRecordHeader) + size;
}