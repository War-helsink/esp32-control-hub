#pragma once

#include "Arduino.h"

enum AppLogLevel {
    APP_LOG_DEBUG,
    APP_LOG_INFO,
    APP_LOG_WARN,
    APP_LOG_ERROR
};

class Logger {
public:
    static void init(unsigned long baudRate = 115200);

    static void log(
        AppLogLevel level,
        const char* tag,
        const char* format,
        ...
    );
};

#define LOG_D(tag, fmt, ...) Logger::log(APP_LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
#define LOG_I(tag, fmt, ...) Logger::log(APP_LOG_INFO, tag, fmt, ##__VA_ARGS__)
#define LOG_W(tag, fmt, ...) Logger::log(APP_LOG_WARN, tag, fmt, ##__VA_ARGS__)
#define LOG_E(tag, fmt, ...) Logger::log(APP_LOG_ERROR, tag, fmt, ##__VA_ARGS__)