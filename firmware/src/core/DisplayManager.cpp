#include "DisplayManager.h"

#include <Wire.h>

#include "assets/PacketBatLogo.h"
#include "assets/StatusIcons.h"

#include "models/status/Manager.h"

static constexpr uint8_t SCREEN_WIDTH = 128;
static constexpr uint8_t SCREEN_HEIGHT = 64;

static constexpr uint8_t OLED_ADDRESS = 0x3C;

static constexpr uint8_t SDA_PIN = 8;
static constexpr uint8_t SCL_PIN = 9;

static constexpr unsigned long ANIMATION_INTERVAL = 400;

Adafruit_SSD1306 DisplayManager::_display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

uint8_t DisplayManager::_dots = 0;
unsigned long DisplayManager::_lastUpdate = 0;


bool DisplayManager::init() {
    Wire.begin(SDA_PIN, SCL_PIN);

    if (!_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        return false;
    }

    _display.clearDisplay();
    _display.setTextColor(SSD1306_WHITE);

    showBoot();

    return true;
}


void DisplayManager::showBoot() {
    _dots = 0;
    _lastUpdate = millis();

    drawBoot();
}


void DisplayManager::update() {
    if (millis() - _lastUpdate < ANIMATION_INTERVAL) {
        return;
    }

    _lastUpdate = millis();

    _dots++;

    if (_dots > 3) {
        _dots = 0;
    }

    drawBoot();
}


void DisplayManager::drawBoot() {
    _display.clearDisplay();

    // =================================================
    // Yellow status area: y = 0..15
    // =================================================

    const StatusState* state = StatusManager::getState();

    if (
        state->running &&
        state->activeProcess.module != StatusModule::None
    ) {
        constexpr int16_t iconX = 112;
        constexpr int16_t iconY = 0;

        const uint8_t* icon = nullptr;

        switch (state->activeProcess.module) {
            case StatusModule::Wifi:
                icon = wifiIcon;
                break;

            case StatusModule::Bluetooth:
                icon = bluetoothIcon;
                break;

            default:
                break;
        }

        if (icon != nullptr) {
            // Animated dots before icon

            _display.setTextSize(1);

            constexpr int16_t dotsX = 88;
            constexpr int16_t dotsY = 3;

            _display.setCursor(dotsX, dotsY);

            for (uint8_t i = 0; i < _dots; i++) {
                _display.print(".");
            }

            // Status icon

            _display.drawBitmap(
                iconX,
                iconY,
                icon,
                STATUS_ICON_WIDTH,
                STATUS_ICON_HEIGHT,
                SSD1306_WHITE
            );
        }
    }

    // =================================================
    // Blue area
    // =================================================

    constexpr int16_t logoWidth = 48;
    constexpr int16_t logoHeight = 24;

    const int16_t logoX =
        (SCREEN_WIDTH - logoWidth) / 2;

    constexpr int16_t logoY = 17;

    _display.drawBitmap(
        logoX,
        logoY,
        packetBatLogo,
        logoWidth,
        logoHeight,
        SSD1306_WHITE
    );

    // =================================================
    // PACKET BAT
    // =================================================

    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);

    const char* title = "PACKET BAT";

    int16_t x1;
    int16_t y1;
    uint16_t textWidth;
    uint16_t textHeight;

    _display.getTextBounds(
        title,
        0,
        0,
        &x1,
        &y1,
        &textWidth,
        &textHeight
    );

    _display.setCursor(
        (SCREEN_WIDTH - textWidth) / 2,
        44
    );

    _display.print(title);

    // =================================================
    // Boot animation
    // =================================================

    constexpr int16_t dotsY = 55;
    constexpr int16_t dotsWidth = 18;

    _display.setCursor(
        (SCREEN_WIDTH - dotsWidth) / 2,
        dotsY
    );

    for (uint8_t i = 0; i < _dots; i++) {
        _display.print(".");
    }

    _display.display();
}


void DisplayManager::clear() {
    _display.clearDisplay();
    _display.display();
}