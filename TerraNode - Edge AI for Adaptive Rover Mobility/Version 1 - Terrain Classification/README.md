# TerraNode V1 — Terrain Classification Baseline

**Status: Completed**  
**Platform: 4WD Mobile Robot**  
**Controller: ESP32**  
**Machine Learning: Softmax Regression**  
**Development Environment: Arduino IDE + Python**

---

## 1. Overview

TerraNode V1 is the first functional prototype of the TerraNode robotic platform.

The objective of this version was to investigate whether onboard proprioceptive sensor data from a 4WD rover could be used to identify different terrain conditions and perform machine-learning inference locally on an ESP32.

The system uses motion information from an **MPU6050 IMU** together with **motor-current measurements from an INA219** to characterize the rover's interaction with the ground.

V1 establishes the initial perception and edge-inference pipeline that will serve as the baseline for future versions of TerraNode.

---

## 2. Objective

The primary objectives of TerraNode V1 were:

- Build a functional 4WD robotic platform.
- Collect sensor data while the rover operates on different surfaces.
- Extract meaningful features from IMU and motor-current measurements.
- Train a classical machine-learning model for terrain classification.
- Deploy the trained model for inference on an ESP32.
- Evaluate the classification output and establish a baseline for future development.

---

## 3. System Architecture

```text
                    TerraNode V1 — Data & Inference Pipeline

        ┌─────────────────────┐
        │      MPU6050 IMU    │
        │ Acceleration + Gyro │
        └──────────┬──────────┘
                   │
                   │
        ┌──────────▼──────────┐
        │   INA219 Current    │
        │       Sensing       │
        └──────────┬──────────┘
                   │
                   │ Sensor Data
                   ▼
        ┌─────────────────────┐
        │   Data Acquisition  │
        │       + Logging     │
        └──────────┬──────────┘
                   │
                   ▼
        ┌─────────────────────┐
        │  Feature Extraction │
        │                     │
        │ Acceleration stats  │
        │ Gyroscope stats     │
        │ Pitch angle         │
        │ Motor-current stats │
        │ PWM                 │
        └──────────┬──────────┘
                   │
                   ▼
        ┌─────────────────────┐
        │  Softmax Regression │
        │   Offline Training  │
        └──────────┬──────────┘
                   │
                   │ Trained Model
                   ▼
        ┌─────────────────────┐
        │   Model Parameters  │
        │    (.h / C++ code)  │
        └──────────┬──────────┘
                   │
                   ▼
        ┌─────────────────────┐
        │        ESP32        │
        │   Edge Inference    │
        └──────────┬──────────┘
                   │
                   ▼
        ┌─────────────────────┐
        │   Terrain Class     │
        │                     │
        │ Smooth / Grass /    │
        │ Gravel / etc.       │
        └─────────────────────┘