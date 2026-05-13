
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Keypad.h>

MD_Parola p = MD_Parola(MD_MAX72XX::FC16_HW, 23, 18, 5, 1);
//                                            D   CLK CS
// ==========================
const byte ro = 4, co = 4;
char keys[ro][co] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { 'E', '0', 'F', 'D' }
};
//8 7 6 5 
byte rPin[ro] = { 13, 12, 14, 27 };
//4 3 2 1
byte cPin[co] = { 26, 25, 33, 32 };
// ==========================
Keypad keypad = Keypad(makeKeymap(keys), rPin, cPin, ro, co);
// ==========================
void show(char c) {
  char text[2] = { c , '\0' };
  p.displayClear();
  p.displayText(text, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  p.displayAnimate();
}
// ==========================
void setup(void) {
  p.begin();
  p.setIntensity(5);
  show('0');
}

void loop(void) {
  char k = keypad.getKey();
  if (k) show(k);
}
// ==========================