
// const int clkPin = 13;
// const int dtPin  = 12;

// int ledPins[] = {2,4,5,18,19,21,22,23};

// int counter = 0;
// int lastCLK;

// unsigned long previousMillis = 0;
// bool blinkState = false;

// void setup()
// {
//   pinMode(clkPin, INPUT);
//   pinMode(dtPin, INPUT);

//   for(int i=0;i<8;i++)
//   {
//     pinMode(ledPins[i], OUTPUT);
//   }

//   Serial.begin(115200);

//   lastCLK = digitalRead(clkPin);
// }

// void loop()
// {
//   int currentCLK = digitalRead(clkPin);

//   // ตรวจการหมุน
//   if(currentCLK != lastCLK && currentCLK == 1)
//   {
//     if(digitalRead(dtPin) != currentCLK)
//       counter++;
//     else
//       counter--;

//     Serial.println(counter);
//   }

//   lastCLK = currentCLK;

//   // =========================
//   // แสดง LED
//   // =========================

//   if(counter >= 0 && counter <= 7)
//   {
//     for(int i=0;i<8;i++)
//     {
//       if(i <= counter)
//         digitalWrite(ledPins[i], HIGH);
//       else
//         digitalWrite(ledPins[i], LOW);
//     }
//   }
//   else
//   {
//     // กระพริบ 1Hz
//     unsigned long currentMillis = millis();

//     if(currentMillis - previousMillis >= 500)
//     {
//       previousMillis = currentMillis;

//       blinkState = !blinkState;

//       for(int i=0;i<8;i++)
//       {
//         digitalWrite(ledPins[i], blinkState);
//       }
//     }
//   }
// }

const int clkPin = 13, dtPin = 12;
const int ledPins[] = {2,4,5,18,19,21,22,23};

int counter = 0, lastCLK;
bool blinkState = false;

void setup() {
  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);
  for (int i = 0; i < 8; i++) pinMode(ledPins[i], OUTPUT);
  Serial.begin(115200);
  lastCLK = digitalRead(clkPin);
}

void setLEDs(bool state) {
  for (int i = 0; i < 8; i++) digitalWrite(ledPins[i], state);
}

void loop() {
  int currentCLK = digitalRead(clkPin);

  if (currentCLK != lastCLK && currentCLK == 1) {
    counter += (digitalRead(dtPin) != currentCLK) ? 1 : -1;
    Serial.println(counter);
  }
  lastCLK = currentCLK;

  if (counter >= 0 && counter <= 7) {
    for (int i = 0; i < 8; i++)
      digitalWrite(ledPins[i], i <= counter);
  } else {
    setLEDs(blinkState = !blinkState);
    delay(500);
  }
}