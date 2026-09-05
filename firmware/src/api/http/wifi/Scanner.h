#pragma once

class HttpApiWifiScanner {
public:
	static void registerRoutes();

private:
	static void registerSelect();
	static void registerScannerNetwork();
};