#include <ArduinoJson.h>
#include <Mouse.h>

void setup() {
  Serial.begin(9600);      // Debug cez USB
  Serial1.begin(9600);     // UART z D1 mini (na pin 0)
  Mouse.begin();
}

void loop() {
  static String input = "";

  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') {
      StaticJsonDocument<128> doc;
      DeserializationError err = deserializeJson(doc, input);
      if (!err) {
        int x = doc["x"];
        int y = doc["y"];
        int btn = doc["btn"];

        int dx = map(x, 0, 4095, -10, 10);
        int dy = map(y, 0, 4095, -10, 10);
        Mouse.move(dx, dy);

        if (btn == 1) Mouse.press(MOUSE_LEFT);
        else          Mouse.release(MOUSE_LEFT);
      } else {
        Serial.println("JSON parse error: " + input);
      }
      input = "";
    } else {
      input += c;
    }
  }
}
