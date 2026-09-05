#include "http.h"
#include "ArduinoJson.h"
#include "models/status/Manager.h"

namespace utils {
    void sendStatusResponse(
        AsyncWebServerRequest* request,
        bool success,
        int httpCode
    ) {
        const StatusState* state = StatusManager::getState();

        JsonDocument doc;

        doc["success"] = success;

        if (!success) {
            doc["error"] = "device_busy";
        }

        if (!state->running) {
            doc["status"] = "normal";
            doc["activeProcess"] = nullptr;
        } else {
            doc["status"] = "running";

            JsonObject process = doc["activeProcess"].to<JsonObject>();
            process["module"] = StatusManager::moduleToString(state->activeProcess.module);
            process["action"] = StatusManager::actionToString(state->activeProcess.action);
        }

        String json;
        serializeJson(doc, json);

        request->send(httpCode, "application/json", json);
    }
}