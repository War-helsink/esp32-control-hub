#include "Logger.h"
#include "stdarg.h"
#include "stdio.h"

void Logger::init(unsigned long baudRate) {
    if (!Serial) {
        Serial.begin(baudRate);
    }
}

void Logger::log(
    AppLogLevel level,
    const char* tag,
    const char* format,
    ...
) {
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