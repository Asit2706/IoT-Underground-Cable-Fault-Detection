#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIFI_SSID     "YOUR_HOTSPOT_NAME"
#define WIFI_PASSWORD "YOUR_HOTSPOT_PASSWORD"
#define DATABASE_URL  "https://cable-fault-detector-64c34-default-rtdb.asia-southeast1.firebasedatabase.app"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NODE_A 34
#define NODE_B 35
#define NODE_C 32
#define NODE_D 33

#define BASELINE_A 2926
#define BASELINE_B 1889
#define BASELINE_C 860
#define DROP_MARGIN 300

unsigned long lastSend = 0;
#define SEND_INTERVAL 3000
bool faultLogged = false;  // ← NEW FLAG

int getFaultLocation(int a, int b, int c) {
  if (a < (BASELINE_A - DROP_MARGIN)) return 1;
  if (b < (BASELINE_B - DROP_MARGIN)) return 2;
  if (c < (BASELINE_C - DROP_MARGIN)) return 3;
  return 0;
}

void sendToFirebase(int faultKm, int a, int b, int c, int d) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = String(DATABASE_URL) + "/current.json";
  String payload = "{\"fault_km\":" + String(faultKm) +
                   ",\"status\":\"" + (faultKm == 0 ? "OK" : "FAULT") + "\"" +
                   ",\"node_a\":" + String(a) +
                   ",\"node_b\":" + String(b) +
                   ",\"node_c\":" + String(c) +
                   ",\"node_d\":" + String(d) + "}";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  Serial.println("Firebase HTTP: " + String(code));
  http.end();

  // Log ONLY ONCE per fault event
  if (faultKm > 0 && !faultLogged) {
    String histUrl = String(DATABASE_URL) + "/history/" + String(millis()/1000) + ".json";
    http.begin(histUrl);
    http.addHeader("Content-Type", "application/json");
    http.PUT(payload);
    http.end();
    faultLogged = true;
    Serial.println("Fault logged once — waiting for repair.");
  }

  // Reset flag when fault is cleared
  if (faultKm == 0) {
    faultLogged = false;
    Serial.println("Fault cleared — ready for next event.");
  }
}

void showOnOLED(int km, bool wifiOk) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("WiFi:");
  display.println(wifiOk ? "OK" : "NO");
  display.drawLine(0, 9, 128, 9, WHITE);
  if (km == 0) {
    display.setTextSize(1);
    display.setCursor(0, 14);
    display.println("Status: OK");
    display.setTextSize(2);
    display.setCursor(0, 30);
    display.println("No Fault");
  } else {
    display.setTextSize(1);
    display.setCursor(0, 14);
    display.println("!! FAULT DETECTED !!");
    display.println("Location:");
    display.setTextSize(2);
    display.setCursor(0, 38);
    display.print(km);
    display.print(" KM");
  }
  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(NODE_A, INPUT);
  pinMode(NODE_B, INPUT);
  pinMode(NODE_C, INPUT);
  pinMode(NODE_D, INPUT);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  display.clearDisplay();
  display.setCursor(0, 0);
  if (WiFi.status() == WL_CONNECTED) {
    display.println("WiFi Connected!");
    display.println(WiFi.localIP().toString());
    Serial.println("WiFi OK");
  } else {
    display.println("WiFi FAILED");
    Serial.println("WiFi FAILED");
  }
  display.display();
  delay(1500);
}

void loop() {
  int a = analogRead(NODE_A);
  int b = analogRead(NODE_B);
  int c = analogRead(NODE_C);
  int d = analogRead(NODE_D);
  Serial.printf("A:%d B:%d C:%d D:%d\n", a, b, c, d);
  int faultKm = getFaultLocation(a, b, c);
  bool wifiOk = (WiFi.status() == WL_CONNECTED);
  showOnOLED(faultKm, wifiOk);
  if (wifiOk && millis() - lastSend > SEND_INTERVAL) {
    lastSend = millis();
    sendToFirebase(faultKm, a, b, c, d);
  }
  delay(1000);
}
