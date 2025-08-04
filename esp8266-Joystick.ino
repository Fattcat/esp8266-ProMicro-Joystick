#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "RemoteControl";
const char* password = "RemoteControlCursor";

ESP8266WebServer server(80);

// Joystick piny
const int joyX = A0;
const int joyY = A1;
const int joyBtn = D2;

void handleRoot() {
  int x = analogRead(joyX);
  int y = analogRead(joyY);
  int button = digitalRead(joyBtn) == LOW ? 1 : 0;

  String json = "{\"x\":" + String(x) + ",\"y\":" + String(y) + ",\"btn\":" + String(button) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  pinMode(joyBtn, INPUT_PULLUP);
  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
