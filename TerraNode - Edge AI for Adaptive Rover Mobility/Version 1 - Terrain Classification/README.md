# TerraNode AI: Offline Edge AI Perception for Autonomous Industrial Machines

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Hardware](https://img.shields.io/badge/Hardware-ESP32-blue.svg)]()
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()


## Overview
**TerraNode** is a proprioceptive (tactile) Edge AI architecture designed for autonomous robotic systems operating in harsh, unstructured industrial environments like agriculture and subterranean mining. 

Instead of relying on exteroceptive sensors (cameras/LiDAR) that fail in heavy dust, fog, or darkness, TerraNode allows a rover to physically "feel" the terrain. By fusing kinematic vibration data with electrical powertrain friction, the system classifies terrain in real-time entirely on a bare-metal microcontroller without requiring cloud connectivity.

## The Problem
Current autonomous rovers rely heavily on vision-first ecosystems. This introduces three critical failure points:
1.  **Visual Vulnerability:** Cameras and LiDAR are easily blinded by environmental occlusion (mud, dust, darkness).
2.  **Zero-Connectivity Constraint:** Heavy deep neural networks require cloud processing or expensive edge GPUs, making them unviable in deep-field agriculture or mines where Wi-Fi/5G networks are nonexistent.
3.  **Hardware Degradation:** Network latency prevents real-time torque adjustments, leading to wheel-slip, battery drain, and motor burnout on high-friction surfaces.

## The TerraNode Solution
TerraNode solves this via mathematical subtraction—replacing heavy vision models with an ultra-fast statistical sensor fusion pipeline running locally on an ESP32.

### Hardware Architecture
*   **Microcontroller:** ESP32 (Dual-core 240MHz Xtensa)
*   **Kinematic Sensor:** MPU-6050 (6-DOF IMU) measuring Z-axis standard deviation (mechanical impact) and spatial pitch angle.
*   **Friction Sensor:** INA219 (Precision Current Sensor) measuring mean electrical current draw from the L298N motor drivers to quantify physical surface drag.

### Software & ML Pipeline
1.  **Data Engineering (Python):** Raw telemetry is logged via an SD card module. Pandas and NumPy structure the noisy data into 66-tick temporal windows (1-second intervals).
2.  **Machine Learning:** A Softmax Regression classifier is trained using Scikit-Learn to categorize distinct physical terrains (e.g., Smooth, Grass, Gravel) based on electro-kinematic features.
3.  **Edge AI Transpilation:** The trained Softmax matrix is transpiled directly into a dependency-free, static C++ header file using `micromlgen`, entirely bypassing heavy ML interpreters like TFLite Micro.

## Performance Metrics
*   **Sensor Acquisition:** **15 ms** deterministic, non-blocking polling loop.
*   **Inference Latency:** **~0.15 ms** execution time directly on the bare-metal ESP32.
*   **Stability:** **Near 100% operational UI stability** during live surface transitions, achieved via a custom C++ temporal smoothing filter (majority-vote circular buffer) that eliminates hardware micro-vibrations.

## Getting Started

### Hardware Requirements
*   ESP32 Development Board
*   MPU-6050 IMU Module
*   INA219 Current Sensor Module
*   L298N Motor Driver & 4WD DC Motor Chassis
*   SD Card Module (for training data collection)

### Software Setup
1.  Clone this repository:
    ```bash
    git clone [https://github.com/yourusername/terranode.git](https://github.com/yourusername/terranode.git)
    ```
2.  **Firmware:** Open the firmware source code in the Arduino IDE or PlatformIO. Ensure you have the necessary I2C libraries (e.g., Adafruit MPU6050, Adafruit INA219) installed. Flash the code to your ESP32.
3.  **Machine Learning:** Navigate to the Python pipeline scripts. Install the required dependencies to review the data engineering and training process:
    ```bash
    pip install pandas numpy scikit-learn micromlgen
    ```

## Future Scope (Stage 2)
The roadmap for TerraNode includes transitioning from linear matrices to **TinyRL (Reinforcement Learning)** and **Random Forest** algorithms. By integrating a hardware voltage-divider circuit to track real-time battery health as an additional dynamic feature, the system will self-calibrate to compensate for hardware drift and voltage sag over extended deployments.

---

*(Note: To see the system in action, watch the short demonstration GIF here [https://drive.google.com/drive/folders/15wK2Z1NLwWeTjrveFmHkj5chMvllWjNF?usp=sharing])*