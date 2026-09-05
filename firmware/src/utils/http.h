#pragma once
#include "Arduino.h"
#include "ESPAsyncWebServer.h"

namespace utils {
    void sendStatusResponse(
        AsyncWebServerRequest* request,
        bool success,
        int httpCode
    );
}