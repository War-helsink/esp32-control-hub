#pragma once
#include "../core/ServerManager.h"

class EventsApi {
public:

  static void begin() {

    ServerManager::events.onConnect([](AsyncEventSourceClient *client){
      client->send("connected", "init", millis());
    });

  }

  static void pushMessage(const String& msg) {
    ServerManager::events.send(msg.c_str(), "message", millis());
  }

};