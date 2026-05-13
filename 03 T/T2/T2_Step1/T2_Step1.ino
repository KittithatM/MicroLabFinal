#include <TM1638plus.h>

// STB = 5, CLK = 18, DIO = 19
TM1638plus tm(5, 18, 19);

void setup() {
  tm.displayBegin();
  tm.reset();
  
  // แสดงรหัสนักศึกษา 8 ตัวท้าย (รหัส B6709112 -> 06709112)
  tm.displayText("B6709112");
}

void loop() {
  // ไม่มีการทำงานใน Loop สำหรับ Step 1
} 