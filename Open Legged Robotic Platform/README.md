# JARVIS – Open Legged Robotic Platform

> **Version:** v0.1 – Hardware & Foundation

## Project Status

🚧 Under Active Development

---

## Overview

JARVIS is a custom-built quadruped robot developed as a long-term robotics and artificial intelligence project. The objective is to design, build, and program a stable, modular, and intelligent walking robot while learning robotics from first principles, including mechanics, electronics, embedded systems, control theory, computer vision, machine learning, and autonomous navigation.

The project follows an incremental development approach, where each version introduces new hardware and software capabilities.

---

## Current Version

**Version:** v0.1 – Hardware & Foundation

This release establishes the hardware foundation of the robot, including the mechanical structure, electronics architecture, power distribution, and development environment.

---

## Version History

| Version | Status | Description |
|----------|:------:|-------------|
| **v0.1** | Current | Hardware & Foundation |

---

## Hardware

### Controller

- ESP32-WROOM-32 (30 Pin)

### Servo Driver

- PCA9685 16-Channel PWM Driver

### Sensors

- HC-SR04 Ultrasonic Sensor

### Actuators

- 9 × MG90S Metal Gear Servo Motors

### Power System

- 2 × 3.7V LiPo Batteries (2S Configuration)
- DC-DC Buck Converter (7.4V → 5V)
- Main Power Switch
- 4700 µF Capacitor

---

## Mechanical Design

Current mechanical components include:

- 1 Robot Head
- 4 Femurs
- 4 Lower Legs (Tibia)

All mechanical components are designed using Autodesk Fusion 360.

---

## License

This project is licensed under the **MIT License**.

See the `LICENSE` file for more information.