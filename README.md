# ESP32 LDR Light Tracking Robot

A light-seeking mobile robot that autonomously moves toward the brightest light source using four LDR sensors.  
The system also provides **real-time telemetry visualization** through a web dashboard powered by an ESP32.

The project combines **embedded control, I2C communication, and web-based monitoring** to observe sensor voltages live while the robot operates.

---

## System Overview

The system consists of two microcontrollers:

### Arduino (Robot Controller)
- Reads **4 LDR sensors** positioned around the robot.
- Determines the **brightest direction**.
- Controls **four DC motors** to move toward the light source.
- Sends sensor voltage readings to the ESP32 using **I2C**.

### ESP32 (Telemetry Server)
- Requests sensor voltages from Arduino via **I2C**.
- Hosts a **WiFi web server**.
- Displays **live voltage plots** for each LDR using **Chart.js**.

---

## Features

- Autonomous **light tracking robot**
- **4-direction light sensing**
- **Omnidirectional movement logic**
- **Real-time web telemetry dashboard**
- **I2C communication between controllers**
- **Live voltage plotting**
- Lightweight browser interface

---

## Hardware Components

- ESP32
- Arduino Uno
- L293D Motor Shield (Adafruit Motor Shield)
- 4 × DC Motors
- 4 × LDR Sensors
- Voltage divider resistors
- Li-Ion battery pack
- Robot chassis

---

## Software Stack

- Arduino IDE
- ESP32 WiFi library
- Wire (I2C) communication
- Chart.js (web visualization)
- Adafruit Motor Shield Library

---

## System Architecture
LDR Sensors
↓
Arduino (Sensor Processing + Motor Control)
↓ I2C
ESP32 (Telemetry Server)
↓ WiFi
Web Dashboard (Chart.js Visualization)


---

## Light Tracking Algorithm

1. Read voltages from four LDR sensors:
   - Front
   - Back
   - Left
   - Right

2. Determine the **maximum light intensity**.

3. Compare with threshold:

- If below threshold → Stop robot
- If front highest → Move forward
- If back highest → Move backward
- If left highest → Strafe left
- If right highest → Strafe right

4. Handle **diagonal light sources** using combined motion.

---

## Telemetry Dashboard

The ESP32 hosts a webpage displaying:

- Front LDR voltage
- Back LDR voltage
- Left LDR voltage
- Right LDR voltage

Each signal is plotted **live vs time** using Chart.js.

Data refresh rate:

500 ms

---

## How to Run

### 1. Upload Arduino Code
Upload the robot controller code to the **Arduino Uno**.

### 2. Upload ESP32 Code
Upload the telemetry server code to the **ESP32**.

### 3. Connect Hardware

I2C Connection:


ESP32 SDA → Arduino SDA
ESP32 SCL → Arduino SCL
GND → GND


### 4. Connect WiFi

Edit in ESP32 code:
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
### 5. Open Dashboard

After ESP32 connects to WiFi, open:

http://ESP32_IP_ADDRESS

in your browser.

Example Output
   The dashboard shows real-time voltage curves for all LDR sensors while the robot moves toward light sources.
---
## Possible Improvements
   Add PID control for smoother motion
   
   Add camera-based light detection
   
   Replace LDR with photodiodes
   
   Add ROS2 integration
   
   Log telemetry to CSV or cloud
