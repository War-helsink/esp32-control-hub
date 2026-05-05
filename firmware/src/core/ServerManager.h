#pragma once
#include <ESPAsyncWebServer.h>

class ServerManager {
public:
  static AsyncWebServer server;
  static AsyncEventSource events;

  static void init();
  static void begin();
};