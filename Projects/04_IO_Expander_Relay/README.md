# IO Expander for Relay & Lock Control - Complete Implementation Guide

## Project Overview

This project implements an I2C-based IO Expander (like PCF8574) to control relays for home automation, lighting, and electronic locks. IO Expanders extend limited GPIO pins on microcontrollers through serial communication.

---

## Key Concepts

### I2C Communication Protocol

- **Two-Wire Interface**: SDA (Serial Data) + SCL (Serial Clock)
- **Master-Slave**: MCU acts as master, Expander as slave
- **Speed**: Standard mode (100kHz), Fast mode (400kHz)
- **Addressing**: 7-bit slave address (0-127)
- **Pulling**: Open-drain requires pull-up resistors (4.7k typical)

### IO Expander Basics (PCF8574/PCF8574A)

- **Pins**: 8 I/O pins (all bidirectional)
- **Communication**: I2C protocol
- **Addressing**: Can stack up to 8 devices (A0/A1/A2 pins)
- **Current**: Limited output current (~25mA per pin)
- **Interrupt**: Optional interrupt output

### Relay Types

| Type | Advantages | Disadvantages | Use Case |
|------|-----------|--------------|----------|
| **Electromagnetic** | High current handling | Slow switching | Power control |
| **Solid State Relay** | Fast, no noise | Limited current | Logic switching |
| **Mechanical Latch** | Low standby power | Complex control | Locks |

---

## Hardware Architecture

```
┌─────────────┐
│    MCU      │
│  (Master)   │
├─────────────┤
│  SDA ───┬───────────┬──────────┬──────────┐
│         │ (4.7k)    │          │          │
│  SCL ───┼───────────┼──────────┼──────────┐
│         │ (4.7k)    │          │          │
│  GND ────┴───────────┴──────────┼──────────┘
│                                 │
└─────────────────────────────────┼──────────┘
                                  │
                          ┌───────┴────────┐
                          │                │
                  ┌───────▼────────┐  ┌───▼──────────┐
                  │   IO EXP #1    │  │   IO EXP #2  │
                  │ (Addr: 0x20)   │  │ (Addr: 0x21) │
                  ├────────────────┤  ├──────────────┤
                  │ P0-P7 outputs  │  │ P0-P7 inputs │
                  └────┬──┬──┬─────┘  └──────────────┘
                       │  │  │
                  ┌────▼┐ │  │
                  │    │ │  │
              ┌───►Relay 1  │
              │   ●         │
          ┌───┴─────────────┴──┐
          │   Electronic       │
        ┌─►   Lock Driver      │
        │   ●                  │
        │   ●                  │
    [PowerSupply]          [Solenoid]
```

---

## Code Implementation

### Header File: io_expander.h

```c
#ifndef IO_EXPANDER_H
#define IO_EXPANDER_H

#include <stdint.h>
#include <stdbool.h>

/* IO Expander Configuration */
#define IO_EXP_I2C_BUS         I2C1
#define IO_EXP_BASE_ADDRESS    0x20  // PCF8574 base address
#define IO_EXP_MAX_DEVICES     8
#define IO_EXP_PINS_PER_DEVICE 8

/* Pin Definitions */
typedef enum {
    IO_EXP_PIN_0 = 0,
    IO_EXP_PIN_1,
    IO_EXP_PIN_2,
    IO_EXP_PIN_3,
    IO_EXP_PIN_4,
    IO_EXP_PIN_5,
    IO_EXP_PIN_6,
    IO_EXP_PIN_7
} io_exp_pin_t;

/* Device Addresses */
typedef enum {
    IO_EXP_ADDR_0x20 = 0,  // A2=0, A1=0, A0=0
    IO_EXP_ADDR_0x21,
    IO_EXP_ADDR_0x22,
    IO_EXP_ADDR_0x23,
    IO_EXP_ADDR_0x24,
    IO_EXP_ADDR_0x25,
    IO_EXP_ADDR_0x26,
    IO_EXP_ADDR_0x27
} io_exp_address_t;

/* Status Codes */
typedef enum {
    IO_EXP_OK = 0,
    IO_EXP_ERROR,
    IO_EXP_I2C_ERROR,
    IO_EXP_INVALID_ADDR,
    IO_EXP_TIMEOUT
} io_exp_status_t;

/* Public Function Prototypes */
io_exp_status_t IO_EXP_Init(void);
io_exp_status_t IO_EXP_PinWrite(io_exp_address_t dev_addr, 
                                 io_exp_pin_t pin, bool value);
io_exp_status_t IO_EXP_PinRead(io_exp_address_t dev_addr, 
                                io_exp_pin_t pin, bool *value);
io_exp_status_t IO_EXP_PortWrite(io_exp_address_t dev_addr, uint8_t value);
io_exp_status_t IO_EXP_PortRead(io_exp_address_t dev_addr, uint8_t *value);
io_exp_status_t IO_EXP_SetPinMode(io_exp_address_t dev_addr, 
                                   io_exp_pin_t pin, bool is_input);
io_exp_status_t IO_EXP_SetPortMode(io_exp_address_t dev_addr, uint8_t mode);
bool IO_EXP_IsDevicePresent(io_exp_address_t dev_addr);

#endif
```

