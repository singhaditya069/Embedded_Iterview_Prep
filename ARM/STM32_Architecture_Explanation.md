# STM32 Architecture Explanation

## Overview
STM32 is a family of 32-bit microcontrollers based on ARM Cortex-M processor cores, developed by STMicroelectronics. This document provides a comprehensive explanation of STM32 architecture, covering hardware components, memory organization, peripherals, and system integration.

## Core Architecture

### ARM Cortex-M Core
STM32 microcontrollers are built around ARM Cortex-M series processors:

- **Cortex-M0/M0+**: Entry-level, optimized for cost and power
- **Cortex-M3**: Balanced performance and power consumption
- **Cortex-M4**: DSP capabilities, single-precision FPU
- **Cortex-M7**: High performance, double-precision FPU, optional cache

### Key Core Features
- **Thumb-2 Instruction Set**: Mix of 16-bit and 32-bit instructions for code density
- **Nested Vectored Interrupt Controller (NVIC)**: Efficient interrupt handling
- **System Tick Timer**: Provides system timing
- **Memory Protection Unit (MPU)**: Memory access control (in M3/M4/M7)
- **Floating Point Unit (FPU)**: Hardware floating-point operations (in M4/M7)

## Memory Architecture

### Memory Types
1. **Flash Memory**: Non-volatile program storage
   - Varies from 16KB to 2MB depending on model
   - Organized in sectors/pages for erase operations
   - Can be programmed in-system

2. **SRAM**: Volatile data memory
   - Ranges from 4KB to 1MB
   - Single-cycle access
   - Can be retained in low-power modes

3. **EEPROM**: Electrically erasable programmable read-only memory
   - Available in some STM32F1/F3 series
   - Byte-addressable
   - Limited write cycles

### Memory Map
STM32 uses a 32-bit address space with predefined regions:

```
0x0000 0000 - 0x1FFF FFFF: Code (Flash, System Memory)
0x2000 0000 - 0x3FFF FFFF: SRAM
0x4000 0000 - 0x5FFF FFFF: Peripherals
0x6000 0000 - 0x9FFF FFFF: External RAM
0xA000 0000 - 0xDFFF FFFF: External Devices
0xE000 0000 - 0xFFFF FFFF: Cortex-M Internal Peripherals
```

## Bus System

### Advanced High-performance Bus (AHB)
- Main system bus connecting CPU to high-bandwidth peripherals
- 32-bit data bus
- Supports burst transfers
- Multiple masters: CPU, DMA controllers

### Advanced Peripheral Bus (APB)
- Lower bandwidth bus for slower peripherals
- APB1: Typically 36MHz max
- APB2: Higher speed for fast peripherals
- Bridge connected to AHB

### DMA Controller
- Direct Memory Access for high-throughput data transfers
- Reduces CPU overhead
- Supports memory-to-memory, peripheral-to-memory transfers
- Multiple channels with configurable priorities

## Clock System

### Clock Sources
1. **HSI (High-Speed Internal)**: 16MHz RC oscillator
2. **HSE (High-Speed External)**: External crystal/ceramic resonator
3. **LSI (Low-Speed Internal)**: 32kHz RC oscillator for watchdog
4. **LSE (Low-Speed External)**: 32.768kHz crystal for RTC

### Clock Distribution
```
HSI/HSE → PLL → System Clock → AHB Prescaler → APB1/APB2 Prescalers
                                    ↓
                              CPU Clock
```

- **PLL**: Phase-locked loop for frequency multiplication
- **Prescalers**: Divide clock frequencies for different domains
- **MCO (Microcontroller Clock Output)**: Output clock signal to external devices

## Power Management

### Power Domains
- **VDD**: Main power supply (1.7V - 3.6V)
- **VDDA**: Analog power supply
- **VREF+**: Reference voltage for ADC
- **VBAT**: Backup power for RTC and backup registers

### Low-Power Modes
1. **Sleep Mode**: CPU stopped, peripherals active
2. **Stop Mode**: All clocks stopped, SRAM retained
3. **Standby Mode**: Lowest power consumption, wake from reset
4. **Shutdown Mode** (STM32L4+): Ultra-low power with limited wake-up

### Voltage Regulator
- **Main regulator**: Active in Run/Sleep modes
- **Low-power regulator**: Used in Stop/Standby modes
- **Backup regulator**: Powers RTC and backup domain

## GPIO (General Purpose Input/Output)

### GPIO Ports
- Up to 16 ports (GPIOA to GPIOP) depending on package
- Each port has 16 pins (0-15)
- Configurable per pin

### Pin Configuration
- **Mode**: Input, Output, Analog, Alternate Function
- **Output Type**: Push-pull, Open-drain
- **Speed**: Low, Medium, High, Very High
- **Pull-up/Pull-down**: Internal resistors
- **Alternate Functions**: Peripheral multiplexing

### EXTI (External Interrupt)
- 16 lines for external interrupts
- Configurable edge detection (rising, falling, both)
- Wake-up capability from low-power modes

