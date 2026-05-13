#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// --- ตั้งค่า WiFi และ Telegram ---
const char* ssid = "test";
const char* password = "11111111";
#define BOTtoken "8603304181:AAGK2WKJQRX42KeeqS-kJv1gWrSa7V3STvg"
#define CHAT_ID "8505941902"  // ต้องใส่ ID เพื่อให้บอทส่งแจ้งเตือนอัตโนมัติได้

const int ledPins[] = { 2, 4, 5, 18 };
const int switchPins[] = { 12, 13 };

// ตัวแปรสำหรับ Step 3: เก็บสถานะล่าสุดของสวิตช์เพื่อเช็คการเปลี่ยนแปลง
bool lastSwitchState[] = { HIGH, HIGH };
unsigned long lastTimeBotRan;
int botRequestDelay = 1000;  // เช็คข้อความใหม่ทุก 1 วินาที

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String text = bot.messages[i].text;
    String chat_id = bot.messages[i].chat_id;

    // Step 2: ตอบกลับสถานะสวิตช์เมื่อถูกถาม
    if (text == "/status") {
      String statusMsg = "Switch 1: " + String(digitalRead(switchPins[0]) == LOW ? "ON" : "OFF") + "\n";
      statusMsg += "Switch 2: " + String(digitalRead(switchPins[1]) == LOW ? "ON" : "OFF");
      bot.sendMessage(chat_id, statusMsg, "");
    }

    // Step 1: ควบคุม LED 4 ดวง
    if (text == "/led1on") digitalWrite(ledPins[0], HIGH);
    if (text == "/led1off") digitalWrite(ledPins[0], LOW);
    if (text == "/led2on") digitalWrite(ledPins[1], HIGH);
    if (text == "/led2off") digitalWrite(ledPins[1], LOW);
    if (text == "/led3on") digitalWrite(ledPins[2], HIGH);
    if (text == "/led3off") digitalWrite(ledPins[2], LOW);
    if (text == "/led4on") digitalWrite(ledPins[3], HIGH);
    if (text == "/led4off") digitalWrite(ledPins[3], LOW);

    if (text.indexOf("/led") != -1) {
      bot.sendMessage(chat_id, "Updated LED status", "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) pinMode(ledPins[i], OUTPUT);
  for (int i = 0; i < 2; i++) pinMode(switchPins[i], INPUT_PULLUP);

  WiFi.begin(ssid, password);
  client.setInsecure();

  // รอการเชื่อมต่อ WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  // --- ส่วนของ Step 1 & 2: ตรวจสอบคำสั่งจาก Telegram ---
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  // --- ส่วนของ Step 3: แจ้งเตือนเมื่อสวิตช์เปลี่ยนสถานะ ---
  for (int i = 0; i < 2; i++) {
    bool currentState = digitalRead(switchPins[i]);

    // ตรวจสอบว่าสถานะปัจจุบันต่างจากสถานะล่าสุดหรือไม่
    if (currentState != lastSwitchState[i]) {
      String msg = "แจ้งเตือนสวิตช์ตัวที่ " + String(i + 1);
      msg += (currentState == LOW) ? " เปิด (ON)" : " ปิด (OFF)";

      // ส่งข้อความแจ้งเตือน (จะส่งเพียง 1 ครั้งเมื่อมีการเปลี่ยนสถานะ)
      bot.sendMessage(CHAT_ID, msg, "");

      lastSwitchState[i] = currentState;  // อัปเดตสถานะล่าสุด
    }
  }
}