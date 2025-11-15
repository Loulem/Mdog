# Robot Dog — Quadruped Robot Firmware

Firmware for a quadruped robot dog with inverse kinematics, gait generation, and wireless control (nRF24L01). Built with PlatformIO for Arduino-compatible boards.

## Features

- **Inverse Kinematics**: 3-DOF leg control with real-time position calculation
- **Crawling Gait**: Pre-computed 100-step walking cycle with lateral stability
- **Multi-Servo Control**: Dual I²C PWM drivers (up to 32 servos)
- **Wireless Control**: nRF24L01 radio for remote commands
- **Debug Mode**: Test without hardware using simulation mode

## System Architecture

The firmware is organized around a modular architecture:

- **`DogMaster`**: Central coordinator managing the control loop (polls RF, advances gait, computes kinematics, applies servo angles)
- **`RFController`**: Wireless receiver parsing `(angle,power)` commands from nRF24L01
- **`CrawlingTrajectory`**: Gait generator with 100-step cycle returning 3D positions for all legs
- **`LegKinematics`**: Inverse kinematics solver (3-DOF: hip, shoulder, knee)
- **`ServoBus`**: Dual PCA9685 PWM driver controller with per-servo calibration

## Hardware Requirements

- **Microcontroller**: Arduino Mega 2560 or Uno R4 Minima
- **Servo Drivers**: 2× PCA9685 (I²C addresses 0x40, 0x41)
- **Radio**: nRF24L01+ (CE: pin 7, CSN: pin 8)
- **Servos**: 12× servos (3 per leg)
- **Power**: 6V, 5-10A supply

### Servo Channel Mapping
| Leg           | Hip | Shoulder | Knee |
|---------------|-----|----------|------|
| Front Left    | 4   | 12       | 8    |
| Front Right   | 5   | 13       | 9    |
| Back Left     | 6   | 14       | 10   |
| Back Right    | 7   | 15       | 11   |

## Installation

**Prerequisites**: VS Code with PlatformIO extension

1. **Open project** in VS Code (File → Open Folder → select `Dog code`)

2. **Select environment** in PlatformIO toolbar: `megaatmega2560` or `uno_r4_minima`

3. **Build**: Click Build button or run `pio run -e megaatmega2560`

4. **Upload**: Connect board via USB, click Upload or run `pio run -t upload -e megaatmega2560`

5. **Monitor**: `pio device monitor -b 250000` (Mega: 57600 baud, Uno R4: 250000)

## Configuration

**Debug mode (no RF)**: Set `IGNORE_RF = true` in `DogMaster.h` to walk forward without radio

**Debug output**: Enable `-DDEBUG_PRINT_ENABLE` in `platformio.ini` for position/angle logging

**Servo calibration**: Edit `ServoConfig.h` for pulse width ranges and angle limits

**Gait tuning**: Modify `CrawlingTrajectory.h` constants:
- `WALK_RADIUS`: Step size (default 6.0cm)
- `STANCE_HEIGHT`: Body height (default 21.0cm)
- `NB_STEPS`: Steps per cycle (default 100)

## Usage

**Command format**: `(angle,power)` via nRF24L01
- `(0,50)` — Forward at 50% speed
- `(45,30)` — Turn 45° right at 30%
- `(0,0)` — Stop

## Project Structure

```
Dog code/
├── platformio.ini          # Build config
├── include/                # Headers
│   ├── DogMaster.h         # Main orchestrator
│   ├── RFController.h      # Radio
│   ├── CrawlingTrajectory.h # Gait
│   ├── LegKinematics.h     # IK solver
│   ├── ServoBus.h          # PWM drivers
│   └── ServoConfig.h       # Calibration
└── src/                    # Implementation
    ├── main.cpp
    └── [...]
```

## Troubleshooting

**Upload fails**: Check USB connection, verify COM port with `pio device list`, close serial monitor

**Servos jitter**: Check power supply current capacity

**No radio commands**: Verify CE/CSN pins (7, 8), check channel matches transmitter

**Unstable walking**: Adjust `STANCE_HEIGHT` or `WALK_RADIUS` in `CrawlingTrajectory.h`

## Future Improvements

- [ ] Rotation and reverse gaits
- [ ] IMU integration for balance
- [ ] Additional gait patterns (trot, sprint)
- [ ] Battery monitoring

---

**Leg dimensions**: Thigh 11.0cm, Shin 14.3cm | **Gait cycle**: 140 steps (~7sec default) | **Performance**: <1ms IK per leg, 300Hz PWM