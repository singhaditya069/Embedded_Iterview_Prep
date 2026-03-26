# Peripheral Interface - Conceptual Questions

## UART Interface
1. UART frame format and transmission.
2. UART timing and baud rate calculation.
3. Flow control in UART (hardware and software).
4. Error detection (parity, framing error).

## SPI Interface
1. SPI transmission modes (CPOL, CPHA combinations).
2. Chip select (CS) timing and configuration.
3. SPI clock frequency and timing constraints.
4. SPI peripherals: SD card, sensors, displays.

## I2C Interface
1. I2C bus protocol and timing.
2. Master/slave addressing schemes.
3. Slave addressing: 7-bit vs 10-bit.
4. Clock stretching.
5. Multi-master handling and arbitration.

## CAN Interface
1. CAN frame structure and arbitration.
2. CAN message ID and filtering.
3. CAN timeout and error handling.
4. CAN FD protocol.

## USB Interface
1. Basic USB concepts for embedded.
2. USB device implementation.
3. USB host and OTG modes.
4. USB data transfer rates and modes.

## Memory Interfaces
1. SRAM interface and timing.
2. Serial flash (NOR) interface.
3. NAND flash interface for storage.
4. Cache coherency with external memory.

## Real-world Scenario Questions

### Scenario 1: Unreliable I2C Communication
**Context**: I2C communication with sensor occasionally fails.

**Questions**:
1. What are common failure causes?
2. How would you diagnose the issue?
3. What hardware and software fixes would help?
4. How would you implement error recovery?

### Scenario 2: SD Card Interface
**Context**: Adding mass storage via SPI SD card.

**Questions**:
1. How do you implement SD card initialization?
2. What is the read/write sequence?
3. How do you handle error conditions?

