#pragma once
#include "./bluetooth/Bluetooth.h"
#include "./wifi/Scanner.h"
#include "./wifi/Attack.h"

class HttpApi {
public:
  static void registerRoutes() {
    HttpApiBluetooth::registerRoutes();
    HttpApiWifiScanner::registerRoutes();
    HttpApiWifiAttack::registerRoutes();
  }
};