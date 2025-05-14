# ESP32 Blynk-Controlled Smart System

This project controls a motor, LED lights, and reads from LDR and smoke sensors using an ESP32 microcontroller and the Blynk IoT platform.

## 🚀 Features

- Motor control (speed, ON/OFF, direction) via Blynk app
- Automatic dimming LED with smooth fade in/out
- Light-based LED brightness control using LDR sensor
- Smoke sensor value monitoring
- Real-time data sent to the Blynk app

## 🧰 Hardware Required

- ESP32 Dev Board
- DC Motor with Driver (e.g., L298N)
- LDR sensor 3.3 to 5v
- Smoke sensor (e.g., MQ2) 5v
- 2 x LEDs
- Resistors (for LDR and LEDs) 220ohm or more
- External power supply (for motor if needed)
- Jumper wires and breadboard

## 🖥️ Pin Configuration (ESP32 GPIOs)

| Component      | Description             | GPIO |
|----------------|--------------------------|------|
| `motorIn1`     | Motor direction control  | 14   |
| `motorIn2`     | Motor direction control  | 27   |
| `motorEnable`  | Motor speed control (PWM)| 26   |
| `ldrPin`       | LDR analog input         | 34   |
| `ledPin`       | Dimming LED (PWM)        | 25   |
| `led2Pin`      | LED controlled by LDR    | 33   |
| `smokePin`     | Smoke sensor analog input| 35   |

> ⚠️ GPIO 34 & 35 are input-only. They are used for analog reading.

## 📲 Blynk Setup

1. Create a new template in the Blynk IoT platform with the following Virtual Pins:
   - `V0` → Smoke sensor reading
   - `V1` → LDR-based LED brightness
   - `V2` → Motor direction (Switch)
   - `V6` → Motor speed (Slider 0–255)
   - `V7` → Motor ON/OFF (Switch)
   - `V8` → Enable dimming (Switch)

2. Use your Blynk Auth Token and Wi-Fi credentials in the code.

## 🧠 Functionality Overview

- The **motor** is controlled using `motorOn`, `motorSpeed`, and `motorDirection` from the app.
- **Dimming LED** fades in and out when enabled via `V8`.
- **LDR sensor** controls another LED (`led2Pin`) brightness based on ambient light.
- **Smoke sensor** value is sent to the app for monitoring.
- All sensor data is sent every 1 second via a Blynk timer.

## 📦 How to Use

1. Upload the code to your ESP32 using Arduino IDE or PlatformIO.
2. Power up the ESP32 and connect it to your Wi-Fi.
3. Open the Blynk app and start controlling the system.
4. Monitor the smoke and light values in real-time.

## 📎 Notes

- `analogWrite()` is used for PWM, which works differently on ESP32. It may need replacement with `ledcWrite()` for advanced use.
- Use resistors in series with LEDs and sensors as needed.
- Ensure external power is used for the motor if current draw exceeds ESP32 capabilities.

