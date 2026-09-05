#include "LedManager.h"

namespace {
    constexpr uint8_t RGB_PIN = 48;
    constexpr uint16_t LEDS_COUNT = 1;
    constexpr uint8_t LED_BRIGHTNESS = 30;
    constexpr uint32_t SUCCESS_DURATION_MS = 1000;
}

Adafruit_NeoPixel LedManager::strip(
    LEDS_COUNT,
    RGB_PIN,
    NEO_GRB + NEO_KHZ800
);

bool LedManager::isBlinking = false;
unsigned long LedManager::lastBlinkTime = 0;
uint16_t LedManager::blinkInterval = 500;
bool LedManager::blinkState = false;
uint32_t LedManager::currentColor = 0;

void LedManager::setStaticColor(
    uint8_t r,
    uint8_t g,
    uint8_t b
) {
    isBlinking = false;
    blinkState = true;

    strip.setPixelColor(
        0,
        strip.Color(r, g, b)
    );

    strip.show();
}

void LedManager::setupBlinking(
    uint16_t intervalMs,
    uint8_t r,
    uint8_t g,
    uint8_t b
) {
    blinkInterval = intervalMs;
    currentColor = strip.Color(r, g, b);

    isBlinking = true;
    blinkState = true;
    lastBlinkTime = millis();

    strip.setPixelColor(0, currentColor);
    strip.show();
}

void LedManager::init() {
    strip.begin();
    strip.clear();
    strip.setBrightness(LED_BRIGHTNESS);
    strip.show();

    setStatus(LedStatus::Default);
}

void LedManager::setStatus(LedStatus status) {
    switch (status) {
        case LedStatus::Default:
            setStaticColor(
                0,
                255,
                0
            );
            break;

        case LedStatus::Info:
            setupBlinking(
                400,
                0,
                0,
                255
            );
            break;

        case LedStatus::Warning:
            setStaticColor(
                255,
                150,
                0
            );
            break;

        case LedStatus::Successful:
            setupBlinking(
                200,
                0,
                255,
                150
            );

            delay(SUCCESS_DURATION_MS);

            setStaticColor(
                0,
                255,
                0
            );
            break;

        case LedStatus::Error:
            setupBlinking(
                100,
                255,
                0,
                0
            );
            break;
    }
}

void LedManager::update() {
    if (!isBlinking) {
        return;
    }

    const unsigned long currentTime = millis();

    if (currentTime - lastBlinkTime < blinkInterval) {
        return;
    }

    lastBlinkTime = currentTime;
    blinkState = !blinkState;

    strip.setPixelColor(
        0,
        blinkState ? currentColor : 0
    );

    strip.show();
}