# Robot Dog — PlatformIO Arduino Project

This repository contains firmware and supporting files for a multi-legged "robot dog" project written for Arduino-compatible boards using PlatformIO.

## Overview

The project implements low-level leg control, walking/crawling paths, and a basic communication interface (RF24) to receive movement commands. The codebase is set up for development with PlatformIO and targets Arduino-style boards (examples: Arduino Mega 2560, Arduino Uno R4).

## Quick Start

Prerequisites:
- Visual Studio Code
- PlatformIO extension for VS Code
- USB drivers for your target board (Arduino Mega or Uno R4)

1. Open this project folder in VS Code.
2. Install the PlatformIO extension (if not already installed).
3. In the bottom PlatformIO status bar, choose the active environment (for example `megaatmega2560` for an Arduino Mega 2560 or `uno_r4_minima` for an Uno R4 board).
4. Build the project with the PlatformIO toolbar or Run: `PlatformIO: Build`.
5. Connect your board and upload using `PlatformIO: Upload`.

Note: The default environments are defined in `platformio.ini`. If you use a different board, change the environment or create a new one in `platformio.ini`.

## Project Structure

- `platformio.ini` — PlatformIO project configuration and build environments.
- `robot_dog_main.cpp`, `src/main.cpp` — main firmware entry points.
- `include/` — project headers (robot leg control, walking logic, communication, tests).
- `lib/` — extra libraries (empty README here).
- `test/` — test utilities and examples.

## Common Tasks

- Build: Use PlatformIO Build in VS Code.
- Upload: Use PlatformIO Upload (choose the environment matching your board).
- Serial Monitor: Use PlatformIO Serial Monitor. The project uses high baud rates in places (e.g. 250000); set the monitor speed accordingly.

## Notes and TODOs

The code contains several in-code TODOs (e.g., replace atan with atan2, add reverse gait and rotation). See top-level comments in the source files.

## Contributing

See `CONTRIBUTING.md` for how to contribute, coding style, and how to run tests.

## License

This project is available under the MIT License — see the `LICENSE` file.

---
_If you prefer an English/French bilingual README or want hardware wiring diagrams added, I can add them next._