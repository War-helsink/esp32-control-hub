#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define RGB_PIN 48 
#define LEDS_COUNT 1

enum class LedStatus {
    Default,   
    Info,        
    Warning,    
    Successful, 
    Error      
};

class LedManager {
private:
    static Adafruit_NeoPixel strip;
    static bool isBlinking;
    static unsigned long lastBlinkTime;
    static uint16_t blinkInterval;
    static bool blinkState;
    static uint32_t currentColor;

    static void setStaticColor(uint8_t r, uint8_t g, uint8_t b) {
        isBlinking = false;
        strip.setPixelColor(0, strip.Color(r, g, b));
        strip.show();
    }

    static void setupBlinking(uint16_t intervalMs, uint8_t r, uint8_t g, uint8_t b) {
        blinkInterval = intervalMs;
        currentColor = strip.Color(r, g, b);
        isBlinking = true;
    }

public:
    static void init() {
        strip.begin();
        strip.setBrightness(30); 
        setStatus(LedStatus::Default);
    }

    static void setStatus(LedStatus status) {
        switch (status) {
            case LedStatus::Default:
                setStaticColor(0, 255, 0); 
                break;

            case LedStatus::Info:
                setupBlinking(400, 0, 0, 255); 
                break;

            case LedStatus::Warning:
                setStaticColor(255, 150, 0); 
                break;

            case LedStatus::Successful:
                setupBlinking(200, 0, 255, 150);
                delay(1000);
                setStaticColor(0, 255, 0); 
                break;

            case LedStatus::Error:
                setupBlinking(100, 255, 0, 0); 
                break;
        }
    }

    static void update() {
        if (isBlinking && (millis() - lastBlinkTime >= blinkInterval)) {
            lastBlinkTime = millis();
            blinkState = !blinkState;
            strip.setPixelColor(0, blinkState ? currentColor : 0);
            strip.show();
        }
    }
};

inline Adafruit_NeoPixel LedManager::strip = Adafruit_NeoPixel(LEDS_COUNT, RGB_PIN, NEO_GRB + NEO_KHZ800);
inline bool LedManager::isBlinking = false;
inline unsigned long LedManager::lastBlinkTime = 0;
inline uint16_t LedManager::blinkInterval = 500;
inline bool LedManager::blinkState = false;
inline uint32_t LedManager::currentColor = 0;

#endif