### Source File: io_expander.c

```c
#include "io_expander.h"
#include "i2c_driver.h"

/* Private Data Structure */
typedef struct {
    uint8_t address;
    uint8_t output_value;
    uint8_t mode;  // 1=Input, 0=Output
} io_exp_device_t;

static io_exp_device_t g_io_exp_devices[IO_EXP_MAX_DEVICES];

/**
 * @brief Initialize IO Expander
 */
io_exp_status_t IO_EXP_Init(void)
{
    // Initialize I2C
    if (I2C_Init(IO_EXP_I2C_BUS, I2C_SPEED_400KHZ) != I2C_OK) {
        return IO_EXP_I2C_ERROR;
    }
    
    // Initialize all devices as outputs initially
    for (int i = 0; i < IO_EXP_MAX_DEVICES; i++) {
        g_io_exp_devices[i].address = IO_EXP_BASE_ADDRESS + i;
        g_io_exp_devices[i].output_value = 0xFF;  // All high
        g_io_exp_devices[i].mode = 0x00;          // All outputs
        
        // Write initial state if device exists
        IO_EXP_PortWrite(i, 0xFF);
    }
    
    return IO_EXP_OK;
}

/**
 * @brief Write single pin
 */
io_exp_status_t IO_EXP_PinWrite(io_exp_address_t dev_addr, 
                                 io_exp_pin_t pin, bool value)
{
    if (dev_addr >= IO_EXP_MAX_DEVICES || pin >= IO_EXP_PINS_PER_DEVICE) {
        return IO_EXP_INVALID_ADDR;
    }
    
    uint8_t port_value = g_io_exp_devices[dev_addr].output_value;
    
    if (value) {
        port_value |= (1 << pin);   // Set bit
    } else {
        port_value &= ~(1 << pin);  // Clear bit
    }
    
    return IO_EXP_PortWrite(dev_addr, port_value);
}

/**
 * @brief Read single pin
 */
io_exp_status_t IO_EXP_PinRead(io_exp_address_t dev_addr, 
                                io_exp_pin_t pin, bool *value)
{
    if (value == NULL || dev_addr >= IO_EXP_MAX_DEVICES) {
        return IO_EXP_INVALID_ADDR;
    }
    
    uint8_t port_value;
    if (IO_EXP_PortRead(dev_addr, &port_value) != IO_EXP_OK) {
        return IO_EXP_I2C_ERROR;
    }
    
    *value = (port_value & (1 << pin)) ? true : false;
    return IO_EXP_OK;
}

/**
 * @brief Write entire port
 */
io_exp_status_t IO_EXP_PortWrite(io_exp_address_t dev_addr, uint8_t value)
{
    if (dev_addr >= IO_EXP_MAX_DEVICES) {
        return IO_EXP_INVALID_ADDR;
    }
    
    uint8_t device_address = g_io_exp_devices[dev_addr].address << 1;
    
    if (I2C_Write(IO_EXP_I2C_BUS, device_address, &value, 1, 
                  I2C_TIMEOUT_MS) != I2C_OK) {
        return IO_EXP_I2C_ERROR;
    }
    
    g_io_exp_devices[dev_addr].output_value = value;
    return IO_EXP_OK;
}

/**
 * @brief Read entire port
 */
io_exp_status_t IO_EXP_PortRead(io_exp_address_t dev_addr, uint8_t *value)
{
    if (value == NULL || dev_addr >= IO_EXP_MAX_DEVICES) {
        return IO_EXP_INVALID_ADDR;
    }
    
    uint8_t device_address = g_io_exp_devices[dev_addr].address << 1;
    
    if (I2C_Read(IO_EXP_I2C_BUS, device_address, value, 1, 
                 I2C_TIMEOUT_MS) != I2C_OK) {
        return IO_EXP_I2C_ERROR;
    }
    
    return IO_EXP_OK;
}

/**
 * @brief Check if device present on bus
 */
bool IO_EXP_IsDevicePresent(io_exp_address_t dev_addr)
{
    if (dev_addr >= IO_EXP_MAX_DEVICES) {
        return false;
    }
    
    uint8_t dummy;
    return IO_EXP_PortRead(dev_addr, &dummy) == IO_EXP_OK;
}
```

