#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "test";
const char* password = "11111111";
#define BOTtoken "8603304181:AAGK2WKJQRX42KeeqS-kJv1gWrSa7V3STvg"  // ได้จาก BotFather

const int ledPins[] = { 2, 4, 5, 18 };  // ขา GPIO สำหรับ LED 4 ดวง
const int switchPins[] = { 12, 13 };
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String text = bot.messages[i].text;
    String chat_id = bot.messages[i].chat_id;

    if (text == "/status") {
      String statusMsg = "Switch 1: " + String(digitalRead(switchPins[0]) == LOW ? "ON" : "OFF") + "\n";
      statusMsg += "Switch 2: " + String(digitalRead(switchPins[1]) == LOW ? "ON" : "OFF");
      bot.sendMessage(chat_id, statusMsg, "");
    }

    // text = bot.messages[i].text;
    if (text == "/led1on") digitalWrite(ledPins[0], HIGH);
    if (text == "/led1off") digitalWrite(ledPins[0], LOW);
    if (text == "/led2on") digitalWrite(ledPins[1], HIGH);
    if (text == "/led2off") digitalWrite(ledPins[1], LOW);
    if (text == "/led3on") digitalWrite(ledPins[2], HIGH);
    if (text == "/led3off") digitalWrite(ledPins[2], LOW);
    if (text == "/led4on") digitalWrite(ledPins[3], HIGH);
    if (text == "/led4off") digitalWrite(ledPins[3], LOW);
    bot.sendMessage(bot.messages[i].chat_id, "Updated LED status", "");
  }
}

void setup() {
  for (int i = 0; i < 4; i++) pinMode(ledPins[i], OUTPUT);
  for (int i = 0; i < 2; i++) pinMode(switchPins[i], INPUT_PULLUP);
  WiFi.begin(ssid, password);
  client.setInsecure();  // สำหรับ ESP32 ไม่ต้องใช้ใบรับรอง SSL
}

void loop() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  while (numNewMessages) {
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
}