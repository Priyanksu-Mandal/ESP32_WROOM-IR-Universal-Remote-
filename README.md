This project implements a smart universal remote system capable of controlling legacy infrared (IR) appliances—such as televisions, air conditioners, and set-top boxes—via voice commands. Built on the ESP32-WROOM-DA microcontroller, the device utilizes an IR transmitter and receiver circuit to capture and replicate remote signals using various protocols (NEC, Samsung, PulseDistance).

The system integrates with Sinric Pro to bridge the ESP32 with Amazon Alexa and Google Home ecosystems. This architecture allows users to issue natural voice commands (e.g., "Alexa, turn on the AC" or "Hey Google, volume up on TV") to trigger specific IR codes wirelessly. By leveraging cloud connectivity and standard IFTTT-style logic, this project creates a cost-effective, retrofittable smart home hub that unifies control of "dumb" devices without requiring expensive proprietary hubs.

Key Components: ESP32, IR LED/Receiver, Sinric Pro Cloud, Amazon Alexa / Google Assistant.
