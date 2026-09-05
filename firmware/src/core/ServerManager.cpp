#include "LittleFS.h"
#include "ServerManager.h"
#include "core/Logger.h"
#include "api/http/Api.h"
#include "api/event/EventsApi.h"

AsyncWebServer ServerManager::server(80);
AsyncEventSource ServerManager::events("/events");

static const char* TAG = "SERVER-MANAGER";

void ServerManager::init() {
  if (!LittleFS.begin(true)) {
    LOG_I(TAG, "LittleFS mount failed");
    return;
  }

  HttpApi::registerRoutes();

  server.addHandler(&events);

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  
  server.onNotFound([](AsyncWebServerRequest *request) {
    String url = request->url();

    if (url.startsWith("/api/") || url.startsWith("/events")) {
      request->send(404, "application/json", R"({"error":"Not found"})");
      return;
    }

    request->send(LittleFS, "/index.html", "text/html");
  });
}

void ServerManager::begin() {
  server.begin();
  EventsApi::begin();
}