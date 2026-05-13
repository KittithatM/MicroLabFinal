#include <TM1638plus.h>

#define CANCEL_PIN 0  // ปุ่ม BOOT บนบอร์ด ESP32
const int PRICE = 275;

TM1638plus tm(5, 18, 19, true);

long balance = 0;
uint8_t lastButtons = 0;
int values[] = { 1000, 500, 100, 50, 20, 10, 5, 1 };

void displayRight(long num) {
  char buf[9];
  sprintf(buf, "%8ld", num);
  tm.displayText(buf);
}

// ฟังก์ชันทอนเงิน/คืนเงิน พร้อมไฟ LED แจ้งเตือน
void processRefund(long amount) {
  if (amount <= 0) {
    balance = 0;
    displayRight(0);
    return;
  }

  char displayStr[9] = "00000000";
  long temp = amount;

  tm.reset();  // ล้างไฟ LED ทั้งหมดก่อนเริ่มทอน

  for (int i = 0; i < 8; i++) {
    int count = temp / values[i];
    temp %= values[i];

    if (count > 0) {
      if (count > 9) count = 9;  // แสดงได้สูงสุดเลข 9 ต่อหลัก
      displayStr[i] = count + '0';
      tm.setLED(i, 1);  // <--- เปิดไฟ LED ในหลักที่มีเงินทอนออกมา
    } else {
      displayStr[i] = '0';
      tm.setLED(i, 0);
    }
  }

  tm.displayText(displayStr);  // แสดงจำนวนใบแยกหลัก
  delay(10000);                 // ค้างไว้ 5 วินาทีให้ดูยอด

  tm.reset();  // ดับไฟและล้างหน้าจอ
  balance = 0;
  displayRight(0);
}

void setup() {
  pinMode(CANCEL_PIN, INPUT_PULLUP);
  tm.displayBegin();
  tm.reset();
  displayRight(0);
}

void loop() {
  // 1. ตรวจสอบปุ่ม Cancel
  if (digitalRead(CANCEL_PIN) == LOW && balance > 0) {
    processRefund(balance);  // คืนเงินเต็มจำนวน
    return;
  }

  // 2. การหยอดเงิน
  uint8_t buttons = tm.readButtons();
  for (int i = 0; i < 8; i++) {
    bool isPressed = (buttons >> i) & 1;
    bool wasPressed = (lastButtons >> i) & 1;

    if (isPressed && !wasPressed) {
      balance += values[i];
      displayRight(balance);
      delay(50);
    }
  }
  lastButtons = buttons;

  // 3. เงื่อนไขการซื้ออัตโนมัติ
  if (balance >= PRICE) {
    delay(1500);  // รอเผื่อกดยกเลิกหรือหยอดเพิ่ม
    if (balance >= PRICE) {
      processRefund(balance - PRICE);  // ทอนเงิน (ยอดเงิน - 275)
    }
  }

  delay(30);
}