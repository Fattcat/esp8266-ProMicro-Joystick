#include <Mouse.h>
#include <ArduinoJson.h>

const int ledPin = 7;
bool wifiConnected = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial1.begin(9600); // Prijíma od ESP8266
  Mouse.begin();
}

void loop() {
  if (Serial1.available()) {
    String json = Serial1.readStringUntil('\n');

    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, json);

    if (!error) {
      int x = doc["x"];
      int y = doc["y"];
      int btn = doc["btn"];

      // Prestavba na delta
      int dx = map(x, 0, 1023, -10, 10);
      int dy = map(y, 0, 1023, -10, 10);

      // Pohnutie kurzora
      Mouse.move(dx, dy, 0);

      // Tlačidlo
      if (btn == 1) {
        Mouse.press(MOUSE_LEFT);
      } else {
        Mouse.release(MOUSE_LEFT);
      }

      // Signalizácia pripojenia
      if (!wifiConnected) {
        digitalWrite(ledPin, HIGH);
        wifiConnected = true;
      }
    } else {
      digitalWrite(ledPin, LOW);
      wifiConnected = false;
    }
  }
}
