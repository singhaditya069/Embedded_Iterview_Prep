# Board Support Package (BSP) for KT10 Controller - Complete Implementation Guide

## Project Overview

This project covers developing a Board Support Package for the KT10 microcontroller. A BSP includes hardware initialization, peripheral drivers, and board-specific configurations that abstract hardware details from applications.

---

## BSP Architecture

```
┌──────────────────────────┐
│   Application Layer      │
│   (User Code)            │
└────────────┬─────────────┘
             │
┌────────────▼─────────────────────────────┐
│  Board Support Package (BSP)             │
│  • Board Initialization                  │
│  • Clock Configuration                   │
│  • Memory Layout                         │
│  • Pin Configuration                     │
│  • Peripheral Mappings                   │
└────────────┬─────────────────────────────┘
             │
┌────────────▼─────────────────────────────┐
│  Hardware Abstraction Layer (HAL)        │
│  • GPIO, UART, SPI, I2C, ADC             │
│  • Timer, Watchdog                       │
│  • Interrupt Management                  │
└────────────┬─────────────────────────────┘
             │
          Real Hardware (KT10)
```

---

## KT10 Controller Overview

### Specifications

| Parameter | Value |
|-----------|-------|
| **MCU** | ARM Cortex-M4F |
| **Flash** | 512KB |
| **RAM** | 128KB |
| **Clock** | Up to 168MHz |
| **GPIO** | 100+ pins |
| **Peripherals** | UART (4), SPI (3), I2C (2), ADC (16ch), CAN |
| **Package** | LQFP144 |

### Pin Configuration

```
KT10 Controller Pinout (Partial):

LQFP144 Package
          PA0  PA1  PA2  PA3
         ┌─────────────────────┐
    PB9  │  PA0····ADC0      PA3│ PC0
    PB8  │  PA1·········PC1    │ PC1
    PB7  │  PA2········SDA     │ PC2
    GND  │  PA3········SCL     │ PC3
         │                     │
         │  [KT10 Controller]  │
         │                     │
         └─────────────────────┘
```

---

## BSP Implementation

### Header: bsp_init.h

```c
#ifndef BSP_INIT_H
#define BSP_INIT_H

#include <stdint.h>
#include <stdbool.h>

/* Memory Layout */
#define FLASH_START         0x08000000
#define FLASH_SIZE          (512 * 1024)  // 512KB
#define RAM_START           0x20000000
#define RAM_SIZE            (128 * 1024)  // 128KB

/* Clock Configuration */
#define SYSTEM_CLOCK_MHz    168
#define APB1_CLOCK_MHz      42
#define APB2_CLOCK_MHz      84

/* GPIO Pin Definitions */
#define GPIO_UART1_TX       PA9
#define GPIO_UART1_RX       PA10
#define GPIO_UART2_TX       PA2
#define GPIO_UART2_RX       PA3
#define GPIO_SPI1_CLK       PA5
#define GPIO_SPI1_MOSI      PA7
#define GPIO_SPI1_MISO      PA6
#define GPIO_I2C1_SDA       PB9
#define GPIO_I2C1_SCL       PB8

/* LED Pin Definitions */
#define GPIO_LED_RED        PD12
#define GPIO_LED_GREEN      PD13
#define GPIO_LED_BLUE       PD14

/* Button Pin Definitions */
#define GPIO_BUTTON_1       PA0
#define GPIO_BUTTON_2       PA1

/* ADC Channel Mappings */
#define ADC_CH_SENSOR_1     ADC_CHANNEL_0
#define ADC_CH_SENSOR_2     ADC_CHANNEL_1
#define ADC_CH_BATTERY      ADC_CHANNEL_4

/* Status Codes */
typedef enum {
    BSP_OK = 0,
    BSP_ERROR,
    BSP_INIT_FAILED,
    BSP_CLOCK_ERROR
} bsp_status_t;

/* Initialization Functions */
bsp_status_t BSP_Init(void);
bsp_status_t BSP_ClockInit(void);
bsp_status_t BSP_GPIOInit(void);
bsp_status_t BSP_PeripheralInit(void);
bsp_status_t BSP_SystemTickInit(void);

#endif /* BSP_INIT_H */
```

