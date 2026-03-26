# Communication Protocols - Conceptual Questions

## UART (Universal Asynchronous Receiver/Transmitter)
1. How does UART asynchronous communication work?
2. Explain start bit, data bits, parity, stop bits.
3. How is baud rate achieved in UART?
4. What are common UART configurations (8N1, 7E1)?
5. How do you handle UART buffer overflows?

## SPI (Serial Peripheral Interface)
1. Explain the SPI communication protocol.
2. What are the roles of Master, Slave, MOSI, MISO, CLK, CS?
3. What are SPI modes (CPOL and CPHA)?
4. How does SPI differ from UART?
5. What are advantages and limitations of SPI?

## I2C (Inter-Integrated Circuit)
1. How does I2C two-wire communication work?
2. Explain open-drain output and pull-up resistors.
3. What is I2C address assignment and 7-bit vs 10-bit addressing?
4. How does I2C clock stretching work?
5. What are I2C speed modes (Standard, Fast, Fast+, High-speed)?

## CAN (Controller Area Network)
1. What is CAN and why is it used in automotive?
2. Explain CAN message structure (ID, DLC, data).
3. What is arbitration in CAN?
4. How does CAN error handling work?
5. What are CAN FD (Flexible Data-rate) improvements?

## LIN (Local Interconnect Network)
1. What is LIN and how does it differ from CAN?
2. Explain LIN master device concept.
3. What is the LIN frame format?
4. What are typical LIN baud rates?

## Ethernet and Industrial Protocols
1. Basic Ethernet frame structure for embedded systems.
2. What is Real-time Ethernet (TSN)?
3. Overview of industrial protocols: Modbus, Profibus, EtherCAT.

## Real-world Scenario Questions

### Scenario 1: Multi-device Communication Bus
**Context**: Communicating with 10 slave devices over I2C.

**Questions**:
1. How would you handle I2C bus conflicts?
2. What is the maximum data transfer rate?
3. How would you handle devices with different response times?

### Scenario 2: High-speed Sensor Interface
**Context**: Reading data from SPI ADC at 5MHz clock.

**Questions**:
1. How would you optimize SPI timing?
2. What are the minimum cable lengths?
3. How do you address noise and signal integrity?

