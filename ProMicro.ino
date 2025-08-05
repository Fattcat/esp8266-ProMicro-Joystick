#include <ArduinoJson.h>
#include <Mouse.h>

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
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
        int rawX = doc["x"];
        int rawY = doc["y"];
        int btn = doc["btn"];

        // Kalibrácia stredu a citlivosť
        const int centerX = 1895;
        const int centerY = 2005;
        const int deadzone = 130;
        const int maxSpeed = 10;
        const int maxDelta = 1500;

        int dx = 0;
        int dy = 0;

        int deltaX = rawX - centerX;
        int deltaY = rawY - centerY;

        // Otočenie o 90° v smere hodinových ručičiek
        // dx = -dy ; dy = dx

        // Pohyb v X (zvislý joystick → vodorovný pohyb myši)
        if (abs(deltaY) > deadzone) {
          int adjusted = constrain(abs(deltaY) - deadzone, 0, maxDelta);
          dx = map(adjusted, 0, maxDelta, 0, maxSpeed);
          dx *= (deltaY > 0) ? -1 : 1;
        }

        // Pohyb v Y (vodorovný joystick → zvislý pohyb myši)
        if (abs(deltaX) > deadzone) {
          int adjusted = constrain(abs(deltaX) - deadzone, 0, maxDelta);
          dy = map(adjusted, 0, maxDelta, 0, maxSpeed);
          dy *= (deltaX > 0) ? 1 : -1;
        }

        // Pohyb kurzora
        if (dx != 0 || dy != 0) {
          Mouse.move(dx, dy);
        }

        // Tlačidlo
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