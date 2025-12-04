# Arduino RV Heater Controller

A simple Arduino-based heater controller for RV applications.  
Controls a resistive heating element via a zero-cross SSR and drives a small fan in parallel. Includes overtemperature protection and adjustable heating cycles via a potentiometer.

---

## Features

- 10-second heating cycles, configurable hourly energy budget.
- Three potentiometer behavior modes:
  - **Option A:** Apply changes at the next hour boundary.
  - **Option B (default):** Apply changes immediately.
  - **Option C:** Apply changes proportionally to the remaining hour.
- Overtemperature safety switch stops heating immediately.
- LED indicator for overtemperature conditions.
- Fan control via BS170 MOSFET, synchronized with SSR for heatsink cooling.
- Hourly reset of heating cycles for predictable energy management.

---

## Hardware

- Arduino Nano (or compatible)
- Zero-crossing SSR for resistive heating element
- BS170 MOSFET for fan control
- Overtemperature switch (normally closed)
- Reset switch (optional)
- Potentiometer for adjusting heating cycles
- Fan mounted on SSR heatsink
- Aluminium heatsink for SSR
- LEDs for status indication

---

## Wiring Overview

**Low voltage (Arduino side):**

- `OVERTEMP_PIN` → NC contact of overtemp switch → GND
- `RESET_PIN` → NC contact of reset switch → GND (optional)
- `RELAY_PIN` → SSR control input (5V logic)
- `command_pin` → Potentiometer wiper, ends to 5V/GND
- BS170 MOSFET gate → Arduino pin (optional fan control)

**High voltage (SSR side):**

- Load side of SSR → Resistive heating element AC connections
- SSR heatsink mounted on aluminium for thermal dissipation

> ⚠ **Important:** Keep AC wiring isolated from Arduino and low-voltage wiring. Always follow proper electrical safety practices.

---

## Software

- Adjust `BEHAVIOR_MODE` in the sketch.
0 = Option A (apply change at next hour)
1 = Option B (apply change immediately)
2 = Option C (apply proportional change)
