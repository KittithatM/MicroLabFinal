#include <TM1638plus.h>

// Pin mapping
#define pin_STB 18
#define pin_CLK 19
#define pin_DIO 21
#define pin_StartSW 0
#define pin_stsLED 2
#define pin_pulseOut22 22
#define pin_pulseOut23 23

int sts_LED = 0;
int Data22 = 0;
int Data23 = 0;
unsigned long startTime;

// ใช้ true/false ตามโมดูลของคุณ (ถ้าโค้ดแรกใช้ true แล้วเวิร์ค ให้ใช้ true ตามครับ)
TM1638plus tm(pin_STB, pin_CLK, pin_DIO, true);

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

  display_Data(Data22, Data23);
}

void loop() {
  // เช็คปุ่มเริ่มทำงานภายนอก (Start Switch)
  if (digitalRead(pin_StartSW) == LOW) pulseGenerate();

  // --- ส่วนการรับค่าปุ่มตามต้นแบบที่คุณให้มา ---
  uint8_t buttons = tm.readButtons();

  if (buttons != 0) {
    // วนเช็ค 8 บิตเหมือนโค้ดต้นแบบ
    for (int i = 0; i < 8; i++) {
      bool DataBit = (buttons >> i) & 1;

      // แสดงไฟ LED ดวงที่กด (เหมือนโค้ดต้นแบบ)
      tm.setLED(i, DataBit);

      if (DataBit) {
        // แบ่งฝั่งการบวกค่า
        if (i == 0) Data22 += 1000;
        if (i == 1) Data22 += 100;
        if (i == 2) Data22 += 10;
        if (i == 3) Data22 += 1;

        if (i == 4) Data23 += 1000;
        if (i == 5) Data23 += 100;
        if (i == 6) Data23 += 10;
        if (i == 7) Data23 += 1;
      }
    }

    // เคลียร์ค่าเกิน 9999
    if (Data22 > 9999) Data22 %= 10000;
    if (Data23 > 9999) Data23 %= 10000;

    display_Data(Data22, Data23);
    delay(250);  // Delay กันปุ่มเบิ้ลตามโค้ดต้นแบบ
  } else {
    // ดับ LED ทั้งหมดถ้าไม่มีการกด
    for (int i = 0; i < 8; i++) tm.setLED(i, 0);

    if (millis() - startTime >= 1500) {
      display_Data(Data22, Data23);
    }
  }
}

void pulseGenerate() {
  while (Data22 > 0 || Data23 > 0) {
    if (Data22 > 0) {
      digitalWrite(pin_pulseOut22, LOW);
      Data22--;
    }
    if (Data23 > 0) {
      digitalWrite(pin_pulseOut23, LOW);
      Data23--;
    }
    delay(20);
    digitalWrite(pin_pulseOut22, HIGH);
    digitalWrite(pin_pulseOut23, HIGH);
    display_Data(Data22, Data23);
    delay(20);  // เพิ่ม delay เล็กน้อยเพื่อให้มองเห็นตัวเลขลดลงทัน
  }
}

void display_Data(int temp22, int temp23) {
  digitalWrite(pin_stsLED, sts_LED);
  sts_LED = !sts_LED;
  startTime = millis();

  // แสดงผล Data22 (4 หลักซ้าย) และ Data23 (4 หลักขวา)
  // ใช้ loop เพื่อความแม่นยำในการวางตำแหน่ง
  for (int i = 0; i < 4; i++) {
    tm.displayHex(3 - i, temp22 % 10);
    temp22 /= 10;
    tm.displayHex(7 - i, temp23 % 10);
    temp23 /= 10;
  }
}