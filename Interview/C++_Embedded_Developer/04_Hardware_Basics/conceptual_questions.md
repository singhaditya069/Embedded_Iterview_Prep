# Hardware Basics - Conceptual Questions

## Digital Logic Fundamentals
1. What are combinational and sequential circuits?
2. Explain logic gates: AND, OR, NOT, NAND, NOR, XOR.
3. What is a flip-flop and how does it store state?
4. Explain multiplexers, demultiplexers, and decoders.

## Microcontroller Architecture
1. What is a microcontroller vs. microprocessor?
2. Explain the Von Neumann and Harvard architectures.
3. What is the fetch-execute cycle?
4. Explain instruction pipelining and its benefits.

## Registers and Memory-Mapped I/O
1. What are CPU registers and their types (general-purpose, special-purpose)?
2. How does memory-mapped I/O work?
3. Explain port-mapped I/O vs. memory-mapped I/O.
4. What is the purpose of control registers?

## Interrupt System
1. What is an interrupt and how does it differ from polling?
2. Explain interrupt priorities and nesting.
3. What is an interrupt service routine (ISR)?
4. How does interrupt masking work?

## Communication Protocols Overview
1. What are serial vs. parallel communication?
2. Explain synchronous vs. asynchronous communication.
3. What is baud rate and how is it calculated?
4. Basic overview of UART, SPI, I2C protocols.

## Timers and Counters
1. What are timer/counter modules in microcontrollers?
2. Explain PWM (Pulse Width Modulation).
3. What are capture and compare functions?
4. How do you measure frequency and period using timers?

## Analog-to-Digital Conversion
1. What is ADC and its role in embedded systems?
2. Explain sampling, quantization, and resolution.
3. What is Nyquist theorem?
4. Explain different ADC architectures (Successive Approximation, Sigma-Delta).

## Power Management
1. What are power modes in microcontrollers (Active, Idle, Sleep, Deep Sleep)?
2. How do you reduce power consumption?
3. What is sleep mode and wake-up mechanisms?
4. Explain dynamic power and static power consumption.

## Real-world Hardware Questions

### Scenario 1: High-frequency Data Acquisition
**Context**: Acquiring sensor data at 100kHz with limited processing power.

**Questions**:
1. Would you use polling or interrupt-driven acquisition?
2. How do you handle interrupt overhead at this frequency?
3. What role does DMA play?
4. How would you design the data flow?

### Scenario 2: Power-constrained IoT Device
**Context**: Battery-powered device needs 1-year lifetime on a single charge.

**Questions**:
1. What power management strategies would you use?
2. How would you minimize CPU utilization in idle states?
3. What is the trade-off between performance and power?
4. How do you measure and optimize power consumption?

