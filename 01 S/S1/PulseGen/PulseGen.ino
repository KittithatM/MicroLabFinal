
#include <TM1638plus.h>
#define pin_STB 18
#define pin_CLK 19
#define pin_DIO 21
#define pin_StartSW 4
#define pin_stsLED 2
#define pin_pulseOut22 22
#define pin_pulseOut23 23

int sts_LED = 0, Data22, Data23;
unsigned long startTime;
TM1638plus tm(pin_STB, pin_CLK, pin_DIO, false);

void setup() {
  pinMode(pin_StartSW, INPUT_PULLUP);
  pinMode(pin_stsLED, OUTPUT);
  pinMode(pin_pulseOut22, OUTPUT);
  pinMode(pin_pulseOut23, OUTPUT);
  digitalWrite(pin_pulseOut22, HIGH);
  digitalWrite(pin_pulseOut23, HIGH);
  Serial.begin(115200);
  tm.displayBegin();
  tm.brightness(6);
  Data22 = 0;
  Data23 = 0;
  display_Data(Data22, Data23);
}

void loop() {
  if (digitalRead(pin_StartSW) == LOW) pulseGenerate();

  uint8_t buttons = tm.readButtons();
  if (buttons != 0) {
    // ตรวจสอบแยกบิตเพื่อให้กดพร้อมกันได้ หรือลดโอกาสการข้ามบิต
    if (buttons & 0x01) Data22 += 1000;
    if (buttons & 0x02) Data22 += 100;
    if (buttons & 0x04) Data22 += 10;
    if (buttons & 0x08) Data22 += 1;
    if (buttons & 0x10) Data23 += 1000;
    if (buttons & 0x20) Data23 += 100;
    if (buttons & 0x40) Data23 += 10;
    if (buttons & 0x80) Data23 += 1;

    display_Data(Data22, Data23);

    // ใช้ delay ที่สั้นลงเพื่อให้ loop วนกลับมาเช็คปุ่มได้ไวขึ้น
    delay(120);
  } else {
    // อัปเดตหน้าจอเฉพาะเมื่อถึงเวลาที่กำหนด (Non-blocking)
    if (millis() - startTime >= 1500) {
      display_Data(Data22, Data23);
    }
  }
}

void pulseGenerate() {
  // สร้างตัวแปรนับรอบเพื่อไม่ให้ update หน้าจอบ่อยเกินไป
  int refreshCounter = 0;

  while ((Data22 > 0) || (Data23 > 0)) {
    if (Data22 > 0) {
      digitalWrite(pin_pulseOut22, LOW);
      Data22--;
    }
    if (Data23 > 0) {
      digitalWrite(pin_pulseOut23, LOW);
      Data23--;
    }

    delay(10);  // ลดเวลา pulse width ลงเล็กน้อยเพื่อความไว
    digitalWrite(pin_pulseOut22, HIGH);
    digitalWrite(pin_pulseOut23, HIGH);
    delay(10);

    // อัปเดตหน้าจอทุกๆ 10 pulse เพื่อลดภาระ CPU และการกวนสัญญาณ
    if (refreshCounter++ >= 10) {
      display_Data(Data22, Data23);
      refreshCounter = 0;
    }
  }
  display_Data(Data22, Data23);  // แสดงค่าสุดท้ายเมื่อเสร็จสิ้น
}


void display_Data(int temp22, int temp23) {
  digitalWrite(pin_stsLED, sts_LED);
  sts_LED = 1 - sts_LED;
  startTime = millis();

  tm.displayHex(7, temp23 % 10);
  temp23 /= 10;
  tm.displayHex(6, temp23 % 10);
  temp23 /= 10;
  tm.displayHex(5, temp23 % 10);
  temp23 /= 10;
  tm.displayHex(4, temp23 % 10);

  tm.displayHex(3, temp22 % 10);
  temp22 /= 10;
  tm.displayHex(2, temp22 % 10);
  temp22 /= 10;
  tm.displayHex(1, temp22 % 10);
  temp22 /= 10;
  tm.displayHex(0, temp22 % 10);
}