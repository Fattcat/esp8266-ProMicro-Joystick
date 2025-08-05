#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

const char* ssid = "HID_WIFI";
const char* password = "12345678";

WiFiUDP udp;
const char* d1_ip = "192.168.4.2";
const int udpPort = 1234;

// Joystick piny
#define JOY_X 34
#define JOY_Y 35
#define JOY_BTN 32

void setup() {
  Serial.begin(115200);
  pinMode(JOY_BTN, INPUT_PULLUP);

  WiFi.softAP(ssid, password);
  delay(500);
  Serial.println("ESP32 WiFi HID AP started");
}

void loop() {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);
  int btn = digitalRead(JOY_BTN) == LOW ? 1 : 0;

  StaticJsonDocument<128> doc;
  doc["x"] = x;
  doc["y"] = y;
  doc["btn"] = btn;

  char buffer[128];
  size_t len = serializeJson(doc, buffer);

  udp.beginPacket(d1_ip, udpPort);
  udp.write((uint8_t*)buffer, len);
  udp.endPacket();

  delay(30);
}