### Source: bsp_init.c

```c
#include "bsp_init.h"
#include "stm32f4xx.h"
#include "hal_clock.h"
#include "hal_gpio.h"

/**
 * @brief System clock configuration
 * Sets up PLL for 168MHz operation from 8MHz external oscillator
 */
bsp_status_t BSP_ClockInit(void)
{
    // Enable external oscillator (HSE)
    RCC->CR |= RCC_CR_HSEON;
    
    // Wait for HSE ready
    uint32_t timeout = 10000;
    while (!(RCC->CR & RCC_CR_HSERDY) && timeout--);
    if (!timeout) return BSP_CLOCK_ERROR;
    
    // Configure PLL
    // SYSCLK = (HSE / M) * N / P = (8 / 8) * 336 / 2 = 168MHz
    RCC->PLLCFGR = (8 << 0) |           // M = 8
                   (336 << 6) |         // N = 336
                   (0 << 16) |          // P = 2 (00 binary)
                   (0 << 28);           // Q = 7 (0111 binary)
    
    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    
    // Wait for PLL ready
    timeout = 10000;
    while (!(RCC->CR & RCC_CR_PLLRDY) && timeout--);
    if (!timeout) return BSP_CLOCK_ERROR;
    
    // Configure bus dividers
    RCC->CFGR = (0 << 0) |              // AHB prescaler = 1
                (4 << 10) |             // APB1 prescaler = 4 (42MHz)
                (2 << 13);              // APB2 prescaler = 2 (84MHz)
    
    // Switch to PLL
    RCC->CFGR |= (2 << 0);  // SW = 10 (PLL)
    
    return BSP_OK;
}

/**
 * @brief GPIO initialization for all board peripherals
 */
bsp_status_t BSP_GPIOInit(void)
{
    // Enable GPIO clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN |
                    RCC_AHB1ENR_GPIOBEN |
                    RCC_AHB1ENR_GPIOCEN |
                    RCC_AHB1ENR_GPIODEN;
    
    // Configure LEDs (PD12-PD14)
    gpio_config_t led_cfg = {
        .pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14,
        .mode = GPIO_MODE_OUTPUT,
        .speed = GPIO_SPEED_MEDIUM,
        .pull = GPIO_PULL_NONE
    };
    GPIO_Init(GPIOD, &led_cfg);
    
    // Configure Buttons (PA0, PA1)
    gpio_config_t btn_cfg = {
        .pin = GPIO_PIN_0 | GPIO_PIN_1,
        .mode = GPIO_MODE_INPUT,
        .pull = GPIO_PULL_UP
    };
    GPIO_Init(GPIOA, &btn_cfg);
    
    // Configure UART1 pins (PA9-TX, PA10-RX)
    gpio_config_t uart_cfg = {
        .pin = GPIO_PIN_9 | GPIO_PIN_10,
        .mode = GPIO_MODE_ALTERNATE,
        .alternate = GPIO_AF7_USART1,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_UP
    };
    GPIO_Init(GPIOA, &uart_cfg);
    
    // Configure SPI1 pins (PA5-CLK, PA6-MISO, PA7-MOSI)
    gpio_config_t spi_cfg = {
        .pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        .mode = GPIO_MODE_ALTERNATE,
        .alternate = GPIO_AF5_SPI1,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_NONE
    };
    GPIO_Init(GPIOA, &spi_cfg);
    
    // Configure I2C1 pins (PB8-SCL, PB9-SDA)
    gpio_config_t i2c_cfg = {
        .pin = GPIO_PIN_8 | GPIO_PIN_9,
        .mode = GPIO_MODE_ALTERNATE,
        .alternate = GPIO_AF4_I2C1,
        .speed = GPIO_SPEED_HIGH,
        .pull = GPIO_PULL_UP
    };
    GPIO_Init(GPIOB, &i2c_cfg);
    
    return BSP_OK;
}

/**
 * @brief Initialize all board peripherals
 */
bsp_status_t BSP_PeripheralInit(void)
{
    // Initialize UART
    uart_config_t uart_cfg = {
        .baudrate = 115200,
        .data_bits = 8,
        .stop_bits = 1,
        .parity = UART_PARITY_NONE
    };
    UART_Init(UART1, &uart_cfg);
    
    // Initialize SPI
    spi_config_t spi_cfg = {
        .mode = SPI_MODE_MASTER,
        .clock_polarity = SPI_CPOL_LOW,
        .clock_phase = SPI_CPHA_1EDGE,
        .baudrate_prescaler = SPI_BAUDRATE_DIV_4,
        .data_size = SPI_DATA_8BIT
    };
    SPI_Init(SPI1, &spi_cfg);
    
    // Initialize I2C
    i2c_config_t i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .clock_speed = 400000
    };
    I2C_Init(I2C1, &i2c_cfg);
    
    // Initialize ADC
    adc_config_t adc_cfg = {
        .resolution = ADC_RESOLUTION_12BIT,
        .data_alignment = ADC_ALIGN_RIGHT,
        .scan_direction = ADC_SCAN_FORWARD
    };
    ADC_Init(&adc_cfg);
    
    return BSP_OK;
}

/**
 * @brief System tick initialization (1ms interrupts)
 */
bsp_status_t BSP_SystemTickInit(void)
{
    // Configure SysTick: 1ms interrupts
    uint32_t ticks = SYSTEM_CLOCK_MHz * 1000000 / 1000;  // 168M / 1000
    
    SysTick->LOAD = ticks - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
    
    NVIC_SetPriority(SysTick_IRQn, 15);
    
    return BSP_OK;
}

/**
 * @brief Main BSP initialization
 */
bsp_status_t BSP_Init(void)
{
    // Initialize clock first
    if (BSP_ClockInit() != BSP_OK) {
        return BSP_ERROR;
    }
    
    // Initialize GPIO
    if (BSP_GPIOInit() != BSP_OK) {
        return BSP_ERROR;
    }
    
    // Initialize peripherals
    if (BSP_PeripheralInit() != BSP_OK) {
        return BSP_ERROR;
    }
    
    // Initialize system tick
    if (BSP_SystemTickInit() != BSP_OK) {
        return BSP_ERROR;
    }
    
    return BSP_OK;
}
```

