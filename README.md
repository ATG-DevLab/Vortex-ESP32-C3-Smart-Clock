# 🕒 Vortex ESP32-C3 Smart Clock

### An IoT-Based Smart Clock with OLED Display, RTC, Touch Controls, and Environmental Monitoring

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--C3-orange?style=for-the-badge&logo=espressif" alt="ESP32-C3">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-blue?style=for-the-badge&logo=cplusplus" alt="C++">
  <img src="https://img.shields.io/badge/Framework-Arduino-00979D?style=for-the-badge&logo=arduino" alt="Arduino">
  <img src="https://img.shields.io/badge/Project-IoT-green?style=for-the-badge" alt="IoT">
</p>

**Vortex ESP32-C3 Smart Clock** is a compact embedded systems project built around the ESP32-C3 Super Mini. It combines a digital clock, OLED display, DS3231 real-time clock module, capacitive touch controls, Wi-Fi connectivity, and temperature and humidity monitoring.

The project demonstrates the practical integration of microcontrollers, sensors, I²C communication, wireless connectivity, and an interactive display using C++ and the Arduino framework.

## 📑 Table of Contents

- [Project Overview](#-project-overview)
- [Key Features](#-key-features)
- [Hardware Components](#-hardware-components)
- [Circuit Connections](#-circuit-connections)
- [Touch Controls](#-touch-controls)
- [Display Modes](#-display-modes)
- [Wi-Fi and Time Synchronization](#-wi-fi-and-time-synchronization)
- [Software Requirements](#-software-requirements)
- [Installation and Setup](#-installation-and-setup)
- [Applications](#-applications)
- [Future Improvements](#-future-improvements)
- [Troubleshooting](#-troubleshooting)
- [Contributing](#-contributing)
- [Author](#-author)
- [YouTube Channel](#-youtube-channel)

---

## 🚀 Project Overview

Vortex ESP32-C3 Smart Clock is designed to combine timekeeping, environmental monitoring, and touch-based interaction in one compact electronic device.

The ESP32-C3 communicates with an SSD1306 OLED display and DS3231 RTC through the I²C bus. A DHT11 sensor measures ambient temperature and relative humidity, while a TTP223 capacitive touch sensor provides user input.

The firmware also supports a Wi-Fi access point and network time synchronization when a working network route to an NTP server is available.

### Project Specifications

| Parameter | Specification |
|---|---|
| Project Name | Vortex ESP32-C3 Smart Clock |
| Alternate Name | ATG Smart Clock-ESP32-C3 |
| Microcontroller | ESP32-C3 Super Mini |
| Architecture | 32-bit RISC-V |
| Programming Language | C++ |
| Development Environment | Arduino IDE |
| Display | 0.96-inch SSD1306 OLED |
| Display Resolution | 128 × 64 pixels |
| Display Interface | I²C |
| OLED I²C Address | `0x3C` |
| Real-Time Clock | DS3231 |
| Temperature and Humidity Sensor | DHT11 |
| Touch Sensor | TTP223 |
| Wireless Connectivity | Wi-Fi Access Point |
| Time Zone | IST (UTC+05:30) |

## ✨ Key Features

- 🕒 **Digital Clock:** Displays the current time and date.
- 🖥️ **OLED Interface:** Uses a 128 × 64 pixel SSD1306 display.
- ⏱️ **RTC Timekeeping:** Uses the DS3231 module for timekeeping when backup power is available.
- 🌐 **NTP Synchronization Support:** Supports network-based time synchronization.
- 📶 **Wi-Fi Access Point:** Creates a wireless network named `ATG Smart Clock`.
- 📡 **Wi-Fi Status Indicator:** Shows an indicator when a device is connected to the access point.
- 🕛 **12-Hour and 24-Hour Formats:** Supports two time display formats.
- 👆 **Touch Gestures:** Provides short-tap, long-press, and triple-tap controls.
- 🌡️ **Temperature Monitoring:** Measures ambient temperature in degrees Celsius.
- 💧 **Humidity Monitoring:** Measures relative humidity as a percentage.
- 🔧 **Expandable Design:** Provides a foundation for additional hardware and firmware features.

## 🔩 Hardware Components

| No. | Component | Function |
|---:|---|---|
| 1 | ESP32-C3 Super Mini | Main microcontroller |
| 2 | 0.96-inch SSD1306 OLED | Displays time and sensor information |
| 3 | DS3231 RTC Module | Real-time clock |
| 4 | DHT11 Sensor | Temperature and humidity monitoring |
| 5 | TTP223 Touch Sensor | Touch-based user input |
| 6 | Jumper Wires | Circuit connections |
| 7 | USB Cable and Compatible Power Supply | Programming and power |

## 🔌 Circuit Connections

### 1. OLED Display and DS3231 RTC

Both modules share the same I²C bus.

| ESP32-C3 Pin | Peripheral Connection | Signal |
|---|---|---|
| GPIO 5 | OLED SDA + RTC SDA | I²C SDA |
| GPIO 6 | OLED SCL + RTC SCL | I²C SCL |

**OLED configuration:** SSD1306, 128 × 64 pixels, I²C address `0x3C`.

### 2. DHT11 Sensor

| DHT11 Pin | ESP32-C3 Connection |
|---|---|
| VCC | Compatible supply voltage |
| DATA | GPIO 4 |
| GND | Common GND |

### 3. TTP223 Touch Sensor

| TTP223 Pin | ESP32-C3 Connection |
|---|---|
| VCC | Compatible supply voltage |
| OUT / SIG | GPIO 3 |
| GND | Common GND |

### 4. Power Supply

All modules must use compatible supply voltages and a common ground.

**Electrical safety:** ESP32-C3 GPIO pins are not 5 V tolerant. Verify the supply voltage, logic levels, and I²C pull-up voltages of all connected modules before powering the circuit.

## 👆 Touch Controls

The TTP223 capacitive touch sensor provides a button-free interface.

| Gesture | Action |
|---|---|
| Short Tap | Displays temperature and humidity temporarily for approximately 5 seconds |
| Long Press (2 seconds or longer) | Toggles between permanent clock and weather views |
| Three Rapid Taps | Switches between 12-hour and 24-hour time formats |

The gestures are implemented using the TTP223 digital output. Triple-tap recognition can be further improved to prevent individual taps from triggering actions before the full gesture is recognized.

## 🖥️ Display Modes

### 1. Clock Mode

Displays the current time and date.

**12-hour format**
- Hours and minutes
- AM/PM indicator
- Date

**24-hour format**
- Hours, minutes, and seconds
- Date

### 2. Permanent Weather Mode

Displays local environmental readings from the DHT11 sensor:

- Temperature in °C
- Relative humidity in %

A sensor error message is displayed if a reading fails.

*This mode displays local sensor measurements, not an internet-based weather forecast.*

### 3. Temporary Weather Mode

A short tap from clock mode opens the temperature and humidity display for approximately five seconds before returning to the clock.

### 4. Wi-Fi Status Indicator

A custom pixel-based Wi-Fi indicator appears at the bottom-right of the OLED when one or more clients are connected to the ESP32-C3 access point.

## 📶 Wi-Fi and Time Synchronization

The firmware uses the following default configuration:

| Setting | Value |
|---|---|
| Wi-Fi SSID | `ATG Smart Clock` |
| Default Password | `12345678` |
| NTP Server | `pool.ntp.org` |
| UTC Offset | `+05:30` |
| Offset in Seconds | `19800` |
| Daylight-Saving Offset | `0` |

### How It Works

1. The ESP32-C3 starts its Wi-Fi access point.
2. A compatible device can connect to the access point.
3. The firmware detects connected clients.
4. Network time synchronization can be attempted when a working route to the NTP server is available.
5. When synchronization succeeds, the RTC can be adjusted using the received time.

**Important:** Starting a Wi-Fi access point does not automatically provide internet access to the ESP32-C3. NTP synchronization requires a working network route to the configured NTP server or access to a suitable local NTP server.

If the RTC reports lost power, the current firmware initializes it using the sketch compilation timestamp. This fallback may not represent the actual current time.

For safer deployment, replace the default Wi-Fi password and consider making the network configuration adjustable.

## 💻 Software Requirements

### Development Environment

- Arduino IDE
- ESP32 board support package
- USB data cable
- Serial Monitor for debugging

### Required Libraries

| Library | Purpose |
|---|---|
| Adafruit GFX Library | Graphics and text rendering |
| Adafruit SSD1306 | OLED display control |
| RTClib by Adafruit | RTC communication |
| DHT sensor library by Adafruit | Temperature and humidity readings |

The following headers are provided by the ESP32 Arduino core or standard development environment:

- `Wire.h`
- `WiFi.h`
- `time.h`

## ⚙️ Installation and Setup

1. Install Arduino IDE and ESP32 board support.
2. Install the required libraries listed above.
3. Assemble the circuit according to the pin configuration.
4. Open the project firmware in the Arduino IDE.
5. Select the appropriate ESP32-C3 board and serial port.
6. Compile and upload the firmware.
7. Open Serial Monitor at **115200 baud** to check initialization and diagnostic messages.

Verify OLED initialization, RTC operation, DHT11 readings, touch response, Wi-Fi startup, and NTP synchronization where network connectivity is available.

## 🎯 Applications

- Digital desk and bedside clocks
- Embedded systems learning
- ESP32-C3 development and prototyping
- I²C peripheral integration
- Temperature and humidity monitoring
- IoT connectivity experiments
- Electronics and Communication Engineering projects
- DIY smart-device development

## 🔮 Future Improvements

Potential enhancements include:

- More reliable NTP connectivity checks
- Improved triple-tap detection and touch debouncing
- Configurable Wi-Fi credentials
- Web-based configuration interface
- Manual time setting and RTC diagnostics
- Improved sensor error recovery
- OLED brightness control and power-saving modes
- Custom PCB and enclosure
- Optional online weather forecast integration
- Circuit schematics and PCB design files

## 🛠️ Troubleshooting

| Problem | Possible Solution |
|---|---|
| OLED is blank | Check I²C wiring, address `0x3C`, and display initialization |
| RTC time is incorrect | Check RTC initialization, backup power, and time synchronization |
| NTP synchronization fails | Verify that a working network route to the NTP server exists |
| DHT11 shows an error | Check GPIO 4, sensor pinout, and supply voltage |
| Touch controls do not respond | Check GPIO 3 and the TTP223 output configuration |
| Wi-Fi indicator does not appear | Verify that a device is connected to the access point |

## 🤝 Contributing

Contributions, bug reports, documentation improvements, firmware optimizations, and hardware modifications are welcome.

1. Fork the repository.
2. Create a branch for your changes.
3. Test your modifications.
4. Submit a pull request with a clear description.

Suggestions and bug reports can also be shared through GitHub Issues.

## 📜 License

This project is licensed under the **MIT License**.

You are free to use, copy, modify, merge, publish, distribute, sublicense, and sell copies of this software, subject to the conditions of the MIT License.

See the [`LICENSE`](LICENSE) file in this repository for the complete license terms.

## 👨‍💻 Author

**Ashish Prajapati**

Electronics enthusiast focused on embedded systems, microcontrollers, IoT, and practical electronics projects.

**Project:** Vortex ESP32-C3 Smart Clock

**Technology:** ESP32-C3 · C++ · Arduino · OLED · RTC · IoT

## ▶️ YouTube Channel

### Ashish Tech Guruji

Explore electronics tutorials, DIY hardware projects, embedded systems, technical experiments, and practical electronics demonstrations on my YouTube channel.

**[Subscribe to Ashish Tech Guruji on YouTube](https://www.youtube.com/@AshishTechGuruji)**

Project demonstration videos will be available on the channel.

## ⭐ Support the Project

If you find this project useful:

- ⭐ Star the repository
- 🍴 Fork the project
- 💡 Suggest improvements
- 🛠️ Experiment with the hardware and firmware
- 📢 Share it with the electronics community

**Vortex ESP32-C3 Smart Clock — Smart Timekeeping, Powered by Embedded Innovation.**
