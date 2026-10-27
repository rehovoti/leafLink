# AGENTS.md

## Project overview

This repository is an ESP32/Arduino firmware project for a LeafLink monitoring and control system. The codebase is organized around a modular FreeRTOS-style architecture: each subsystem lives in its own library directory under `lib/`, while the main application entry point is in `src/main.cpp`.

The project appears to be in an early-to-mid implementation stage. Several modules are present and partially wired in, but some integration paths are commented out or still use placeholder values. The design intent is to collect sensor data, manage pump operation, provide Wi-Fi configuration, and send telemetry to a remote endpoint.

## Repository structure

- `src/main.cpp` — application startup and task registration
- `src/common.h` — shared data model used across tasks
- `lib/Measurements/` — sensor acquisition logic (temperature sensors)
- `lib/Pump/` — pump control logic
- `lib/WiFiSetup/` — captive Wi-Fi configuration portal support
- `lib/TelemetrySender/` — telemetry upload logic over HTTP
- `lib/README` — generic PlatformIO library notes
- `test/README` — PlatformIO test-runner reference
- `README` — repo notes / snippets and installation tip for the CH340 USB driver

## Architecture summary

### 1. Entry point and runtime model

The firmware starts in `src/main.cpp`:

- `setup()` initializes serial output
- `initWiFiSetup()` is called to configure the ESP32 Wi-Fi mode and trigger pin
- two tasks are created:
  - `TelemetryTask`
  - `wifiSetupTask`
- `loop()` is empty, which is typical for ESP32 tasks built around FreeRTOS

This indicates the project expects to run as a multitask firmware application rather than a single loop-driven sketch.

### 2. Shared data contract

`src/common.h` defines the core payload type:

- `tempIn`
- `tempOut`
- `tempTank`
- `am2302_temp`
- `am2302_humidity`
- `voltage`
- `pumpStatus`

The queue constants are also here:

- `QUEUE_LENGTH 10`
- `ITEM_SIZE sizeof(SensorData)`

This is the intended shared data structure for sensor values flowing between tasks. The `README` in the repo includes an earlier queue example using a `SensorData` struct, which matches the current `common.h` data model.

### 3. Measurement subsystem

`lib/Measurements/Measurements.h` and `Measurements.cpp` implement the environmental measurement layer.

Key points:

- DS18B20 sensors are used for temperature readings
- `T_SURFACE_PIN`, `T_WATER_IN_PIN`, and `T_WATER_OUT_PIN` are defined for sensors
- `initMeasurements()` calls `tSurfaceSensor.begin()`
- `measurementsTask()` requests temperatures and reads them in a loop
- there are commented-out references to additional sensors and print helpers

This module is clearly designed to read temperature values periodically and expose them to the rest of the system, but it is not yet actively started in `main.cpp`.

### 4. Pump subsystem

`lib/Pump/Pump.h` and `Pump.cpp` implement pump control.

Key points:

- pump pins are defined as:
  - `pump_enA 27`
  - `pump_in1 32`
  - `pump_in2 33`
- `initPump()` sets the pins as outputs and configures motor direction
- `pumpTask()` ramps the PWM duty cycle across a range from 0 to 255 with a 1-second delay between steps

This is a direct hardware-control module for a DC pump motor. It is a candidate consumer of sensor data, but the task is currently commented out in `main.cpp`.

### 5. Wi-Fi setup and configuration portal

`lib/WiFiSetup/WiFiSetup.h` and `WiFiSetup.cpp` provide a Wi-Fi management layer using `WiFiManager`.

Key points:

- `TRIGGER_PIN 0` is used as a hardware trigger
- `TIMEOUT 120` sets the config portal time limit
- `initWiFiSetup()` sets the ESP32 to Wi-Fi station mode and configures the trigger pin as input with pull-up
- `wifiSetupTask()` monitors the trigger pin and, when pressed, starts a config portal at `192.168.4.1`

This allows the device to enter a captive portal for first-time setup or reconfiguration without hardcoding credentials in firmware.

### 6. Telemetry sender

`lib/TelemetrySender/TelemetrySender.h` and `TelemetrySender.cpp` handle outbound telemetry.

Key points:

- `TelemetryTask(void* parameters)` loops forever
- it checks `WiFi.status()` and reconnects if needed
- it uses `WiFiManager` to fetch saved SSID/password values
- it submits an HTTP GET request to a Google Apps Script URL
- the URL includes query parameters such as:
  - `tin`
  - `tout`
  - `ttank`
  - `voc`
  - `At`
  - `Ah`
  - `lux`
  - `pump`

The telemetry URL is currently static and hardcoded in the source. It is not yet fed by the shared sensor data structure, so it behaves more like a proof-of-concept integration than a final production pipeline.

## Intended system flow

Based on the code and comments, the intended flow looks like this:

1. Device boots and initializes Wi-Fi configuration handling.
2. User may trigger the Wi-Fi Manager portal to connect to a local network.
3. Sensor tasks acquire readings from DS18B20 and AM2302-style inputs.
4. Sensor values are collected into `SensorData` and placed into a queue.
5. Downstream tasks consume the data for:
   - telemetry transmission
   - pump control decisions
   - logging or storage
6. The system runs as independent FreeRTOS tasks with periodic delays for sensor polling and network reporting.

## Current integration status

The codebase is a partial implementation with several important observations:

- The project is clearly structured as an ESP32/PlatformIO firmware project.
- `src/main.cpp` only starts `TelemetryTask` and `wifiSetupTask`.
- Measurement and pump tasks are present but commented out in `main.cpp`.
- The queue-based architecture is defined, but not yet wired into the active runtime tasks.
- Telemetry is not yet driven by real sensor data; it uses a hardcoded Google Apps Script URL.
- The code is in a prototype/integration stage rather than a finished production build.

## Build and tooling signals

The repo includes PlatformIO conventions:

- `lib/` directory for project-specific libraries
- `test/` directory for PlatformIO test runner
- `README` references ESP32 board setup and driver installation

There is no `platformio.ini` in the workspace snapshot, so the build configuration may be missing or intentionally omitted in this current repository state.

## Recommended next steps for future work

1. Add the missing `platformio.ini` if this is meant to be a complete PlatformIO project.
2. Re-enable and wire the measurement and pump tasks in `src/main.cpp`.
3. Replace the static telemetry URL with a data-driven payload built from `SensorData`.
4. Add proper task synchronization using the shared queue and `xQueueSend` / `xQueueReceive` pattern.
5. Validate hardware pin mappings and sensor libraries against the target ESP32 board.

## Agent guidance

When working on this repository, prefer the following mental model:

- `src/main.cpp` is the orchestrator
- `lib/*` modules are independent task implementations
- `src/common.h` defines the shared contract between tasks
- the design is FreeRTOS-based and event-driven, so new tasks should be added carefully and should communicate through the `SensorData` queue or explicit API boundaries

Keep changes modular and consistent with the existing hardware-oriented, Arduino/ESP32 style used throughout the project.
