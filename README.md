

# 🎙️ Voice-Activated Universal IR Hub (ESP32 + Sinric Pro)

> **"Alexa, turn on the AC to 24 degrees."** ❄️

### 📝 Project Overview

This project transforms a standard **ESP32-WROOM-DA** into a powerful, cloud-connected Universal Remote. By integrating **Sinric Pro**, it bridges the gap between legacy infrared appliances (TVs 📺, Air Conditioners ❄️, Sound Systems 🔊) and modern smart home ecosystems like **Amazon Alexa** and **Google Home**.

Unlike standard "dumb" remotes, this system allows for **voice control**, **remote triggering** (from anywhere in the world), and **automation routines** without requiring expensive proprietary hubs.

---

### ✨ Key Features

* **🗣️ Voice Control:** Native integration with Alexa & Google Assistant via Sinric Pro.
* **📡 Universal Cloning:** Capable of learning and replicating generic (NEC, Samsung, Sony) and complex (PulseDistance/AC) IR protocols.
* **🔋 High-Power Range:** Custom MOSFET driver circuit to boost IR signal range across large rooms.
* **📶 Dual Antenna Reliability:** Utilizes the ESP32-WROOM-DA for superior Wi-Fi connectivity in dead zones.
* **🔄 Status Feedback:** Real-time device state synchronization with the mobile app.

---

### 🛠️ Hardware Architecture & The "Power Driver"

The core of this project is not just the code, but the robust hardware design ensuring the signal actually reaches the device.

#### 1. The Brain: ESP32-WROOM-DA 🧠

We utilize the **DA (Dual Antenna)** version of the ESP32. This automatically switches between two PCB antennas to ensure the strongest Wi-Fi connection, which is critical for a device hidden behind a TV or mounted on a wall.

#### 2. The Muscle: High-Current IR MOSFET Driver 🔌

A standard GPIO pin on a microcontroller can only source about **12mA to 40mA**. However, to achieve a reliable remote control range (5–10 meters), high-intensity IR LEDs require current pulses of **100mA+**.

To solve this, I designed a **Low-Side Switching Circuit** using a **MOSFET** (Metal-Oxide-Semiconductor Field-Effect Transistor).

* **The Problem:** Connecting the IR LED directly to the ESP32 would result in a weak signal (1-2 meters range) and could damage the microcontroller.
* **The Solution:** I use an N-Channel MOSFET (e.g., 2N7000 or similar) as a high-speed electronic switch.
* **Gate (G):** Connected to the ESP32 GPIO (PWM signal).
* **Drain (D):** Connected to the IR LED cathode (ground side).
* **Source (S):** Connected to Common Ground.


* **The Result:** When the ESP32 sends a weak 3.3V signal to the Gate, the MOSFET "opens" the floodgates, allowing high current from the 5V rail to flow directly through the IR LED. This creates a powerful burst of infrared light, ensuring the signal bounces off walls and reaches devices even without direct line-of-sight. ⚡

#### 3. The Learner: IR Receiver (VS1838B) 👁️

A dedicated 38kHz IR demodulator allows the system to "listen" to existing remotes. This enables the **Learning Mode**, where the ESP32 captures raw timing arrays (Raw Data) from unknown remotes (like Air Conditioners) and saves them to flash memory for replay.

---

### ☁️ Software & IoT Workflow

1. **Command:** User says *"Alexa, turn on the TV"* 🗣️.
2. **Cloud:** Alexa sends the request to the **Sinric Pro** cloud server ☁️.
3. **WebSocket:** Sinric Pro forwards a payload via WebSocket to the **ESP32** 📶.
4. **Processing:** The ESP32 parses the JSON payload to find the device ID (e.g., "Samsung TV").
5. **Execution:** The ESP32 activates the **IRremote** library to generate the specific hex code (e.g., `0xE0E040BF`).
6. **Firing:** The GPIO triggers the **MOSFET**, blasting the IR code into the room 💥.

---

### 🧰 Tech Stack

* **Microcontroller:** Espressif ESP32-WROOM-DA
* **IoT Platform:** Sinric Pro (Free Tier)
* **Libraries:** `IRremote.hpp` (Encoding/Decoding), `SinricPro.h` (Connectivity), `ArduinoWebSockets`
* **Components:** IR LED (940nm), N-Channel MOSFET, VS1838B Receiver, Resistors (current limiting).

---

### 🚀 Future Improvements

* [ ] Add DHT11 sensor for automatic AC temperature triggers.
* [ ] Create a custom PCB layout for the MOSFET driver.
* [ ] Offline local control via Web Server.

---

*Made with ❤️ and soldering irons.*
