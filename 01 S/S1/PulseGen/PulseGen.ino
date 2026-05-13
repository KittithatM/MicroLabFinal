
#include <TM1638plus.h>
#define pin_STB 18
#define pin_CLK 19
#define pin_DIO 21
#define pin_StartSW 0
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
    if ((buttons >> 0) & 1) Data22 += 1000;
    if ((buttons >> 1) & 1) Data22 += 100;
    if ((buttons >> 2) & 1) Data22 += 10;
    if ((buttons >> 3) & 1) Data22 += 1;
    if ((buttons >> 4) & 1) Data23 += 1000;
    if ((buttons >> 5) & 1) Data23 += 100;
    if ((buttons >> 6) & 1) Data23 += 10;
    if ((buttons >> 7) & 1) Data23 += 1;
    display_Data(Data22, Data23);
    delay(250);
  } else {
    if (millis() - startTime >= 1500)
      display_Data(Data22, Data23);
  }
}

void pulseGenerate() {
  while ((Data22 > 0) | (Data23 > 0)) {
    if (Data22 > 0) {
      digitalWrite(pin_pulseOut22, LOW);
      Data22 -= 1;
    }
    if (Data23 > 0) {
      digitalWrite(pin_pulseOut23, LOW);
      Data23 -= 1;
    }
    delay(20);
    digitalWrite(pin_pulseOut22, HIGH);
    digitalWrite(pin_pulseOut23, HIGH);
    display_Data(Data22, Data23);
  }
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