#include <TM1638plus.h>

// ขาเชื่อมต่อ: STB, CLK, DIO
TM1638plus tm(5, 18, 19);

bool led1Status = false;
bool lastButtonState = false;

void setup() {
  tm.displayBegin();
  tm.reset();
}

void loop() {
  // readButtons() จะคืนค่าเป็น byte (ปุ่ม 1 คือ bit 0)
  uint8_t buttons = tm.readButtons();
  bool currentButtonState = (buttons & 0x01); 

  if (currentButtonState && !lastButtonState) {
    led1Status = !led1Status;
    tm.setLED(0, led1Status); // LED ดวงที่ 1 (index 0)
  }
  
  lastButtonState = currentButtonState;
  delay(50); 
}