### Usage Example

```c
#include "bsp_init.h"
#include "stdio.h"

int main(void)
{
    // Initialize board
    if (BSP_Init() != BSP_OK) {
        // Hardware initialization failed
        while (1);
    }
    
    // Now can use all peripherals
    printf("KT10 Board Initialized\n");
    printf("System Clock: %d MHz\n", SYSTEM_CLOCK_MHz);
    printf("APB1 Clock: %d MHz\n", APB1_CLOCK_MHz);
    printf("APB2 Clock: %d MHz\n", APB2_CLOCK_MHz);
    
    // Toggle LED
    GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);  // RED LED on
    
    // Read button
    uint8_t button = GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    
    // Use UART, SPI, I2C, ADC...
    
    return 0;
}
```

---

## Memory Management

### Linker Script Excerpt

```ld
/* Linker script for KT10 */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
  /* Code section */
  .text :
  {
    KEEP(*(.vectors))
    *(.text*)
  } > FLASH

  /* Read-only data */
  .rodata :
  {
    *(.rodata*)
  } > FLASH

  /* Initialized data (stored in FLASH, copied to RAM) */
  .data :
  {
    *(.data*)
  } > RAM AT > FLASH

  /* Uninitialized data */
  .bss :
  {
    *(.bss*)
    *(COMMON)
  } > RAM

  _end = .;
}
```

---

## Startup Code

### Startup Assembly: startup_kt10.s

```asm
; Vector table
__Vectors       DCD     __initial_sp          ; Stack pointer
                DCD     Reset_Handler         ; Reset
                DCD     NMI_Handler           ; NMI
                DCD     HardFault_Handler     ; Hard fault
                ; ... other exceptions ...

; Reset handler
Reset_Handler   PROC
                ; Copy initialized data from FLASH to RAM
                LDR     R0, =__data_start
                LDR     R1, =__data_load
                LDR     R2, =__data_end
                
CopyLoop        CMP     R0, R2
                BEQ     CopyDone
                LDR     R3, [R1], #4
                STR     R3, [R0], #4
                B       CopyLoop

CopyDone        ; Zero BSS
                LDR     R0, =__bss_start
                LDR     R1, =__bss_end
                MOV     R2, #0

ZeroLoop        CMP     R0, R1
                BEQ     ZeroDone
                STR     R2, [R0], #4
                B       ZeroLoop

ZeroDone        ; Call main
                BL      main
                B       .
                ENDP
```

