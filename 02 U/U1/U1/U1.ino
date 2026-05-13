
#include <Keypad.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define DIN 2
#define CS 4
#define CLK 5

#define HW_TYPE MD_MAX72XX::FC16_HW

MD_MAX72XX mx = MD_MAX72XX(HW_TYPE, DIN, CLK, CS, 1);

char keys[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

uint8_t numm[11][8] = {
  { 0x00, 0x3C, 0x42, 0x46, 0x4A, 0x52, 0x62, 0x3C },  // 0
  { 0x00, 0x18, 0x38, 0x38, 0x18, 0x18, 0x18, 0x3C },  // 1
  { 0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7C },  // 2
  { 0x00, 0x3C, 0x02, 0x02, 0x0C, 0x02, 0x02, 0x3C },  // 3
  { 0x00, 0x0C, 0x14, 0x24, 0x3E, 0x04, 0x04, 0x04 },  // 4
  { 0x00, 0x7E, 0x40, 0x40, 0x7C, 0x02, 0x02, 0x7C },  // 5
  { 0x00, 0x1C, 0x22, 0x20, 0x3C, 0x22, 0x22, 0x1C },  // 6
  { 0x00, 0x3E, 0x02, 0x04, 0x08, 0x08, 0x08, 0x08 },  // 7
  { 0x00, 0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x1C },  // 8
  { 0x00, 0x1C, 0x22, 0x22, 0x1E, 0x02, 0x22, 0x1C },  // 9
  { 0x00, 0x1C, 0x22, 0x22, 0x22, 0x3E, 0x22, 0x22 }   // A
};

byte rowPin[4] = { 13, 12, 14, 27 };
byte colPin[4] = { 26, 25, 33, 32 };

Keypad keypad = Keypad(makeKeymap(keys), rowPin, colPin, 4, 4);

void setup() {
  Serial.begin(115200);
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 2);
  mx.clear();
}

void displayMetrix(int index) {
  mx.clear();
  for (int i = 0; i < 8; i++) {
    mx.setColumn(0, 7 - i, numm[index][i]);
  }
}

void loop() {
  displayMetrix(0);
  delay(1000);
  displayMetrix(1);
  delay(1000);
  displayMetrix(2);
  delay(1000);
  displayMetrix(3);
  delay(1000);
  displayMetrix(4);
  delay(1000);
  displayMetrix(5);
  delay(1000);
  displayMetrix(6);
  delay(1000);
  displayMetrix(7);
  delay(1000);
  displayMetrix(8);
  delay(1000);
  displayMetrix(9);
  delay(1000);
  displayMetrix(10);
  delay(1000);
}
