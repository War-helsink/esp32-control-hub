#pragma once
#include "Arduino.h"

struct PcapGlobalHeader {
  uint32_t magicNumber;   
  uint16_t versionMajor;  
  uint16_t versionMinor;  
  int32_t  thiszone;      
  uint32_t sigfigs;       
  uint32_t snaplen; 
  uint32_t network;    
};

struct PcapRecordHeader{
  uint32_t tsSec;         
  uint32_t tsUsec;       
  uint32_t inclLen;      
  uint32_t origLen;  
};