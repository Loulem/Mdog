# Mdog — Quadruped Robot Project

This repository contains the firmware and controller code for a quadruped robot dog project built with Arduino-compatible boards.

## Repository Structure

- **`Dog code/`** — Main robot firmware (PlatformIO project)
  - Complete quadruped control system with inverse kinematics, gait generation, and wireless control
  - See [`Dog code/ReadMe.md`](Dog%20code/ReadMe.md) for detailed documentation

- **`Dog controller/`** — Remote controller firmware (PlatformIO project)
  - nRF24L01 transmitter for sending movement commands to the robot

- **`old/`** — Legacy experimental code and reference implementations

## Quick Start

### Robot Firmware
```powershell
cd "Dog code"
pio run -e megaatmega2560  # or uno_r4_minima
pio run -t upload
```

### Controller Firmware
```powershell
cd "Dog controller"
pio run -t upload
```

## Branch Information

- **`refactor/clean-architecture`** — Current development branch with modular architecture

## Hardware Overview

- **Robot**: Arduino Mega 2560 or Uno R4 + 12 servos + 2× PCA9685 PWM drivers + nRF24L01
- **Controller**: Arduino board + nRF24L01 + joystick/control input

## Features

- 3-DOF inverse kinematics for precise leg control
- Pre-computed crawling gait with 100-step cycle
- Wireless command interface via nRF24L01 (2.4GHz)
- Debug mode for testing without hardware
- Dual I²C PWM driver support (up to 32 servos)

---

For detailed setup, configuration, and usage instructions, see the README in each project folder.
