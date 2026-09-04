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

The following table maps each digital pin to its corresponding AVR port and bit:

| Digital pin | AVR port | Digital pin | AVR port | Digital pin | AVR port |
| ---: | :--- | ---: | :--- | ---: | :--- |
| 0 | PE0 | 18 | PD3 | 36 | PC1 |
| 1 | PE1 | 19 | PD2 | 37 | PC0 |
| 2 | PE4 | 20 | PD1 | 38 | PD7 |
| 3 | PE5 | 21 | PD0 | 39 | PG2 |
| 4 | PG5 | 22 | PA0 | 40 | PG1 |
| 5 | PE3 | 23 | PA1 | 41 | PG0 |
| 6 | PH3 | 24 | PA2 | 42 | PL7 |
| 7 | PH4 | 25 | PA3 | 43 | PL6 |
| 8 | PH5 | 26 | PA4 | 44 | PL5 |
| 9 | PH6 | 27 | PA5 | 45 | PL4 |
| 10 | PB4 | 28 | PA6 | 46 | PL3 |
| 11 | PB5 | 29 | PA7 | 47 | PL2 |
| 12 | PB6 | 30 | PC7 | 48 | PL1 |
| 13 | PB7 | 31 | PC6 | 49 | PL0 |
| 14 | PJ1 | 32 | PC5 | 50 | PB3 (MISO) |
| 15 | PJ0 | 33 | PC4 | 51 | PB2 (MOSI) |
| 16 | PH1 | 34 | PC3 | 52 | PB1 (SCK) |
| 17 | PH0 | 35 | PC2 | 53 | PB0 (SS) |

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
