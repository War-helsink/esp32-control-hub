#include "ServerManager.h"
#include <LittleFS.h>

AsyncWebServer ServerManager::server(80);
AsyncEventSource ServerManager::events("/events");

void ServerManager::init() {

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed");
    return;
  }

  server.addHandler(&events);

  server.serveStatic("/", LittleFS, "/")
        .setDefaultFile("index.html");
}

void ServerManager::begin() {
  server.begin();
}