### Relay Control Implementation

```c
/* Relay control definitions */
#define RELAY_IO_EXP_DEVICE     IO_EXP_ADDR_0x20
#define RELAY_LIGHT_PIN         IO_EXP_PIN_0
#define RELAY_LOCK_PIN          IO_EXP_PIN_1
#define RELAY_DOOR_PIN          IO_EXP_PIN_2

/**
 * @brief Control light relay
 */
void Relay_ControlLight(bool on)
{
    IO_EXP_PinWrite(RELAY_IO_EXP_DEVICE, RELAY_LIGHT_PIN, on);
}

/**
 * @brief Control electronic lock
 */
void Relay_ControlLock(bool unlock)
{
    IO_EXP_PinWrite(RELAY_IO_EXP_DEVICE, RELAY_LOCK_PIN, unlock);
}

/**
 * @brief Control door mechanism
 */
void Relay_ControlDoor(bool open)
{
    IO_EXP_PinWrite(RELAY_IO_EXP_DEVICE, RELAY_DOOR_PIN, open);
}

/**
 * @brief Unlock with timeout
 */
void Relay_UnlockTemporary(uint32_t duration_ms)
{
    Relay_ControlLock(true);
    HAL_Delay(duration_ms);
    Relay_ControlLock(false);
}
```

---

## Interview Q&A

### Q1: Why use IO Expander instead of direct GPIO?

**Answer**:
Reasons to use IO Expanders:

1. **GPIO Shortage**: Microcontroller has limited pins
2. **Modular Design**: Expander can be on separate board
3. **Distributed Control**: Can place near components
4. **Daisy-chaining**: Multiple devices via single I2C bus
5. **Cost**: Cheaper than larger MCU
6. **Isolation**: Easy PCB layout with isolated power domains

**Trade-offs**: 
- Slower (I2C vs direct GPIO)
- Higher latency
- More complex code

### Q2: How do you ensure relay safety and prevent damage?

**Answer**:
```c
/* Electrical protection */
1. Freewheeling diode across coil (prevents back-EMF)
2. Current limiting resistor
3. Transistor driver circuit (relay can't draw from MCU pins)
4. Surge protectors on power lines

/* Software protection */
void RelayControl_SafeToggle(relay_id_t relay) {
    if (relay_is_powered()) {
        // Prevent excessive switching
        if (time_since_last_switch < MIN_INTERVAL_MS) 
            return;  // Rate limiting
    }
    
    // Apply control with debounce
    set_relay(relay, STATE_ON);
    wait_ms(10);  // Ensure contact closure
    verify_relay_feedback(relay);  // Monitor
}
```

### Q3: How handle multiple IO Expanders for relay matrix?

**Answer**:
```c
#define RELAY_MATRIX_ROWS  4
#define RELAY_MATRIX_COLS  2

typedef struct {
    io_exp_address_t row_device;
    io_exp_address_t col_device;
    uint8_t row_pin;
    uint8_t col_pin;
} relay_matrix_t;

relaymatrix_t relay_matrix[RELAY_MATRIX_ROWS][RELAY_MATRIX_COLS];
```

Address assignment:
- Rows: IO_EXP_ADDR_0x20 (P0-P3 for 4 rows)
- Cols: IO_EXP_ADDR_0x21 (P0-P1 for 2 cols)

---

**End of IO Expander Guide**
