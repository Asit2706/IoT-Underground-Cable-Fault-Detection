# ⚡ IoT-Based Underground Cable Fault Detection System

![ESP32](https://img.shields.io/badge/ESP32-IoT-blue?style=for-the-badge&logo=espressif)
![Firebase](https://img.shields.io/badge/Firebase-Realtime_DB-orange?style=for-the-badge&logo=firebase)
![Arduino](https://img.shields.io/badge/Arduino-C++-teal?style=for-the-badge&logo=arduino)
![HTML](https://img.shields.io/badge/Dashboard-HTML%2FCSS%2FJS-red?style=for-the-badge&logo=html5)

> A real-time IoT system that automatically detects and locates faults in underground cables using voltage drop analysis, ESP32 microcontroller, Firebase cloud, and a live web dashboard.

---

## 📌 Project Overview

Underground cable faults are difficult to detect manually and cause long power outages and expensive repairs. This system automatically monitors cable health, detects faults instantly, pinpoints the exact fault location in KM, and sends real-time alerts through an OLED display and cloud dashboard.

This project demonstrates the **Murray Loop Test / Voltage Divider Principle** using affordable IoT components — the same concept used by companies like **Siemens, ABB, and Megger** in industrial fault detection systems.

---

## 🎯 Features

- ✅ **Real-time fault detection** — detects open circuit faults in under 2 seconds
- ✅ **Precise fault location** — identifies fault at 1 KM, 2 KM, or 3 KM
- ✅ **OLED local display** — shows fault location and WiFi status instantly
- ✅ **Firebase cloud logging** — data sent every 3 seconds via WiFi
- ✅ **Live web dashboard** — charts, alerts, history, analytics, sidebar navigation
- ✅ **Multi-alert system** — sound + popup + flashing red background on fault
- ✅ **Smart fault logging** — fault logged only once per event, resets on repair
- ✅ **Works on any device** — dashboard accessible from phone, tablet, or laptop

---

## 🔧 Components Used

| Component | Purpose |
|---|---|
| ESP32 Dev Module | IoT microcontroller, WiFi, ADC reading |
| OLED SSD1306 (128×64) | Local fault display |
| 4 × Resistors (1kΩ) | Simulate 1 km cable segments |
| Mobile Hotspot / WiFi | Internet connectivity |
| Firebase Realtime DB | Cloud data storage |
| Web Browser | Dashboard access |

---

## 🖥️ Tech Stack

```
Hardware   →  ESP32 + OLED SSD1306 + Resistor Chain
Firmware   →  C++ (Arduino Framework)
Cloud      →  Google Firebase Realtime Database
Dashboard  →  HTML + CSS + JavaScript + Chart.js
Protocol   →  HTTP / WiFi
```

---

## 📐 Circuit Diagram

```
3.3V ── R1 ──┬── R2 ──┬── R3 ──┬── R4 ── GND
             │        │        │
           GPIO34   GPIO35   GPIO32
          (Node A) (Node B) (Node C)

OLED: VCC→3.3V | GND→GND | SDA→GPIO21 | SCL→GPIO22
```

> See `circuit_diagram.svg` for full circuit diagram

---

## 📊 Voltage Thresholds (Calibrated)

| Node | GPIO | Healthy Baseline | Fault Threshold | Fault Distance |
|---|---|---|---|---|
| A | GPIO 34 | ~2926 ADC | < 2626 ADC | **1 KM** |
| B | GPIO 35 | ~1889 ADC | < 1589 ADC | **2 KM** |
| C | GPIO 32 | ~860 ADC | < 560 ADC | **3 KM** |
| D | GPIO 33 | ~0 ADC | — | Reference |

---

## 🚀 How to Run This Project

### Step 1 — Hardware Setup
```
3.3V → R1 → [GPIO34] → R2 → [GPIO35] → R3 → [GPIO32] → R4 → [GPIO33] → GND
OLED SDA → GPIO21
OLED SCL → GPIO22
```

### Step 2 — Install Arduino Libraries
```
1. Adafruit SSD1306
2. Adafruit GFX
```

### Step 3 — Configure ESP32 Code
Open `cable_fault.ino` and update:
```cpp
#define WIFI_SSID     "YOUR_HOTSPOT_NAME"
#define WIFI_PASSWORD "YOUR_HOTSPOT_PASSWORD"
#define DATABASE_URL  "YOUR_FIREBASE_URL"
```

### Step 4 — Firebase Setup
```
1. Create project at console.firebase.google.com
2. Enable Realtime Database
3. Set rules to: { "rules": { ".read": true, ".write": true } }
4. Copy database URL to code
```

### Step 5 — Flash ESP32
```
1. Open cable_fault.ino in Arduino IDE
2. Select ESP32 Dev Module board
3. Upload code
4. OLED shows "WiFi Connected" on success
```

### Step 6 — Open Dashboard
```
1. Open dashboard.html in Chrome
2. Click anywhere to enable sound alerts
3. Live data appears automatically
```

---

## 🌐 Web Dashboard Pages

| Page | Description |
|---|---|
| 🏠 Dashboard | Overview — status cards, live chart, node readings |
| 📡 Live Readings | Big ADC values with progress bars per node |
| 📋 Fault History | Complete log of all fault events |
| 📈 Analytics | Bar chart showing faults per segment |
| ℹ️ About | Project specs and GPIO pin mapping |

---

## 📱 How Fault Detection Works

```
Step 1 → ESP32 reads voltage at Node A, B, C, D every second
Step 2 → Compares each reading against calibrated baseline
Step 3 → If voltage drops below threshold → fault detected
Step 4 → OLED shows "FAULT DETECTED — X KM"
Step 5 → Data sent to Firebase via WiFi
Step 6 → Dashboard triggers sound + popup + flashing alert
Step 7 → Fault logged once in history
Step 8 → When resistor reconnected → system resets to "No Fault"
```

---

## 🏭 Real-World Applications

- ⚡ Power distribution networks (BESCOM, MSEDCL, TATA Power)
- 🏙️ Smart city underground cable monitoring
- 🚇 Railways and Metro signaling cables
- 🏭 Industrial plants and factories
- 🏥 Hospitals and data centers
- 📡 Telecom underground networks

---

## 🔮 Future Scope

- TDR (Time Domain Reflectometry) for single-endpoint detection
- GPS-based fault location on Google Maps
- SMS / WhatsApp alerts via Twilio API
- Machine Learning for predictive fault detection
- LoRaWAN for rural long-range coverage
- Current sensor (INA219) for fault type detection

---

## 📁 Repository Structure

```
📁 IoT-Underground-Cable-Fault-Detection/
  ├── 📄 README.md              ← Project documentation
  ├── 📄 cable_fault.ino        ← ESP32 Arduino code
  ├── 📄 dashboard.html         ← Web dashboard
  ├── 📄 circuit_diagram.svg    ← Circuit diagram
  ├── 📄 flowchart.svg          ← System flowchart
  └── 📁 images/
        ├── oled_nofault.jpg    ← OLED No Fault display
        ├── oled_fault.jpg      ← OLED Fault display
        ├── dashboard.jpg       ← Dashboard screenshot
        └── model.jpg           ← Physical model
```

---

## 🎓 Project Info

- **Internship:** IoT and Data Analytics for Smart System Design
- **Institute:** National Institute of Technology (NIT) Rourkela
- **Duration:** 8th June – 7th July 2026
- **Convener:** Prof. Suchismita Chinara, Associate Professor, CSE, NIT Rourkela

---

## 👨‍💻 Author

**Asit Kumar Pradhan**
- LinkedIn: [linkedin.com/in/asit-kumar-pradhan-106886343](https://www.linkedin.com/in/asit-kumar-pradhan-106886343)
- GitHub: [github.com/Asit2706](https://github.com/yourusername)

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

---

⭐ **If you found this project helpful, please give it a star!** ⭐