## Analog Peripherals

### ADC (Analog-to-Digital Converter)
- 12-bit resolution (up to 16-bit in some models)
- Multiple channels (up to 24)
- Sampling rates up to 5 Msps
- Internal reference voltage
- DMA support

### DAC (Digital-to-Analog Converter)
- 12-bit resolution
- 1 or 2 channels
- Buffered/unbuffered output
- DMA support

### Comparators
- Fast analog comparators
- Programmable thresholds
- Interrupt generation

## Timers

### General-Purpose Timers
- 16-bit or 32-bit counters
- Multiple channels for PWM, input capture, output compare
- Encoder interface support
- DMA request generation

### Advanced Control Timers
- Complementary PWM outputs
- Dead-time insertion for motor control
- Break input for safety
- Hall sensor interface

### Basic Timers
- Simple 16-bit counters
- Used for time-base generation
- DAC trigger in some models

## Communication Interfaces

### UART/USART
- Asynchronous/synchronous serial communication
- Baud rates up to system clock/16
- Hardware flow control
- DMA support
- SmartCard, IrDA, LIN support in some USARTs

### SPI (Serial Peripheral Interface)
- Full-duplex synchronous communication
- Master/slave modes
- Up to 50 Mbps
- Multiple slave select lines
- DMA support

### I2C (Inter-Integrated Circuit)
- Multi-master/multi-slave bus
- Standard mode (100 kbps), Fast mode (400 kbps), Fast+ (1 Mbps)
- 7-bit/10-bit addressing
- DMA support

### CAN (Controller Area Network)
- CAN 2.0B protocol support
- Bit rates up to 1 Mbps
- Hardware message filtering
- Time-triggered communication

### USB
- Full-speed (12 Mbps) or High-speed (480 Mbps) depending on model
- Host, Device, or OTG modes
- Built-in PHY in some models

### Ethernet (in select models)
- 10/100 Mbps MAC
- MII/RMII interface
- Hardware checksum offload

## Security Features

### Hardware Security
- **Unique Device ID**: 96-bit unique identifier
- **Flash Protection**: Read/write protection
- **JTAG/SWD Protection**: Debug interface disable
- **Cryptographic Acceleration**: AES, DES, HASH in some models

### TrustZone (in STM32L5 series)
- Hardware-enforced security
- Secure and non-secure worlds
- Protected peripherals and memory

## Development and Debugging

### Debug Interfaces
- **SWD (Serial Wire Debug)**: 2-pin debug interface
- **JTAG**: 4-pin debug interface (in larger packages)
- **ETM (Embedded Trace Macrocell)**: Instruction trace (Cortex-M3/M4)
- **ITM (Instrumentation Trace Macrocell)**: Software trace

### Boot Modes
- **Main Flash**: Normal operation
- **System Memory**: Bootloader for programming
- **SRAM**: For testing or emergency recovery
- **Embedded Bootloader**: UART, USB, SPI, I2C, CAN interfaces

## Package Options

### Pin Count
- 20 to 216 pins depending on model
- Various package types: LQFP, UFBGA, WLCSP, etc.

### Pin Functions
- **Power pins**: VDD, VSS, VDDA, VSSA, VBAT
- **Reset and Boot**: NRST, BOOT0, BOOT1
- **Oscillator**: HSE, LSE pins
- **Debug**: SWD/JTAG pins
- **GPIO**: Multiplexed with alternate functions

## STM32 Series Overview

### Mainstream Series
- **STM32F0**: Cortex-M0, entry-level
- **STM32F1**: Cortex-M3, high performance
- **STM32F2**: Cortex-M3, Ethernet support
- **STM32F3**: Cortex-M4, mixed-signal
- **STM32F4**: Cortex-M4, DSP/FPU
- **STM32F7**: Cortex-M7, advanced graphics
- **STM32H7**: Cortex-M7, high performance

### Ultra-Low-Power Series
- **STM32L0**: Cortex-M0+, ultra-low power
- **STM32L1**: Cortex-M3, ultra-low power
- **STM32L4**: Cortex-M4, ultra-low power
- **STM32L5**: Cortex-M33, security features

### General-Purpose Series
- **STM32G0**: Cortex-M0+, advanced peripherals
- **STM32G4**: Cortex-M4, advanced analog

## System Integration

### Reset and Clock Control (RCC)
- Manages all resets and clocks
- Configures clock sources and prescalers
- Controls peripheral clocks

### Power Control (PWR)
- Manages power modes
- Controls voltage regulator
- Handles wake-up sources

### Flash Interface
- Programs and erases flash memory
- Implements read/write protection
- Supports dual-bank flash in some models

### Backup Domain
- RTC and backup registers
- Powered by VBAT
- Retained in Standby/Shutdown modes

This comprehensive architecture enables STM32 microcontrollers to be used in a wide range of applications from simple IoT devices to complex industrial control systems, automotive electronics, and consumer products.