---

## Interview Q&A

### Q1: Component selection for KT10 BSP design.

**Answer**:

**Peripheral Selection**:
- **UART**: For debugging and communication
- **SPI**: Fast peripheral access (EEPROM, sensors)
- **I2C**: Multiple sensor integration
- **ADC**: Analog input sampling
- **GPIO**: Direct pin control
- **Timer**: System timing and PWM

**Clock Strategy**:
- Use internal oscillator for development
- External oscillator for production (better accuracy)
- PLL multiplication for required frequency
- APB prescaling for peripheral timing

### Q2: Handle hardware revision differences in single BSP.

**Answer**:
```c
// Hardware version detection
typedef enum {
    HW_REV_A = 0,
    HW_REV_B = 1,
    HW_REV_C = 2
} hw_version_t;

hw_version_t detect_hardware(void) {
    // Read hardware ID pins
    uint8_t id_pins = (GPIO_ReadPin(PB0) << 0) |
                      (GPIO_ReadPin(PB1) << 1) |
                      (GPIO_ReadPin(PB2) << 2);
    return (hw_version_t)id_pins;
}

void bsp_init_variant(hw_version_t version) {
    switch (version) {
        case HW_REV_A:
            gpio_led_pin = PD12;  // Old design
            break;
        case HW_REV_B:
            gpio_led_pin = PE0;   // New design
            break;
        default:
            break;
    }
}
```

### Q3: Design extensibility for future KT10 variants.

**Answer**:
```c
// Use feature flags
#define FEATURE_CAN         (1 << 0)
#define FEATURE_ETH         (1 << 1)
#define FEATURE_USB         (1 << 2)

typedef struct {
    uint32_t flash_size;
    uint32_t ram_size;
    uint32_t features;
    const char *variant_name;
} bsp_config_t;

const bsp_config_t kt10_std = {
    .flash_size = 512 * 1024,
    .ram_size = 128 * 1024,
    .features = FEATURE_CAN,
    .variant_name = "KT10-STD"
};

const bsp_config_t kt10_plus = {
    .flash_size = 1024 * 1024,
    .ram_size = 256 * 1024,
    .features = FEATURE_CAN | FEATURE_ETH | FEATURE_USB,
    .variant_name = "KT10-PLUS"
};

// Runtime selection
bsp_status_t bsp_init_variant(bsp_config_t *config) {
    g_bsp_config = config;
    // Configure accordingly
}
```

---

## Testing BSP

```c
// bsp_selftest.c
void BSP_SelfTest(void) {
    // Test GPIO
    GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
    GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
    
    // Test ADC
    uint16_t adc_val = ADC_Read(ADC_CH_SENSOR_1);
    TEST_ASSERT(adc_val < 4096);
    
    // Test UART loopback
    uint8_t data[] = "HELLO";
    UART_Transmit(UART1, data, 5);
    uint8_t received[5];
    UART_Receive(UART1, received, 5, 1000);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(data, received, 5);
}
```

---

**End of Board Support Package Guide**

---

## Comprehensive Projects Summary

All 10 embedded systems projects have been documented in detail:

1. ✅ SPI EEPROM Communication
2. ✅ Dynamic Logging System
3. ✅ PIR Sensor & ADC Integration
4. ✅ IO Expander for Relay & Lock Control
5. ✅ Bug Fixes & Debugging
6. ✅ Peripherals, Firmware, Build System
7. ✅ ATE Commands & Testing
8. ✅ Unit Testing & Static Analysis
9. ✅ Yocto Build System & GitHub Actions
10. ✅ Board Support Package for KT10

**Each project includes**:
- Complete theory and architecture
- Full code implementations
- Header files with detailed documentation
- Practical examples and usage
- Advanced features and best practices
- Interview Q&A with detailed answers
- Common pitfalls and solutions

Perfect for interview preparation!
