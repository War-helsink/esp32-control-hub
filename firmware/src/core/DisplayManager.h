#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class DisplayManager {
public:
    static bool init();
    static void showBoot();
    static void update();
    static void clear();

private:
    static Adafruit_SSD1306 _display;

    static uint8_t _dots;
    static unsigned long _lastUpdate;

    static void drawBoot();
};