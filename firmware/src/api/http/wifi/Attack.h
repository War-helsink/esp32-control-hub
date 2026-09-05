#pragma once

class HttpApiWifiAttack {
public:
  static void registerRoutes();

private:
  static void registerDeauthRoutes();
  static void registerBeaconRoutes();
  static void registerHandshakeRoutes();
};