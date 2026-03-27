# Smart Heater Control System (STM32 Bare Metal)

![STM32F103](https://img.shields.io/badge/MCU-STM32F103C8T6-blue)
![C](https://img.shields.io/badge/Language-C-green)
![BareMetal](https://img.shields.io/badge/Architecture-Bare--Metal-orange)
![License](https://img.shields.io/badge/License-MIT-brightgreen)

An industrial-grade, fail-safe intelligent controller for infrared heaters. This project features a custom-built asynchronous firmware developed from the ground up using direct register manipulation (No HAL/LL libraries) to ensure maximum performance and deterministic behavior.

## 🚀 Key Features

*   **Brain-Muscle Architecture**: A decoupled design where logic (Decision Making) and execution (Hardware Safety) are isolated for maximum reliability.
*   **Asynchronous OneWire Driver**: Non-blocking DS18B20 integration with 0.1°C precision. Zero CPU-blocking during the 750ms sensor conversion cycle.
*   **Reactive Communication**: Custom binary protocol (v1.2) with XOR checksums. Command response latency < 10ms.
*   **Advanced Telemetry**: Dual-stream data reporting (User Data @ 30s, Health Data @ 5min) with instant event-driven updates for relay state changes.
*   **Hardware Safety**: Integrated Short Cycle Protection (5s relay guard) and fail-safe "No Signal = No Heat" logic.
*   **MCU Health Monitoring**: Continuous internal die temperature tracking via ADC1 + DMA + EMA (Exponential Moving Average) filter.

## 🏗️ Technical Architecture

The system operates on a **Decoupled Orchestration** model:

1.  **Logic Hub (telemetry_manager.c)**: Acts as the "Brain." It manages system time, processes UART commands, polls sensors, and makes heating decisions based on a 0.5°C hysteresis.
2.  **Safety Executor (thermostat.c)**: Acts as the "Muscle." It receives logic requests and applies them to the hardware only if safety constraints (timings) are met.
3.  **Transport Layer (frame_packer.c)**: A robust FSM-based parser that handles byte-stream reconstruction, timeouts, and CRC validation.

## 🛠️ Hardware Stack

*   **MCU**: STM32F103C8T6 (72MHz HSE Clock).
*   **Sensor**: DS18B20 (Digital Thermometer via OneWire).
*   **Actuator**: Power Relay (connected to PA0).
*   **Interface**: ESP32 Bridge (provides Wi-Fi and Web UI).

## 📂 Project Structure

```text
├── App/
│   ├── Inc/             # Modular headers (Service API)
│   └── Src/             # Implementation (Logic & Drivers)
├── Src/
│   └── main.c           # System Orchestrator & Task Dispatcher
├── UART_PROTOCOL.md     # Binary protocol specification v1.2
└── BARE_METAL_GUIDE.md  # Detailed register-level development insights
```

## 🔌 Communication Protocol (v1.2)

The system uses a binary frame structure for ultra-lean communication:
`[$] [CMD] [LEN] [DATA...] [XOR_CRC]`

| Command | Name | Description |
| :--- | :--- | :--- |
| 0x03 | USER_DATA | Current Temp, Target Temp (30s Heartbeat) |
| 0x04 | HEALTH_DATA | Sensor Stat, Relay State, MCU Temp, Uptime |
| 0x13 | SET_TARGET | Set new target temperature (Reactive) |

## ⚙️ Development Highlights

*   **Direct Register Access**: Every peripheral (RCC, GPIO, UART, ADC, DMA, SysTick) is configured via memory-mapped I/O.
*   **DWT Timing**: Uses the Data Watchpoint and Trace (DWT) unit for microsecond-precision delays required by the OneWire protocol.
*   **Memory Efficiency**: Zero dynamic allocation (Heap). Minimal stack usage through static module encapsulation.

## 👨‍💻 Author
**Andrey** - *Lead Embedded Systems Engineer*

---
*Developed as a high-performance prototype for smart home industrial solutions.*
