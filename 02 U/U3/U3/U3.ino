#include <Arduino.h>

// Define BCD pins (Weights 1, 2, 4, 8)
const int bcdPins[] = { 18, 19, 21, 22 };  // GPIOs for bit 0, 1, 2, 3
const int ledPins[] = { 13, 12, 14, 27, 26, 25, 33, 32 };

unsigned long lastBlinkTime = 0;
bool blinkState = false;

void setup() {
  Serial.begin(115200);

  // Initialize BCD pins with Pull-up (if Common is connected to GND)
  for (int i = 0; i < 4; i++) {
    pinMode(bcdPins[i], INPUT_PULLUP);
  }

  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

int readBCD() {
  int value = 0;
  for (int i = 0; i < 4; i++) {
    // If using INPUT_PULLUP and Common is GND,
    // a 'LOW' reading means the bit is active.
    if (digitalRead(bcdPins[i]) == LOW) {
      value |= (1 << i);
    }
  }
  return value;
}

void loop() {
  int val = readBCD();

  // Logic from image_ccd2fc.png
  if (val >= 0 && val <= 7) {
    // Steady cumulative LEDs
    for (int i = 0; i < 8; i++) {
      digitalWrite(ledPins[i], (i <= val) ? HIGH : LOW);
    }
  } else {
    // Blink all 8 LEDs at 1 Hz for values 8-15
    if (millis() - lastBlinkTime >= 500) {
      lastBlinkTime = millis();
      blinkState = !blinkState;
      for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], blinkState ? HIGH : LOW);
      }
    }
  }
}