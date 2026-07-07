#pragma once
#include <Arduino.h>

enum AppLogLevel {
    APP_LOG_DEBUG,
    APP_LOG_INFO,
    APP_LOG_WARN,
    APP_LOG_ERROR
};

class Logger {
public:
    static void init(unsigned long baudRate = 115200) {
        if (!Serial) {
            Serial.begin(baudRate);
        }
    }

    static void log(AppLogLevel level, const char* tag, const char* format, ...) {
        char buffer[256];
        
        const char* levelStr = "INFO";
        switch (level) {
            case APP_LOG_DEBUG: levelStr = "DEBUG"; break;
            case APP_LOG_INFO:  levelStr = "INFO";  break;
            case APP_LOG_WARN:  levelStr = "WARN";  break;
            case APP_LOG_ERROR: levelStr = "ERROR"; break;
        }

        int prefixLen = snprintf(buffer, sizeof(buffer), "[%s] [%s] (%lu ms) ", tag, levelStr, millis());
        
        va_list args;
        va_start(args, format);
        vsnprintf(buffer + prefixLen, sizeof(buffer) - prefixLen, format, args);
        va_end(args);

        Serial.println(buffer);
    }
};


#define LOG_D(tag, fmt, ...) Logger::log(APP_LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
#define LOG_I(tag, fmt, ...) Logger::log(APP_LOG_INFO, tag, fmt, ##__VA_ARGS__)
#define LOG_W(tag, fmt, ...) Logger::log(APP_LOG_WARN, tag, fmt, ##__VA_ARGS__)
#define LOG_E(tag, fmt, ...) Logger::log(APP_LOG_ERROR, tag, fmt, ##__VA_ARGS__)