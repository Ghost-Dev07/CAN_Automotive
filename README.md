![Language](https://img.shields.io/badge/Language-Embedded%20C-blue)
![Protocol](https://img.shields.io/badge/Protocol-CAN-green)
![MCU](https://img.shields.io/badge/MCU-PIC18F4580-orange)
![Domain](https://img.shields.io/badge/Domain-Automotive-purple)
![Architecture](https://img.shields.io/badge/System-Multi--ECU-red)

# 🚗 CAN-Based Automotive ECU System

## 🚀 Overview

A distributed embedded system simulating automotive Electronic Control Units (ECUs) communicating over the CAN protocol to exchange real-time vehicle data such as speed, RPM, gear position, and indicator status.

---

## 🧠 Problem Statement

Modern vehicles rely on multiple ECUs communicating over a shared network for real-time monitoring and control. Reliable inter-ECU communication is essential for synchronizing vehicle parameters efficiently.

---

## 💡 Solution

This project simulates a CAN-based automotive network where:

* Multiple PIC microcontrollers function as independent ECUs
* CAN protocol enables real-time communication between nodes
* Vehicle parameters are acquired, transmitted, processed, and displayed centrally

---

## 🏗️ System Architecture

The system consists of three ECU nodes:

* ECU 1 → Speed and gear acquisition
* ECU 2 → RPM and indicator acquisition
* Main ECU → Data processing and display

```text id="vbz27f"
        +----------------------+
        | ECU 1                |
        | Speed + Gear         |
        +----------+-----------+
                   |
                   | CAN Bus
                   v
        +----------------------+
        | Main ECU             |
        | Display + Processing |
        +----------+-----------+
                   ^
                   | CAN Bus
                   |
        +----------+-----------+
        | ECU 2                |
        | RPM + Indicators     |
        +----------------------+
```

---

## 🔧 Hardware Used

* PIC18F4580 Microcontrollers (3 boards)
* CAN Transceiver
* CLCD Display
* Potentiometers (Speed & RPM simulation)
* Digital Keypad

---

## 💻 Software Concepts Used

* Embedded C
* CAN Protocol
* ADC (Analog-to-Digital Conversion)
* Inter-Microcontroller Communication
* Real-Time Data Processing

---

## 🔍 Feature to Implementation Mapping

| Feature                 | Implementation           |
| ----------------------- | ------------------------ |
| Multi-node architecture | Multiple PIC MCUs        |
| Inter-ECU communication | CAN Protocol             |
| Speed/RPM acquisition   | ADC using potentiometers |
| User input handling     | Digital keypad           |
| Centralized display     | CLCD interface           |

---

## 🔄 Working

### 🚗 ECU 1

* Reads vehicle speed using ADC
* Reads gear position through keypad input
* Transmits processed data over CAN bus

### 🚗 ECU 2

* Reads RPM values using ADC
* Detects indicator status through keypad input
* Sends data over CAN bus

### 🧠 Main ECU

* Receives messages from both ECUs
* Processes incoming CAN frames
* Displays:

  * Speed
  * RPM
  * Gear Position
  * Indicator Status on CLCD

---

## ⚡ Key Features

* Distributed multi-ECU architecture
* CAN-based real-time communication
* Distributed data acquisition
* Centralized monitoring and display
* Real-time parameter synchronization

---

## 🎥 Demonstration Video

* [View CAN System Demo](https://drive.google.com/file/d/1DBVDsLQW1zQE0up9Ed-VPp6PlpFPhNlS/view?usp=sharing)

---

## 📊 Results

* Successfully established communication between multiple ECUs
* Achieved synchronized real-time parameter transmission
* Accurately displayed vehicle data using CAN communication

---

## ⚠️ Engineering Challenges

* CAN frame synchronization between nodes
* Managing communication across multiple ECUs
* Debugging inter-node data transmission
* Handling real-time data updates reliably

---

## 🔮 Future Improvements

* Fault detection and diagnostics
* Integration with real automotive sensors
* Expansion to additional ECU nodes
* CAN message prioritization
