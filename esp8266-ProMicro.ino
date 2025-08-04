#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "RemoteControl";
const char* password = "RemoteControlCursor";
const char* host = "192.168.4.1";

void setup() {
  Serial.begin(9600); // Komunikácia s Pro Micro
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://" + String(host) + "/");
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload); // Posielame Pro Micro
    }

    http.end();
  }

  delay(50); // Rýchlosť aktualizácie
}
