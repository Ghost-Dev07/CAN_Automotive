![Language](https://img.shields.io/badge/Language-Embedded%20C-blue)
![Protocol](https://img.shields.io/badge/Protocol-CAN-green)
![MCU](https://img.shields.io/badge/MCU-PIC18F4580-orange)
![Domain](https://img.shields.io/badge/Domain-Automotive-purple)


# 🚗 CAN-Based Automotive ECU System

## 🚀 Overview

A distributed embedded system simulating automotive Electronic Control Units (ECUs) communicating over the CAN protocol to exchange real-time vehicle data.

---

## 🧠 Problem Statement

Modern vehicles rely on multiple ECUs communicating over a network. Efficient and reliable communication is required for real-time monitoring of parameters like speed, RPM, and gear.

---

## 💡 Solution

This project simulates a multi-ECU automotive system where:

* Multiple PIC microcontrollers act as ECUs
* CAN protocol is used for communication
* Real-time vehicle parameters are transmitted and displayed

---

## 🏗️ System Architecture

ECU 1 (Speed + Gear)
↓ CAN
Main ECU (Display + Processing)
↑ CAN
ECU 2 (RPM + Indicators)

---

## 🔧 Hardware Used

* PIC18F4580 Microcontrollers (3 boards)
* CAN Transceiver
* CLCD Display
* Potentiometers (Speed & RPM simulation)
* Keypad (Gear & Indicators)

---

## 💻 Software Concepts Used

* Embedded C
* CAN Protocol
* ADC (Speed, RPM input)
* Inter-microcontroller communication
* Real-time data processing

---

```text id="bpt3u9"
        +----------------------+
        | ECU 1               |
        | Speed + Gear        |
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
        | ECU 2               |
        | RPM + Indicators    |
        +----------------------+
```
## 🔍 Feature to Implementation Mapping

| Feature           | Implementation      |
| ----------------- | ------------------- |
| Multi-node system | Multiple PIC MCUs   |
| Communication     | CAN Protocol        |
| Speed/RPM input   | ADC (Potentiometer) |
| User input        | Keypad              |
| Display           | CLCD                |


## 🔄 Working

### 🚗 ECU 1

* Reads speed using ADC (POT)
* Reads gear input via digital keypad
* Sends data via CAN

### 🚗 ECU 2

* Reads RPM using ADC
* Reads indicator status via digital Keypad
* Sends data via CAN

### 🧠 Main ECU

* Receives data from both ECUs
* Processes incoming messages
* Displays:

  * Speed
  * RPM
  * Gear
  * Indicator status on CLCD

---

## ⚡ Key Features

* Multi-node embedded system
* CAN-based real-time communication
* Distributed data acquisition
* Centralized data display

---

## 📊 Results

* Successful communication between multiple ECUs
* Real-time data synchronization achieved
* Accurate display of vehicle parameters

---

##Video link
https://drive.google.com/file/d/1DBVDsLQW1zQE0up9Ed-VPp6PlpFPhNlS/view?usp=drive_link

---

## ⚠️ Engineering Challenges

* CAN message synchronization
* Handling multiple data sources
* Debugging inter-node communication

---

## 🔮 Future Improvements

* Add fault detection system
* Integrate real vehicle sensors
* Expand to more ECUs
