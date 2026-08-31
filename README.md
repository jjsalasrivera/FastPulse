# FastPulse

FastPulse is a compact Arduino project focused on high-speed digital output control using direct port manipulation on an Arduino Mega 2560.

The implementation is inspired by and based on the work described in the following reference project:

https://github.com/MonzurulAlam/OpenVstim/blob/main/README.md

This project follows the same general approach of optimizing pin operation by bypassing slower standard Arduino functions and writing directly to the MCU registers. That makes it suitable for timing-sensitive applications where performance matters more than readability.

## Purpose

The project demonstrates how to use hardware-level register access to:

- configure pins faster than using `pinMode()`
- toggle outputs with reduced overhead using direct port writes
- achieve lower-latency behavior in time-critical tasks

## Arduino Mega 2560 port mapping

The following pins are configured using direct register access for faster operation:

### Digital 10 => PB4

```cpp
DDRB = B00111111;  // faster pinMode operation
PORTB = B00010000; // faster digitalWrite operation
```

- `DIG 10` maps to `PB4`
- `DDRB` configures the data direction register
- `PORTB` writes the output value directly to the port register

### Digital 9 => PH6

```cpp
DDRH = B01000000;  // faster pinMode operation
PORTH = B01000000; // faster digitalWrite operation
```

- `DIG 9` maps to `PH6`
- `DDRH` configures the pin direction
- `PORTH` writes the output state directly to the port register

## Notes

This kind of optimization is commonly used in embedded systems where execution speed is critical, such as signal generation, timing control, or fast output patterns. The code is intentionally low-level and should be understood carefully before being reused in production projects.

## Reference

This project is based on the concepts and implementation patterns described in the work referenced above:

- OpenVstim reference: https://github.com/MonzurulAlam/OpenVstim/blob/main/README.md
