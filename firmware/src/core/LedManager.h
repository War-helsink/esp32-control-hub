#pragma once

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

enum class LedStatus {
    Default,
    Info,
    Warning,
    Successful,
    Error
};

class LedManager {
private:
    static void setStaticColor(uint8_t r, uint8_t g, uint8_t b);
    static void setupBlinking(uint16_t intervalMs, uint8_t r, uint8_t g, uint8_t b);

private:
    static Adafruit_NeoPixel strip;
    static bool isBlinking;
    static unsigned long lastBlinkTime;
    static uint16_t blinkInterval;
    static bool blinkState;
    static uint32_t currentColor;

public:
    static void init();
    static void setStatus(LedStatus status);
    static void update();
};