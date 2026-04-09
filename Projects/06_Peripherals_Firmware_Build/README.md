# Peripherals, Firmware Flashing & Build System - Complete Implementation Guide

## Project Overview

This project covers integrating device drivers, firmware flashing procedures, build system optimization, and tool integration (like ENTRAPASS). This is crucial for production-ready embedded systems.

---

## Peripheral Driver Architecture

### Driver Layer Model

```
┌───────────────────────────────────┐
│   Application Layer               │
│   (Main Application Code)         │
└────────────┬──────────────────────┘
             │
┌────────────▼──────────────────────┐
│   Device Abstraction Layer (DAL)  │
│   (UART, SPI, I2C, ADC, etc)     │
└────────────┬──────────────────────┘
             │
┌────────────▼──────────────────────┐
│   Hardware Abstraction Layer (HAL)│
│   (STM32Cube, NXP SDK, etc)      │
└────────────┬──────────────────────┘
             │
┌────────────▼──────────────────────┐
│   Low-Level Drivers              │
│   (GPIO, Clock, Interrupts)      │
└─────────────┬────────────────────┘
              │
          Hardware
```

### Generic Peripheral Driver Template

```c
// Header: uart_driver.h
#ifndef UART_DRIVER_H
#define UART_DRIVER_H

typedef enum {
    UART_OK,
    UART_ERROR,
    UART_TIMEOUT,
    UART_BUSY
} uart_status_t;

typedef struct {
    uint32_t baudrate;
    uint8_t data_bits;
    uint8_t stop_bits;
    uint8_t parity;
    uint32_t rts_buffer_size;
} uart_config_t;

uart_status_t UART_Init(uart_config_t *config);
uart_status_t UART_Transmit(const uint8_t *data, uint16_t length);
uart_status_t UART_Receive(uint8_t *data, uint16_t *length);
uart_status_t UART_RegisterCallback(void (*callback)(uint8_t));
uart_status_t UART_DeInit(void);

#endif
```

---

## Firmware Flashing Procedures

### 1. Flash Memory Layout

```
0x00000000 ┌─────────────────────┐
           │  Bootloader         │ 64KB
0x00010000 ├─────────────────────┤
           │  Application        │ 384KB
0x00070000 ├─────────────────────┤
           │  Configuration      │ 32KB
0x00078000 ├─────────────────────┤
           │  Logs/Telemetry     │ 32KB
0x00080000 └─────────────────────┘
           (Total 512KB example)
```

### 2. Flashing Methods

**A. ST-Link/JTAG Debugger**
```bash
# Using STM32Cube Programmer CLI
STM32_Programmer_CLI -c port=SWD -w flash.bin 0x08000000 -rst

# Using GDB
(gdb) target extended-remote /dev/ttyACM0
(gdb) load firmware.elf
(gdb) continue
```

**B. Serial Bootloader (STM bootloader)**
```bash
# Using UART bootloader
stm32flash -w firmware.bin -g 0x0 /dev/ttyUSB0

# Custom bootloader protocol
python bootloader.py --file firmware.bin --port COM3 --speed 115200
```

**C. USB DFU (Device Firmware Update)**
```bash
dfu-util -a 0 -D firmware.dfu -R
```

### 3. Bootloader Implementation

```c
// bootloader.h - Minimal bootloader
#define APP_START_ADDR 0x08010000
#define APP_SIZE_MAX   (384 * 1024)

typedef int (*main_t)(void);

void Bootloader_Init(void) {
    // Initialize clock, UART (for updates)
}

void Bootloader_CheckUpdate(void) {
    if (is_firmware_update_available()) {
        receive_firmware_over_uart();
        write_to_flash();
        set_app_valid_flag();
    }
}

void Bootloader_JumpToApp(void) {
    if (!is_app_valid()) {
        // Bootloader stays resident OR halt
    }
    
    // Setup app stack pointer
    uint32_t *app_stack = (uint32_t *)APP_START_ADDR;
    __set_MSP(*app_stack);
    
    // Jump to app
    main_t app_main = (main_t)(*(uint32_t *)(APP_START_ADDR + 4));
    app_main();
}
```

---

## Build System Optimization

### CMake-based Build

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(firmware C ASM)

# Set compiler and optimization flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

# Add source files
file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HAL_SOURCES "hal/*.c")

# Create executable
add_executable(firmware.elf ${SOURCES} ${HAL_SOURCES})

# Link libraries
target_link_libraries(firmware.elf m)  # Math library

# Generate binary
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary 
            firmware.elf firmware.bin
    COMMAND arm-none-eabi-size firmware.elf
)
```

### Makefile Build

```makefile
# Makefile (simplified)
TARGET = firmware
BUILD_DIR = build

# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Optimization levels
CFLAGS = -mcpu=cortex-m4 -mfloat-abi=hard
CFLAGS += -O2 -Wall -Wextra
CFLAGS += -ffunction-sections -fdata-sections

# Source files
SOURCES = main.c uart.c i2c.c adc.c
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(CFLAGS) -Wl,--gc-sections -T script.ld -o $@ $^

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	$(SIZE) $<

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

flash:
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x08000000

.PHONY: all clean flash
```

---

## Tool Integration (ENTRAPASS Example)

### ENTRAPASS Integration

ENTRAPASS is a firmware update and configuration management tool.

```python
# entrapass_interface.py
import serial
import struct

class ENTRAPASSClient:
    def __init__(self, port, baudrate=115200):
        self.ser = serial.Serial(port, baudrate)
    
    def enter_bootloader(self):
        """Send command to enter bootloader mode"""
        cmd = b'BOOT'  # Magic command
        self.ser.write(cmd)
        response = self.ser.read(4)
        return response == b'ACKB'
    
    def send_firmware(self, firmware_file):
        """Send firmware file in chunks"""
        with open(firmware_file, 'rb') as f:
            firmware = f.read()
        
        chunk_size = 256
        for i in range(0, len(firmware), chunk_size):
            chunk = firmware[i:i+chunk_size]
            self.ser.write(bytes([len(chunk)]) + chunk)
    
    def verify_firmware(self, crc32):
        """Verify firmware integrity"""
        self.ser.write(b'VRFY')
        received_crc = struct.unpack('>I', self.ser.read(4))[0]
        return received_crc == crc32
    
    def set_live(self):
        """Mark firmware as valid and boot"""
        self.ser.write(b'LIVE')
        return self.ser.read(4) == b'ACKL'

# Usage
client = ENTRAPASSClient('/dev/ttyUSB0')
if client.enter_bootloader():
    client.send_firmware('firmware.bin')
    if client.verify_firmware(expected_crc):
        client.set_live()
```

---

## Code Snippets for Peripherals

### UART Peripheral

```c
#include "uart.h"

void UART_Init(uint32_t baudrate) {
    // Clock enable
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    // Configuration
    USART2->BRR = SystemCoreClock / baudrate;
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
    USART2->CR1 |= USART_CR1_RXNEIE;  // RX interrupt
    
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) {
        uint8_t data = USART2->DR;
        // Process received data
    }
}
```

### ADC Peripheral

```c
void ADC_Init(void) {
    // Clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    
    // Configuration
    ADC1->CR2 = ADC_CR2_ADON;
    ADC1->SMPR2 = ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP0_1;
    ADC1->SQR3 = 0;  // Channel 0
    
    // Calibration
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL);
}

uint16_t ADC_Read(uint32_t channel) {
    ADC1->SQR3 = channel;
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while (!(ADC1->SR & ADC_SR_EOC));
    return ADC1->DR;
}
```

---

## Interview Q&A

### Q1: Explain firmware update process and safety measures.

**Answer**:
```
Safe firmware update sequence:

1. Signature verification
   - Check firmware digital signature
   - Prevent corrupted/malicious firmware

2. Checksum/CRC validation
   - Calculate CRC of firmware before write
   - Verify after write to flash

3. Version checking
   - Prevent downgrade attacks
   - Ensure compatibility

4. Dual-bank update (Optional)
   - Keep bootloader valid always
   - Switch after successful boot

5. Watchdog protection
   - If boot fails, fallback to previous
```

### Q2: How handle firmware update during critical operation?

**Answer**:
```c
#define SAFE_UPDATE_WINDOW_HOURS 2  // 2:00 AM
#define SAFE_STATE_CHECK()         (system_idle && power_stable)

bool FirmwareUpdate_CanProceed(void) {
    if (!SAFE_STATE_CHECK()) {
        schedule_update_for_later(next_safe_window());
        return false;
    }
    
    // Ensure battery/power safe
    if (battery_voltage < SAFE_THRESHOLD || on_battery_power)
        return false;
    
    // Stop critical operations
    stop_measurement();
    close_file_handles();
    flush_buffers();
    
    return true;
}
```

### Q3: Design recovery mechanism for failed firmware update.

**Answer**:
```c
// Store critical data before update
typedef struct {
    uint32_t boot_count;
    uint32_t last_good_crc;
    uint32_t app_start_addr;
} boot_info_t;

void FirmwareUpdate_WithRecovery(void) {
    boot_info_t info;
    info.boot_count++;
    
    if (info.boot_count > MAX_BOOT_TRIES) {
        // Rollback to previous
        bootloader_install_backup();
        info.boot_count = 0;
        return;
    }
    
    save_boot_info(&info);
    perform_update();
    
    // If we reach here, update successful
    info.boot_count = 0;
    save_boot_info(&info);
}
```

---

**End of Peripherals & Firmware Guide**
