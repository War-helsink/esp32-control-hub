#pragma once
#include "../core/ServerManager.h"

class EventsApi {
public:

  static void begin() {
    ServerManager::events.onConnect([](AsyncEventSourceClient *client){
      client->send("connected", "init", millis());
    });
  }

};