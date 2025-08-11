# ESP32-Based Environmental Monitoring for Precision Agriculture

This project uses an ESP32 microcontroller running FreeRTOS to monitor key environmental factors important for agriculture.

It reads data from multiple sensors simultaneously:
- Temperature and humidity with a DHT11 sensor
- Soil moisture with an analog soil moisture sensor
- Air quality with an MQ135 gas sensor

Each sensor is managed by its own FreeRTOS task, allowing real-time, non-blocking measurements.

The system connects to Wi-Fi and prints sensor data to the Serial Monitor, enabling remote monitoring and potential cloud integration.

---

## Features

- Real-time monitoring of temperature, humidity, soil moisture, and air quality  
- Concurrent multitasking using FreeRTOS on ESP32  
- Wi-Fi connectivity for future IoT expansion  
- Simple serial output for easy debugging and data visualization

---

## Use Case

Ideal for smart farming applications such as greenhouses or remote crop monitoring, this system helps optimize irrigation and environmental conditions to improve crop health and yield.

---

## How It Works

- Each sensor runs in its own FreeRTOS task with independent timing  
- Data is read and printed every few seconds without blocking the main program  
- Wi-Fi connection is established on startup

---

## Getting Started

- Flash the code to an ESP32 board using the Arduino IDE  
- Replace Wi-Fi credentials in the code with your network details  
- Open Serial Monitor at 115200 baud to view sensor data

---

## Future Improvements

- Send data to a cloud service for remote access  
- Add alerting for critical sensor thresholds  
- Integrate automatic irrigation control based on soil moisture  




