# TerraNode — Edge AI for Adaptive Rover Mobility

TerraNode is a low-cost 4WD robotic platform developed to explore the integration of edge AI, embedded systems, multimodal sensor data, and adaptive robot control.

The project is being developed incrementally through versioned prototypes. Each version is intended to establish a working baseline, identify limitations, introduce measurable improvements, and provide the foundation for the next stage of development.

The long-term objective is to move from simple terrain perception toward a rover that can understand its own mobility condition, adapt its behavior, and eventually recover autonomously from difficult terrain.

---

## Project Objective

The long-term goal of TerraNode is to develop a rover capable of:

- collecting and processing onboard sensor data,
- understanding terrain and mobility conditions,
- performing AI inference locally on an embedded controller,
- adapting its driving behavior according to observed conditions,
- detecting loss of mobility such as excessive slip or increasing resistance,
- and eventually performing autonomous recovery when mobility deteriorates.

The project follows the progression:

**Terrain Perception → Mobility Estimation → Adaptive Control → Autonomous Recovery**

---

# Version 1 — Terrain Classification

**Status: Completed**

Version 1 is the initial baseline prototype of TerraNode.

The objective of this version was to investigate whether onboard proprioceptive sensor data could be used to classify different terrain conditions and perform machine-learning inference directly on an ESP32.

### System Architecture

```text
MPU6050 IMU + Motor Current Data
              ↓
       Feature Extraction
              ↓
       Softmax Regression
              ↓
       Terrain Classification
              ↓
        ESP32 Inference