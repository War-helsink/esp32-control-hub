#pragma once

class HttpApiBluetooth {
public:
  static void registerRoutes();
private:
  static void registerJammerRoutes();
  static void registerScannerRoutes();
};