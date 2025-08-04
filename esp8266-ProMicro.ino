#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>

const char* ssid = "HID_WIFI";
const char* password = "12345678";

WiFiUDP udp;
const int localPort = 1234;

SoftwareSerial softSerial(D8, D7); // TX, RX

const int LED_PIN = LED_BUILTIN;

unsigned long lastWiFiCheck = 0;
bool wasConnected = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED vypnutá (aktívna LOW)

  Serial.begin(115200);
  softSerial.begin(9600);

  connectToWiFi();
  udp.begin(localPort);
}

void loop() {
  // === Kontrola WiFi stavu každých 1000 ms ===
  if (millis() - lastWiFiCheck > 1000) {
    lastWiFiCheck = millis();
    if (WiFi.status() != WL_CONNECTED) {
      if (wasConnected) {
        Serial.println("WiFi connection lost. Reconnecting...");
        wasConnected = false;
      }
      blinkWhileConnecting();
      connectToWiFi();
    }
  }

  // === Spracovanie UDP packetov ===
  if (WiFi.status() == WL_CONNECTED) {
    int packetSize = udp.parsePacket();
    if (packetSize) {
      char buffer[128];
      int len = udp.read(buffer, sizeof(buffer) - 1);
      if (len > 0) {
        buffer[len] = '\0';
        softSerial.println(buffer);
        Serial.println(buffer);
      }
    }
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    blinkWhileConnecting();
  }

  Serial.println("\nConnected to ESP32");
  digitalWrite(LED_PIN, LOW);  // LED zapnutá
  wasConnected = true;
}

void blinkWhileConnecting() {
  digitalWrite(LED_PIN, LOW);  // LED ON
  delay(250);
  digitalWrite(LED_PIN, HIGH); // LED OFF
  delay(250);
}
