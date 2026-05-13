#include <WiFi.h>
#include <WebServer.h>
#include <TM1638plus.h>

// --- Config WiFi ---
const char* ssid = "test";
const char* password = "11111111";

// --- Config TM1638 (STB, CLK, DIO) ---
TM1638plus tm(5, 18, 19);

WebServer server(80);

// ตัวแปรสถานะไฟดวงที่ 1
bool led1State = false;
uint8_t lastButtons = 0;

// --- หน้า Web Dashboard สำหรับ 1 ดวง ---
String getHTML() {
  String statusText = led1State ? "ON" : "OFF";
  String btnClass = led1State ? "btn-on" : "btn-off";

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1' charset='UTF-8'>";
  html += "<style>";
  html += "body { font-family: Arial; text-align: center; background: #f4f4f9; padding-top: 50px; }";
  html += ".btn { width: 150px; height: 150px; border-radius: 50%; border: none; font-size: 24px; cursor: pointer; transition: 0.3s; box-shadow: 0 5px 15px rgba(0,0,0,0.2); }";
  html += ".btn-on { background-color: #2ecc71; color: white; }";
  html += ".btn-off { background-color: #e74c3c; color: white; }";
  html += "h1 { color: #333; }";
  html += "</style></head><body>";
  html += "<h1>LED 1 Control</h1>";
  html += "<button id='ledBtn' class='btn " + btnClass + "' onclick='toggleLED()'>" + statusText + "</button>";
  html += "<script>";
  html += "function toggleLED() {";
  html += "  fetch('/toggle').then(() => location.reload());";
  html += "}";
  html += "</script></body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void handleToggle() {
  led1State = !led1State;
  tm.setLED(0, led1State);  // สั่งไฟดวงที่ 1 (Index 0)
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  tm.displayBegin();
  tm.reset();
  // tm.displayText("STEP 2");

  // เริ่มเชื่อมต่อ WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // แสดงผล IP เมื่อเชื่อมต่อสำเร็จ
  Serial.println("");
  Serial.println("WiFi Connected!");
  Serial.print("Web URL: http://");
  Serial.println(WiFi.localIP());

  // ตั้งค่า Route สำหรับ Server
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
}

void loop() {
  server.handleClient();

  // อ่านปุ่มจาก TM1638
  uint8_t currentButtons = tm.readButtons();

  // เช็คเฉพาะปุ่มที่ 1 (Bit 0)
  bool isPressed = (currentButtons & 0x01);
  bool wasPressed = (lastButtons & 0x01);

  if (isPressed && !wasPressed) {  // ทำงานเมื่อเริ่มกด
    led1State = !led1State;
    tm.setLED(0, led1State);
    Serial.println("Physical Button S1 Pressed - LED1 Toggled");
  }

  lastButtons = currentButtons;
  delay(20);
}