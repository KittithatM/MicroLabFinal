#include <TM1638plus.h>

TM1638plus tm(5, 18, 19, true);

long totalMoney = 0;
uint8_t lastButtons = 0;
// S1(bit0)=1000 ... S8(bit7)=1
int values[] = { 1000, 500, 100, 50, 20, 10, 5, 1 };

void displayRight(long num) {
  char buf[9];
  sprintf(buf, "%8ld", num);  // %8ld คือจอง 8 หลัก ชิดขวา
  tm.displayText(buf);
}

void setup() {
  Serial.begin(115200);
  tm.displayBegin();
  tm.reset();
  displayRight(0);
}

void loop() {
  uint8_t buttons = tm.readButtons();

  for (int i = 0; i < 8; i++) {
    bool isPressed = (buttons >> i) & 1;
    bool wasPressed = (lastButtons >> i) & 1;

    if (isPressed && !wasPressed) {
      totalMoney += values[i];
      displayRight(totalMoney);  // แสดงผลชิดขวา
    }
  }

  lastButtons = buttons;
  delay(50);
}