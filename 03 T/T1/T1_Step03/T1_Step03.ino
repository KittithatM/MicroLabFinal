#include <WiFi.h>
#include <WebServer.h>
#include <TM1638plus.h>

// --- ตั้งค่า WiFi ---
const char* ssid = "test";
const char* password = "11111111";

// --- ตั้งค่า TM1638 (STB, CLK, DIO) ---
TM1638plus tm(5, 18, 19);

WebServer server(80);

// ตัวแปรเก็บสถานะไฟ 8 ดวง
bool ledStates[8] = { false, false, false, false, false, false, false, false };
uint8_t lastButtons = 0;

// --- หน้า Dashboard HTML ---
String getHTML() {
  String html = "<!DOCTYPE html><html lang='en'><head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Smart Home Dashboard</title>";
  html += "<style>";
  html += "body { font-family: sans-serif; background: #f0f2f5; display: flex; flex-direction: column; align-items: center; padding: 20px; }";
  html += "h1 { color: #1c1e21; }";
  html += ".grid { display: grid; grid-template-columns: repeat(4, 1fr); gap: 15px; max-width: 500px; }";
  html += ".btn { width: 80px; height: 80px; border: none; border-radius: 12px; font-weight: bold; cursor: pointer; transition: 0.3s; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }";
  html += ".on { background: #42b72a; color: white; }";
  html += ".off { background: #ebedf0; color: #4b4f56; }";
  html += "@media (max-width: 400px) { .grid { grid-template-columns: repeat(2, 1fr); } }";
  html += "</style></head><body>";
  html += "<h1>🏠 Smart Light</h1>";
  html += "<div class='grid'>";

  for (int i = 0; i < 8; i++) {
    String stateClass = ledStates[i] ? "on" : "off";
    String label = ledStates[i] ? "ON" : "OFF";
    html += "<button id='btn" + String(i) + "' class='btn " + stateClass + "' onclick='toggleLED(" + String(i) + ")'>";
    html += "L" + String(i + 1) + "<br>" + label + "</button>";
  }

  html += "</div>";
  html += "<script>";
  html += "function toggleLED(id) {";
  html += "  fetch('/set?id=' + id).then(response => response.text()).then(data => {";
  html += "    location.reload();";  // โหลดใหม่เพื่ออัปเดตสีปุ่ม
  html += "  });";
  html += "}";
  html += "</script></body></html>";
  return html;
}

// --- ฟังก์ชันจัดการคำสั่งจากเว็บ ---
void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void handleSetLED() {
  if (server.hasArg("id")) {
    int id = server.arg("id").toInt();
    if (id >= 0 && id < 8) {
      ledStates[id] = !ledStates[id];
      tm.setLED(id, ledStates[id]);
      server.send(200, "text/plain", "OK");
      return;
    }
  }
  server.send(400, "text/plain", "Invalid ID");
}

void setup() {
  Serial.begin(115200);

  // เริ่มต้น TM1638
  tm.displayBegin();
  tm.reset();
  // tm.displayText("HELLO");  // แสดงคำทักทายบนหน้าปัด

  // เชื่อมต่อ WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("Web Dashboard IP: http://");
  Serial.println(WiFi.localIP());

  // ตั้งค่า Web Server
  server.on("/", handleRoot);
  server.on("/set", handleSetLED);
  server.begin();
}

void loop() {
  server.handleClient();

  // อ่านค่าปุ่มจากบอร์ด TM1638 (ครอบคลุม Step 1 และ 3)
  uint8_t currentButtons = tm.readButtons();

  for (int i = 0; i < 8; i++) {
    bool isPressed = (currentButtons & (1 << i));
    bool wasPressed = (lastButtons & (1 << i));

    if (isPressed && !wasPressed) {  // ตรวจจับการกด
      ledStates[i] = !ledStates[i];
      tm.setLED(i, ledStates[i]);
      Serial.printf("Physical Button S%d Pressed - LED %d is %s\n", i + 1, i + 1, ledStates[i] ? "ON" : "OFF");
    }
  }
  lastButtons = currentButtons;
  delay(20);
}