# STM32 Bootloader Design - Step by Step Guide

## Table of Contents
1. [Introduction](#introduction)
2. [Memory Layout](#memory-layout)
3. [Bootloader Architecture](#bootloader-architecture)
4. [Step-by-Step Implementation](#step-by-step-implementation)
5. [Complete Example Code](#complete-example-code)
6. [Host Communication Protocol](#host-communication-protocol)

---

## Introduction

A bootloader is a small program that runs before the main application. It performs:
- Hardware initialization
- Firmware upgrade/flashing
- Validation of application
- Transfer control to application

### Key Features for STM32 Bootloader:
- UART/Serial communication
- Flash memory programming
- CRC validation
- Application jumping
- Timeout handling for firmware update mode

---

## Memory Layout

Standard STM32F4 Memory Layout (512KB Flash):

```
Flash Memory (512KB):
├── 0x0800_0000 - 0x0800_4000: Bootloader (16KB)
├── 0x0800_4000 - 0x0807_0000: Application (432KB)
└── 0x0807_0000 - 0x0808_0000: Configuration/Reserved (64KB)

SRAM Memory (192KB):
├── 0x2000_0000 - 0x2000_FFFF: Application RAM
└── 0x2001_0000 - 0x2003_0000: Extended RAM
```

---

## Bootloader Architecture

```
┌─────────────────────────────────────┐
│     STM32 Power On / Reset          │
└────────────┬────────────────────────┘
             │
┌────────────▼────────────────────────┐
│   Bootloader Initialization          │
│   - Clock Setup                      │
│   - GPIO/UART Config                │
│   - Flash Unlock                     │
└────────────┬────────────────────────┘
             │
┌────────────▼────────────────────────┐
│   Check Boot Mode                    │
│   - Timeout/Button Press?            │
│   - Valid App Signature?             │
└────────────┬────────────────────────┘
             │
      ┌──────┴──────┐
      │             │
   YES│             │NO
      │             │
┌─────▼────┐  ┌────▼──────────────┐
│Firmware   │  │Jump to Application│
│Update     │  │at 0x0800_4000     │
│Protocol   │  └───────────────────┘
└──────────┘
```

---

## Step-by-Step Implementation

### Step 1: Linker Script Configuration

Create `stm32f407_bootloader.ld`:

```ld
/* Bootloader Linker Script */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 16K   /* Bootloader */
  FLASH_APP (rx)  : ORIGIN = 0x08004000, LENGTH = 432K  /* Application */
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 192K  /* SRAM */
}

SECTIONS
{
  .text :
  {
    KEEP(*(.vectors))
    *(.text)
    *(.text*)
    *(.rodata)
    *(.rodata*)
  } > FLASH

  .data :
  {
    *(.data)
    *(.data*)
  } > RAM AT > FLASH

  .bss :
  {
    *(.bss)
    *(.bss*)
  } > RAM

  _sidata = LOADADDR(.data);
  _sdata = ADDR(.data);
  _edata = ADDR(.data) + SIZEOF(.data);
}
```

### Step 2: Bootloader Header Files

**bootloader.h:**

```c
#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>
#include <stdbool.h>

/* Memory Definitions */
#define BOOTLOADER_START     0x08000000
#define APPLICATION_START    0x08004000
#define APP_SIZE             (432 * 1024)
#define APP_END              (APPLICATION_START + APP_SIZE)

/* Bootloader Commands */
#define BL_CMD_ERASE         0x01
#define BL_CMD_WRITE         0x02
#define BL_CMD_READ          0x03
#define BL_CMD_VERIFY        0x04
#define BL_CMD_JUMP          0x05
#define BL_CMD_GET_STATUS    0x06

/* Response Codes */
#define BL_OK                0x00
#define BL_ERROR             0xFF
#define BL_BUSY              0x01

/* Application Signature */
#define APP_SIGNATURE        0xABCDEF12
#define SIGNATURE_OFFSET     4

/* Function Prototypes */
void bootloader_init(void);
void bootloader_main(void);
bool is_app_valid(void);
void jump_to_application(void);
void uart_init(void);
void uart_write(uint8_t data);
uint8_t uart_read(void);
void flash_erase_sector(uint32_t sector);
bool flash_write(uint32_t address, uint8_t *data, uint32_t size);
uint16_t calculate_crc16(uint8_t *data, uint32_t size);

#endif
```

### Step 3: UART Communication Functions

**uart.c:**

```c
#include "stm32f4xx.h"
#include "bootloader.h"

/* Use USART1: PA9(TX), PA10(RX) */

void uart_init(void)
{
    /* Enable GPIOA and USART1 clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /* PA9 Alternative Function 7 (USART1_TX) */
    GPIOA->MODER |= (2 << (9 * 2));
    GPIOA->AFRH |= (7 << (1 * 4));
    GPIOA->OSPEEDR |= (3 << (9 * 2));

    /* PA10 Alternative Function 7 (USART1_RX) */
    GPIOA->MODER |= (2 << (10 * 2));
    GPIOA->AFRH |= (7 << (2 * 4));
    GPIOA->OSPEEDR |= (3 << (10 * 2));

    /* Configure USART1: 115200 baud, 8-bit, no parity */
    USART1->BRR = (42 << 4) | 9; /* 84MHz / (16 * 115200) ≈ 45.57 = 45 + 9/16 */
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void uart_write(uint8_t data)
{
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = data;
}

uint8_t uart_read(void)
{
    while (!(USART1->SR & USART_SR_RXNE));
    return USART1->DR;
}

void uart_write_string(const char *str)
{
    while (*str)
        uart_write(*str++);
}

void uart_write_hex(uint32_t val)
{
    for (int i = 7; i >= 0; i--) {
        uint8_t nibble = (val >> (i * 4)) & 0x0F;
        uart_write((nibble < 10) ? (nibble + '0') : (nibble + 'A' - 10));
    }
}
```

### Step 4: Flash Programming Functions

**flash.c:**

```c
#include "stm32f4xx.h"
#include "bootloader.h"

#define FLASH_KEY1  0x45670123
#define FLASH_KEY2  0xCDEF89AB

void flash_unlock(void)
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

void flash_lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}

bool flash_erase_sector(uint32_t sector_num)
{
    if (sector_num < 4) return false;  /* Don't erase bootloader */

    flash_unlock();

    /* Wait for flash to be ready */
    while (FLASH->SR & FLASH_SR_BSY);

    /* Clear all flags */
    FLASH->SR = 0xF3;

    /* Configure erase operation */
    FLASH->CR = FLASH_CR_EOPIE | (sector_num << 3) | FLASH_CR_SER;
    FLASH->CR |= FLASH_CR_STRT;

    /* Wait for operation to complete */
    while (FLASH->SR & FLASH_SR_BSY);

    /* Clear SER bit */
    FLASH->CR &= ~FLASH_CR_SER;

    bool result = !(FLASH->SR & (FLASH_SR_PGSERR | FLASH_SR_PGERR));

    flash_lock();

    return result;
}

bool flash_write(uint32_t address, uint8_t *data, uint32_t size)
{
    if (address < APPLICATION_START || (address + size) > APP_END) {
        return false;
    }

    flash_unlock();

    /* Set 8-bit programming mode */
    FLASH->CR = (FLASH_CR_CR_PSIZE_0) | FLASH_CR_PG;

    for (uint32_t i = 0; i < size; i++) {
        /* Wait for ready */
        while (FLASH->SR & FLASH_SR_BSY);

        /* Write byte */
        *(volatile uint8_t *)(address + i) = data[i];

        /* Check for EOP */
        while (FLASH->SR & FLASH_SR_BSY);

        if (FLASH->SR & (FLASH_SR_PGSERR | FLASH_SR_PGERR)) {
            flash_lock();
            return false;
        }
    }

    FLASH->CR &= ~FLASH_CR_PG;
    flash_lock();

    return true;
}

bool flash_verify(uint32_t address, uint8_t *data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        if (*(volatile uint8_t *)(address + i) != data[i]) {
            return false;
        }
    }
    return true;
}

uint16_t calculate_crc16(uint8_t *data, uint32_t size)
{
    uint16_t crc = 0xFFFF;

    for (uint32_t i = 0; i < size; i++) {
        crc ^= data[i] << 8;
        for (int j = 0; j < 8; j++) {
            crc = (crc & 0x8000) ? ((crc << 1) ^ 0x1021) : (crc << 1);
            crc &= 0xFFFF;
        }
    }

    return crc;
}
```

### Step 5: Application Jump Function

**startup.c - Application Jump:**

```c
#include "stm32f4xx.h"
#include "bootloader.h"

/* Vector table in application */
typedef struct {
    uint32_t *sp;
    void (*reset_handler)(void);
} app_vector_t;

bool is_app_valid(void)
{
    /* Check if stack pointer is valid (must be in SRAM) */
    uint32_t stack_ptr = *(uint32_t *)APPLICATION_START;
    if (stack_ptr < 0x20000000 || stack_ptr > 0x20030000) {
        return false;
    }

    /* Check signature stored at offset 4 */
    uint32_t signature = *(uint32_t *)(APPLICATION_START + SIGNATURE_OFFSET);
    if (signature != APP_SIGNATURE) {
        return false;
    }

    /* Verify CRC if stored at offset 8 */
    uint32_t stored_crc = *(uint32_t *)(APPLICATION_START + 8);
    uint16_t calculated_crc = calculate_crc16(
        (uint8_t *)(APPLICATION_START + 16),
        APP_SIZE - 16
    );

    return (stored_crc & 0xFFFF) == calculated_crc;
}

void jump_to_application(void)
{
    if (!is_app_valid()) {
        uart_write_string("\nERROR: Invalid application!\n");
        return;
    }

    /* Disable all interrupts */
    __disable_irq();

    /* Deinitialize HAL */
    HAL_DeInit();

    /* Disable systick */
    SysTick->CTRL = 0;

    /* Get application vector table */
    uint32_t app_stack = *(uint32_t *)APPLICATION_START;
    uint32_t app_reset = *(uint32_t *)(APPLICATION_START + 4);

    /* Set MSP (Main Stack Pointer) */
    __set_MSP(app_stack);

    /* Set VTOR to application start */
    SCB->VTOR = APPLICATION_START;

    /* Jump to application reset handler */
    asm volatile ("BX %0" : : "r"(app_reset));
}
```

### Step 6: Bootloader Main Logic

**bootloader.c:**

```c
#include "stm32f4xx.h"
#include "bootloader.h"

volatile uint32_t bootloader_timeout = 0;
#define BOOTLOADER_WAIT_TIME 5000  /* 5 seconds */

void SysTick_Handler(void)
{
    bootloader_timeout++;
}

void systick_init(void)
{
    SysTick_Config(SystemCoreClock / 1000);  /* 1ms tick */
}

void bootloader_init(void)
{
    /* Initialize system */
    SystemInit();
    systick_init();
    uart_init();

    uart_write_string("\n================================\n");
    uart_write_string("STM32F407 Bootloader v1.0\n");
    uart_write_string("================================\n");
}

void bootloader_main(void)
{
    bootloader_init();

    /* Check for firmware update request */
    uint32_t start_time = bootloader_timeout;
    bool update_mode = false;

    uart_write_string("\nWaiting for character to enter update mode...\n");
    uart_write_string("Timeout in 5 seconds\n");

    while ((bootloader_timeout - start_time) < BOOTLOADER_WAIT_TIME) {
        if (USART1->SR & USART_SR_RXNE) {
            uart_read();
            update_mode = true;
            break;
        }
    }

    if (update_mode) {
        uart_write_string("\nEntering Bootloader Update Mode\n");
        bootloader_update_protocol();
    } else {
        uart_write_string("\nJumping to Application\n");
        jump_to_application();
    }

    /* Should never reach here */
    while (1);
}

void bootloader_update_protocol(void)
{
    uint8_t cmd;
    uint32_t address, size;
    uint8_t buffer[256];

    uart_write_string("Ready for firmware update\n");
    uart_write_string("Commands: A=Erase, W=Write, V=Verify, J=Jump, S=Status\n");

    while (1) {
        uart_write_string("> ");
        cmd = uart_read();
        uart_write(cmd);
        uart_write('\n');

        switch (cmd) {
        case 'A':
        case 'a':
            uart_write_string("Erasing application area...\n");
            for (int sector = 4; sector <= 11; sector++) {
                if (flash_erase_sector(sector)) {
                    uart_write_string(".");
                } else {
                    uart_write_string("E");
                }
            }
            uart_write_string("\nErase completed\n");
            break;

        case 'W':
        case 'w':
            uart_write_string("Enter start address (hex): 0x");
            address = read_hex_number(8);
            uart_write_string("\nEnter size (hex): 0x");
            size = read_hex_number(8);
            uart_write_string("\nSend data...\n");

            for (uint32_t i = 0; i < size; i++) {
                buffer[i % 256] = uart_read();
                if ((i + 1) % 256 == 0) {
                    flash_write(address + i - 255, buffer, 256);
                    uart_write_string(".");
                }
            }
            if (size % 256) {
                flash_write(address + size - (size % 256), buffer, size % 256);
            }
            uart_write_string("\nWrite completed\n");
            break;

        case 'V':
        case 'v':
            uart_write_string("Verifying application...\n");
            if (is_app_valid()) {
                uart_write_string("Application is VALID\n");
            } else {
                uart_write_string("Application is INVALID\n");
            }
            break;

        case 'J':
        case 'j':
            uart_write_string("Jumping to application...\n");
            jump_to_application();
            break;

        case 'S':
        case 's':
            uart_write_string("App Start: 0x");
            uart_write_hex(APPLICATION_START);
            uart_write_string("\nApp Size: 0x");
            uart_write_hex(APP_SIZE);
            uart_write('\n');
            break;

        default:
            uart_write_string("Unknown command\n");
        }
    }
}

uint32_t read_hex_number(uint8_t digits)
{
    uint32_t result = 0;
    for (int i = 0; i < digits; i++) {
        uint8_t c = uart_read();
        uart_write(c);
        result = result * 16;
        if (c >= '0' && c <= '9') {
            result += (c - '0');
        } else if (c >= 'A' && c <= 'F') {
            result += (c - 'A' + 10);
        } else if (c >= 'a' && c <= 'f') {
            result += (c - 'a' + 10);
        }
    }
    return result;
}

void main(void)
{
    bootloader_main();
}
```

### Step 7: Application Code Template

**user_app.c:**

```c
#include "stm32f4xx.h"

/* Application must be at 0x08004000 */
void app_main(void)
{
    /* Initialize system */
    SystemInit();

    /* Your application code here */
    /* Example: Blink LED on PD12 */

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER |= (1 << (12 * 2));  /* Output mode */

    while (1) {
        GPIOD->ODR |= (1 << 12);   /* LED ON */
        for (volatile int i = 0; i < 1000000; i++);

        GPIOD->ODR &= ~(1 << 12);  /* LED OFF */
        for (volatile int i = 0; i < 1000000; i++);
    }
}

/* Application startup code */
void Reset_Handler(void)
{
    app_main();
}
```

---

## Complete Example Code

### stm32f4xx startup vector table:

```c
extern void Reset_Handler(void);
extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);

__attribute__((section(".vectors")))
void (*vector_table[])(void) = {
    (void (*)())0x20000000,      /* Stack pointer (SRAM address) */
    Reset_Handler,               /* Reset handler */
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    /* Remaining ISRs... */
};
```

---

## Host Communication Protocol

### Serial Protocol Format:

```
Packet Structure:
┌──────────┬──────────┬──────────┬──────────────┬──────────┐
│ CMD (1B) │ ADDR(4B) │ SIZE(4B) │ DATA(N bytes)│ CRC(2B)  │
└──────────┴──────────┴──────────┴──────────────┴──────────┘

Example UART Commands:

1. Erase Flash:
   Request:  0x01 0x08004000 0x00000000 CRC16
   Response: 0x00 (Success) or 0xFF (Error)

2. Write Flash:
   Request:  0x02 0x08004000 0x00001000 [1024 bytes of data] CRC16
   Response: 0x00 (Success) or 0xFF (Error)

3. Verify:
   Request:  0x03 0x08004000 0x00001000
   Response: 0x00 (Valid) or 0xFF (Invalid)

4. Jump to App:
   Request:  0x04
   Response: Device jumps to application (no response)
```

### Python Host Utility:

```python
import serial
import struct

class STM32Bootloader:
    def __init__(self, port, baudrate=115200):
        self.serial = serial.Serial(port, baudrate, timeout=1)

    def send_command(self, cmd, address=0, data=None):
        if data is None:
            data = b''

        size = len(data)
        packet = struct.pack('>BIH', cmd, address, size) + data

        # Calculate CRC16
        crc = self.calc_crc16(packet)
        packet += struct.pack('>H', crc)

        self.serial.write(packet)
        return self.read_response()

    def calc_crc16(self, data):
        crc = 0xFFFF
        for byte in data:
            crc ^= byte << 8
            for _ in range(8):
                crc = ((crc << 1) ^ 0x1021) if (crc & 0x8000) else (crc << 1)
                crc &= 0xFFFF
        return crc

    def read_response(self):
        response = self.serial.read(1)
        return response[0] if response else None

    def erase_flash(self):
        return self.send_command(0x01) == 0x00

    def write_flash(self, address, data):
        return self.send_command(0x02, address, data) == 0x00

    def verify(self, address):
        return self.send_command(0x03, address) == 0x00

    def jump_to_app(self):
        self.send_command(0x04)
```

---

## Key Considerations

1. **Memory Protection**: Always protect bootloader from accidental erase
2. **CRC/Checksum**: Verify application integrity before jumping
3. **Timeout**: Implement mechanism to enter update mode
4. **Error Handling**: Handle all flash programming errors
5. **Watchdog**: Consider WDT for firmware update monitoring
6. **Security**: Add signature verification for critical systems

---

## Testing Steps

1. **Build bootloader and program to device**
   ```bash
   arm-none-eabi-gcc -mcpu=cortex-m4 -c bootloader.c -o bootloader.o
   arm-none-eabi-gcc -mcpu=cortex-m4 bootloader.o -T stm32f407_bootloader.ld -o bootloader.elf
   ```

2. **Create firmware binary with application signature**
3. **Use host utility to flash application via UART**
4. **Verify application is recognized and executed**

---

