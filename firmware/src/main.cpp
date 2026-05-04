#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

const char* ssid = "Esp32-Control-Hub";
const char* password = "12345678";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("\nBooting ESP32-S3...");

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed");
    return;
  }
  Serial.println("LittleFS OK");

  WiFi.mode(WIFI_AP);

  IPAddress local_ip(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/api/ping", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"ok\":true}");
  });

  server.on("/api/device", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"device\":\"ESP32-S3\",\"status\":\"running\"}");
  });

  server.serveStatic("/", LittleFS, "/")
        .setDefaultFile("index.html");

  server.begin();

  Serial.println("Server started");
}

void loop() {
  
}