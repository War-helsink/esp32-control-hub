#pragma once
#include "Arduino.h"

struct __attribute__((__packed__)) Hccapx{
  uint32_t signature;
  uint32_t version;
  uint8_t messagePair;
  uint8_t essidLen;
  uint8_t essid[32];
  uint8_t keyver;
  uint8_t keymic[16];
  uint8_t macAp[6];
  uint8_t nonceAp[32];
  uint8_t macSta[6];
  uint8_t nonceSta[32];
  uint16_t eapolLen;
  uint8_t eapol[256];
};