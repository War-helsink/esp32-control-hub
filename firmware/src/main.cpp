#include "Arduino.h"

#include "core/WiFiManager.h"
#include "core/ServerManager.h"
#include "core/Config.h"
#include "core/Logger.h"
#include "core/LedManager.h"

static const char* TAG = "MAIN";

void setup() {

  // Init
  Logger::init();
  LedManager::setStatus(LedStatus::Warning);

  WiFiManager::initAP();
  ServerManager::init();

  // Begin
  ServerManager::begin();

  delay(1500);
  LedManager::setStatus(LedStatus::Default);

  LOG_I(TAG, "Server started at 192.168.1.1");
}

void loop() {
  LedManager::update(); 
  delay(10);
}