# SPI EEPROM Communication - Complete Implementation Guide

## Project Overview

This project implements serial peripheral interface (SPI) communication protocol for reading and writing data from/to an EEPROM (Electrically Erasable Programmable Read-Only Memory) device. EEPROM is commonly used in embedded systems for storing persistent configuration data, calibration values, and device parameters.

## Learning Objectives

- Understand SPI protocol and timing
- Implement master-slave communication
- Handle EEPROM read/write operations
- Manage chip select (CS) signals
- Implement error handling and data verification

---

## Table of Contents

1. [Theory & Concepts](#theory--concepts)
2. [Hardware Architecture](#hardware-architecture)
3. [Code Implementation](#code-implementation)
4. [Key Functions](#key-functions)
5. [Testing & Verification](#testing--verification)
6. [Interview Q&A](#interview-qa)

---

## Theory & Concepts

### SPI Protocol Overview

**SPI (Serial Peripheral Interface)** is a synchronous serial communication protocol with the following characteristics:

- **Master-Slave Architecture**: One master device controls multiple slave devices
- **Full-Duplex**: Data can be transmitted and received simultaneously
- **Synchronous**: Communication is clock-driven
- **High Speed**: Supports clock frequencies from 1 MHz to 50+ MHz

### Signals Used

| Signal | Function | Direction |
|--------|----------|-----------|
| SCLK (Serial Clock) | Clock signal | Master to Slave |
| MOSI (Master Out Slave In) | Data from master | Master to Slave |
| MISO (Master In Slave Out) | Data to master | Slave to Master |
| CS (Chip Select) | Slave selection | Master to Slave |

### SPI Modes

| Mode | CPOL | CPHA | Description |
|------|------|------|-------------|
| 0 | 0 | 0 | Clock idle low, sample on rising edge |
| 1 | 0 | 1 | Clock idle low, sample on falling edge |
| 2 | 1 | 0 | Clock idle high, sample on falling edge |
| 3 | 1 | 1 | Clock idle high, sample on rising edge |

### EEPROM Basics

- **Capacity**: Common sizes are 256B, 512B, 1KB, 2KB, 4KB, 8KB, etc.
- **Write Cycles**: Limited (typically 100,000 to 1,000,000 cycles)
- **Retention**: Data persists without power (10+ years)
- **Addressing**: Sequential memory addressing for page operations
- **Page Write**: Typically 16-256 bytes per write operation

---

## Hardware Architecture

### Connection Diagram

```
┌─────────────────┐
│   Microcontroller├──► SCLK ──────┐
│   (SPI Master)  ├──► MOSI ──────┤
│                 │◄──┤MISO ◄─────┤
│                 ├──► CS(1) ─┐   │
│                 │           │   │
└─────────────────┘           │   │
                              │   │
                      ┌───────┴───┴─────┐
                      │  EEPROM Device  │
                      │ (SPI Slave)     │
                      │ (AT25256/etc)   │
                      └─────────────────┘
```

### Pin Configuration

**STM32 MCU (Example)**:
- PA5: SCLK
- PA6: MISO
- PA7: MOSI
- PA4: CS (GPIO controlled)

**EEPROM Connections**:
- PIN 1: CS (Chip Select)
- PIN 2: MISO (Serial Output)
- PIN 3: Not Connected
- PIN 4: GND
- PIN 5: MOSI (Serial Input)
- PIN 6: SCLK (Serial Clock)
- PIN 7: WP (Write Protect - Optional)
- PIN 8: VCC

---

## Code Implementation

### Header File: spi_eeprom.h

```c
#ifndef SPI_EEPROM_H
#define SPI_EEPROM_H

#include <stdint.h>
#include <stdbool.h>

/* EEPROM Configuration */
#define EEPROM_SIZE             (32 * 1024)  // 32KB (AT25256)
#define EEPROM_PAGE_SIZE        256           // Page size for writing
#define EEPROM_PAGE_WRITE_TIME  5             // ms

/* SPI Commands */
#define EEPROM_CMD_READ         0x03          // Read data
#define EEPROM_CMD_WRITE        0x02          // Write data
#define EEPROM_CMD_WREN         0x06          // Write enable
#define EEPROM_CMD_WRDI         0x04          // Write disable
#define EEPROM_CMD_RDSR         0x05          // Read status register
#define EEPROM_CMD_WRSR         0x01          // Write status register

/* Status Register Bits */
#define EEPROM_SR_WIP           (1 << 0)      // Write in progress
#define EEPROM_SR_WEL           (1 << 1)      // Write enable latch
#define EEPROM_SR_BP0           (1 << 2)      // Block protect bit 0
#define EEPROM_SR_BP1           (1 << 3)      // Block protect bit 1

/* Error Codes */
typedef enum {
    EEPROM_OK = 0,
    EEPROM_BUSY,
    EEPROM_WRITE_FAILED,
    EEPROM_READ_FAILED,
    EEPROM_TIMEOUT,
    EEPROM_INVALID_ADDR,
    EEPROM_INVALID_SIZE
} eeprom_status_t;

/* Function Prototypes */
eeprom_status_t EEPROM_Init(void);
eeprom_status_t EEPROM_Read(uint16_t addr, uint8_t *data, uint16_t size);
eeprom_status_t EEPROM_Write(uint16_t addr, const uint8_t *data, uint16_t size);
eeprom_status_t EEPROM_WriteEnable(void);
eeprom_status_t EEPROM_WriteDisable(void);
uint8_t EEPROM_ReadStatus(void);
bool EEPROM_IsBusy(void);
eeprom_status_t EEPROM_WaitReady(uint32_t timeout_ms);
eeprom_status_t EEPROM_ChipErase(void);

#endif /* SPI_EEPROM_H */
```

### Source File: spi_eeprom.c

```c
#include "spi_eeprom.h"
#include "spi_driver.h"
#include "hal_gpio.h"
#include "hal_systick.h"

/* Private Variables */
static volatile uint8_t g_eeprom_ready = 0;

/* Chip Select Control */
static inline void CS_Low(void)
{
    GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);  // Active low
}

static inline void CS_High(void)
{
    GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);    // Inactive high
}

/**
 * @brief Initialize SPI EEPROM
 * @return Status code
 */
eeprom_status_t EEPROM_Init(void)
{
    // Initialize SPI peripheral
    SPI_Config_t spi_config;
    spi_config.clock_polarity = SPI_CPOL_LOW;
    spi_config.clock_phase = SPI_CPHA_1EDGE;
    spi_config.baudrate_prescaler = SPI_BAUDRATE_DIV_4;  // Clock speed
    spi_config.data_size = SPI_DATA_8BIT;
    spi_config.bit_order = SPI_BIT_ORDER_MSB;
    
    if (SPI_Init(SPI1, &spi_config) != SPI_OK) {
        return EEPROM_INIT_FAILED;
    }
    
    // Initialize CS pin as output
    GPIO_Config_t gpio_config;
    gpio_config.pin = GPIO_PIN_4;
    gpio_config.mode = GPIO_MODE_OUT;
    gpio_config.speed = GPIO_SPEED_HIGH;
    GPIO_Init(GPIOA, &gpio_config);
    
    CS_High();  // CS inactive by default
    
    // Wait for EEPROM to be ready
    return EEPROM_WaitReady(1000);
}

/**
 * @brief SPI transmit and receive with CS control
 * @param tx_buf: Transmit buffer
 * @param rx_buf: Receive buffer
 * @param size: Data size
 * @return Status code
 */
static eeprom_status_t SPI_TransmitReceive(const uint8_t *tx_buf, 
                                            uint8_t *rx_buf, 
                                            uint16_t size)
{
    if (SPI_TransmitReceive(SPI1, (uint8_t *)tx_buf, rx_buf, size, 
                            SPI_TIMEOUT_MS) != SPI_OK) {
        return EEPROM_FAILED;
    }
    return EEPROM_OK;
}

/**
 * @brief Read status register
 * @return Status register value
 */
uint8_t EEPROM_ReadStatus(void)
{
    uint8_t cmd = EEPROM_CMD_RDSR;
    uint8_t status = 0;
    uint8_t rx_buf[2] = {0};
    
    CS_Low();
    SPI_TransmitReceive(&cmd, rx_buf, 1);
    SPI_TransmitReceive(&status, rx_buf, 1);
    CS_High();
    
    return rx_buf[1];
}

/**
 * @brief Check if EEPROM is busy
 * @return true if busy, false if ready
 */
bool EEPROM_IsBusy(void)
{
    return (EEPROM_ReadStatus() & EEPROM_SR_WIP) != 0;
}

/**
 * @brief Wait for EEPROM to be ready
 * @param timeout_ms: Timeout in milliseconds
 * @return Status code
 */
eeprom_status_t EEPROM_WaitReady(uint32_t timeout_ms)
{
    uint32_t start_time = HAL_GetTick();
    
    while (EEPROM_IsBusy()) {
        if ((HAL_GetTick() - start_time) > timeout_ms) {
            return EEPROM_TIMEOUT;
        }
    }
    return EEPROM_OK;
}

/**
 * @brief Enable write operations
 * @return Status code
 */
eeprom_status_t EEPROM_WriteEnable(void)
{
    uint8_t cmd = EEPROM_CMD_WREN;
    
    CS_Low();
    if (SPI_Transmit(SPI1, &cmd, 1, SPI_TIMEOUT_MS) != SPI_OK) {
        CS_High();
        return EEPROM_WRITE_FAILED;
    }
    CS_High();
    
    return EEPROM_OK;
}

/**
 * @brief Disable write operations
 * @return Status code
 */
eeprom_status_t EEPROM_WriteDisable(void)
{
    uint8_t cmd = EEPROM_CMD_WRDI;
    
    CS_Low();
    if (SPI_Transmit(SPI1, &cmd, 1, SPI_TIMEOUT_MS) != SPI_OK) {
        CS_High();
        return EEPROM_WRITE_FAILED;
    }
    CS_High();
    
    return EEPROM_OK;
}

/**
 * @brief Read data from EEPROM
 * @param addr: Start address
 * @param data: Pointer to receive buffer
 * @param size: Number of bytes to read
 * @return Status code
 */
eeprom_status_t EEPROM_Read(uint16_t addr, uint8_t *data, uint16_t size)
{
    if (addr + size > EEPROM_SIZE) {
        return EEPROM_INVALID_ADDR;
    }
    
    if (data == NULL || size == 0) {
        return EEPROM_INVALID_SIZE;
    }
    
    uint8_t cmd_buf[3] = {
        EEPROM_CMD_READ,
        (addr >> 8) & 0xFF,     // High byte of address
        addr & 0xFF              // Low byte of address
    };
    
    CS_Low();
    
    // Send command and address
    if (SPI_Transmit(SPI1, cmd_buf, 3, SPI_TIMEOUT_MS) != SPI_OK) {
        CS_High();
        return EEPROM_READ_FAILED;
    }
    
    // Read data
    if (SPI_Receive(SPI1, data, size, SPI_TIMEOUT_MS) != SPI_OK) {
        CS_High();
        return EEPROM_READ_FAILED;
    }
    
    CS_High();
    return EEPROM_OK;
}

/**
 * @brief Write data to EEPROM
 * @param addr: Start address
 * @param data: Pointer to data buffer
 * @param size: Number of bytes to write
 * @return Status code
 * @note Automatically handles page boundaries and write enable
 */
eeprom_status_t EEPROM_Write(uint16_t addr, const uint8_t *data, uint16_t size)
{
    if (addr + size > EEPROM_SIZE) {
        return EEPROM_INVALID_ADDR;
    }
    
    if (data == NULL || size == 0) {
        return EEPROM_INVALID_SIZE;
    }
    
    uint16_t offset = 0;
    uint16_t bytes_to_write;
    uint16_t page_offset;
    
    while (offset < size) {
        // Wait for previous write to complete
        if (EEPROM_WaitReady(EEPROM_PAGE_WRITE_TIME) != EEPROM_OK) {
            return EEPROM_TIMEOUT;
        }
        
        // Enable write
        if (EEPROM_WriteEnable() != EEPROM_OK) {
            return EEPROM_WRITE_FAILED;
        }
        
        // Calculate bytes to write in this page
        page_offset = (addr + offset) % EEPROM_PAGE_SIZE;
        bytes_to_write = EEPROM_PAGE_SIZE - page_offset;
        if (bytes_to_write > (size - offset)) {
            bytes_to_write = size - offset;
        }
        
        // Prepare write command
        uint8_t cmd_buf[3 + EEPROM_PAGE_SIZE];
        cmd_buf[0] = EEPROM_CMD_WRITE;
        cmd_buf[1] = ((addr + offset) >> 8) & 0xFF;
        cmd_buf[2] = (addr + offset) & 0xFF;
        
        // Copy data to command buffer
        for (uint16_t i = 0; i < bytes_to_write; i++) {
            cmd_buf[3 + i] = data[offset + i];
        }
        
        // Transmit
        CS_Low();
        if (SPI_Transmit(SPI1, cmd_buf, 3 + bytes_to_write, 
                         SPI_TIMEOUT_MS) != SPI_OK) {
            CS_High();
            return EEPROM_WRITE_FAILED;
        }
        CS_High();
        
        offset += bytes_to_write;
    }
    
    // Wait for final write to complete
    return EEPROM_WaitReady(EEPROM_PAGE_WRITE_TIME);
}

/**
 * @brief Erase entire EEPROM chip
 * @return Status code
 * @note This is a time-consuming operation
 */
eeprom_status_t EEPROM_ChipErase(void)
{
    // Fill with 0xFF (erase value)
    uint8_t erase_data[256];
    for (int i = 0; i < 256; i++) {
        erase_data[i] = 0xFF;
    }
    
    // Write 0xFF to all memory
    for (uint16_t addr = 0; addr < EEPROM_SIZE; addr += 256) {
        uint16_t write_size = (addr + 256 <= EEPROM_SIZE) ? 256 : 
                             (EEPROM_SIZE - addr);
        if (EEPROM_Write(addr, erase_data, write_size) != EEPROM_OK) {
            return EEPROM_WRITE_FAILED;
        }
    }
    
    return EEPROM_OK;
}
```

### Usage Example

```c
#include "spi_eeprom.h"

int main(void)
{
    // Initialize EEPROM
    if (EEPROM_Init() != EEPROM_OK) {
        // Initialization failed
        return -1;
    }
    
    uint8_t test_data[] = {0x12, 0x34, 0x56, 0x78};
    uint8_t read_data[4] = {0};
    
    // Write data to EEPROM
    if (EEPROM_Write(0x0000, test_data, 4) != EEPROM_OK) {
        return -1;
    }
    
    // Read data from EEPROM
    if (EEPROM_Read(0x0000, read_data, 4) != EEPROM_OK) {
        return -1;
    }
    
    // Verify data
    for (int i = 0; i < 4; i++) {
        if (test_data[i] != read_data[i]) {
            return -1;  // Data mismatch
        }
    }
    
    return 0;
}
```

---

## Key Functions

### EEPROM_Init()
**Purpose**: Initialize SPI peripheral and EEPROM device
**Parameters**: None
**Returns**: Status code
**Key Points**:
- Configures SPI clock, CPOL, CPHA
- Sets up CS pin as GPIO output
- Waits for EEPROM to be ready

### EEPROM_Read()
**Purpose**: Read data from EEPROM at specified address
**Parameters**: 
- addr: 16-bit address (0 to 32KB)
- data: Buffer to receive data
- size: Number of bytes to read
**Returns**: Status code
**Key Points**:
- Full-duplex SPI communication
- Efficient for any data size
- Handles address splitting into high/low bytes

### EEPROM_Write()
**Purpose**: Write data to EEPROM with automatic page handling
**Parameters**:
- addr: 16-bit address
- data: Buffer with data to write
- size: Number of bytes
**Returns**: Status code
**Key Points**:
- Automatically enables write for each page
- Handles page boundaries (256 byte pages)
- Waits for write completion between pages

---

## Testing & Verification

### Unit Tests

```c
#include "unity.h"
#include "spi_eeprom.h"

void setUp(void)
{
    EEPROM_Init();
}

void tearDown(void)
{
    // Cleanup if needed
}

void test_EEPROM_Read_Success(void)
{
    uint8_t test_data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    uint8_t read_data[4] = {0};
    
    EEPROM_Write(0x0000, test_data, 4);
    eeprom_status_t status = EEPROM_Read(0x0000, read_data, 4);
    
    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_data, read_data, 4);
}

void test_EEPROM_Write_MultiplePage(void)
{
    uint8_t test_data[512];  // 2 pages
    uint8_t read_data[512] = {0};
    
    // Fill with pattern
    for (int i = 0; i < 512; i++) {
        test_data[i] = i & 0xFF;
    }
    
    eeprom_status_t status = EEPROM_Write(0x0000, test_data, 512);
    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    
    status = EEPROM_Read(0x0000, read_data, 512);
    TEST_ASSERT_EQUAL(EEPROM_OK, status);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_data, read_data, 512);
}

void test_EEPROM_InvalidAddress(void)
{
    uint8_t data[10];
    eeprom_status_t status = EEPROM_Read(EEPROM_SIZE, data, 10);
    TEST_ASSERT_EQUAL(EEPROM_INVALID_ADDR, status);
}
```

### Debugging Checklist

- [ ] Verify CS pin toggles correctly with oscilloscope
- [ ] Check SCLK frequency and waveform
- [ ] Verify MOSI/MISO data lines with logic analyzer
- [ ] Confirm status register reads correctly
- [ ] Test read after write verification
- [ ] Check page boundary handling
- [ ] Verify write enable/disable sequence

---

## Interview Q&A

### Q1: What is SPI and why is it used for EEPROM communication?

**Answer**: 
SPI (Serial Peripheral Interface) is a synchronous, full-duplex serial communication protocol. It's ideal for EEPROM because:
- **High Speed**: Supports clock speeds up to 50+ MHz
- **Simple Interface**: Only 4 signals (SCLK, MOSI, MISO, CS)
- **Full-Duplex**: Can transmit and receive simultaneously
- **Synchronous**: Clock-driven communication ensures reliable data transfer
- **Low Power**: Efficient for embedded devices
- **Wide Support**: Most microcontrollers have built-in SPI hardware

The master controls the clock and chip select, while the slave (EEPROM) responds with data. This makes it perfect for reading/writing persistent data in embedded systems.

### Q2: Explain the difference between SPI modes and when to use each.

**Answer**:
SPI has 4 modes based on clock polarity (CPOL) and clock phase (CPHA):

| Mode | CPOL | CPHA | When to Use |
|------|------|------|-------------|
| 0 | 0 | 0 | Most common mode; Most EEPROM devices |
| 1 | 0 | 1 | Specific device requirements |
| 2 | 1 | 0 | Specialized communication |
| 3 | 1 | 1 | Legacy devices |

- **CPOL (Clock Polarity)**: Determines idle state (0=low, 1=high)
- **CPHA (Clock Phase)**: Determines sampling edge (0=first edge, 1=second edge)

For most EEPROMs like AT25256, Mode 0 is standard. You must match the device datasheet requirement to avoid data corruption.

### Q3: How does SPI handle chip select and why is it important?

**Answer**:
Chip Select (CS) is an active-low signal that:
- **Enables Communication**: CS must go LOW before communication begins
- **Disables Communication**: CS goes HIGH to end communication
- **Selects Slave**: In multi-slave systems, CS identifies which device to talk to
- **Synchronizes**: Tells the slave when to start expects data

Example sequence:
```
CS High (inactive)
   ↓
CS goes Low (select slave)
   ↓
Master sends command + address
   ↓
Master/Slave exchange data
   ↓
CS goes High (deselect slave)
```

Proper CS handling prevents:
- Data corruption from partial commands
- Conflicts in multi-master scenarios
- Accidental writes to wrong devices

### Q4: Explain the EEPROM word/page write concept. Why is page write necessary?

**Answer**:
**Byte Write vs Page Write**:

| Aspect | Byte Write | Page Write |
|--------|-----------|-----------|
| Time per byte | ~5ms | ~5ms total |
| Bytes written | 1 | Up to 256 |
| Efficiency | 1 byte/5ms | 256 bytes/5ms |

**Why Page Write**:
- **Speed**: Writing 1KB takes ~4ms instead of 1 second
- **Efficiency**: Each write operation has overhead
- **Wear**: EEPROM has limited write cycles (~100K); page write counts as one cycle
- **Battery Life**: Reduced power consumption in low-power devices

**Implementation**:
```c
// Automatically handle page boundaries
uint16_t bytes_to_write = EEPROM_PAGE_SIZE - page_offset;
if (bytes_to_write > remaining) 
    bytes_to_write = remaining;
EEPROM_Write(addr, data, bytes_to_write);
```

### Q5: How do you verify data integrity after write operations?

**Answer**:
**Multi-level Verification Strategy**:

1. **Immediate Read-After-Write**:
```c
EEPROM_Write(addr, data, size);
EEPROM_Read(addr, read_data, size);
if (memcmp(data, read_data, size) != 0) 
    // Verification failed!
```

2. **CRC-Based Verification**:
```c
uint32_t crc = CRC32_Calculate(data, size);
EEPROM_Write(addr, data, size);
EEPROM_Write(addr + size, (uint8_t*)&crc, 4);
```

3. **Wear-Leveling with Backup**:
```c
// Store in two locations and rotate
Write to primary location
Write to backup location
Read from primary, if corrupted read from backup
```

4. **Error Handling**:
- Implement retry logic
- Mark data as invalid if verification fails
- Use watchdog for timeout detection

This ensures mission-critical data remains uncorrupted throughout the device lifetime.

### Q6: What happens if CS is not toggled correctly? How do you debug this?

**Answer**:
**Wrong CS Behavior**:
- **CS stays LOW**: Next command corrupts EEPROM state
- **CS toggles mid-command**: Incomplete command/data, EEPROM confused
- **CS timing wrong**: Data misalignment, read wrong bytes

**Debugging Steps**:
1. **Use Oscilloscope/Logic Analyzer**:
   - Confirm CS goes LOW before SPI clock starts
   - Confirm CS goes HIGH after all 8 bits transmitted
   - Check CS timing relative to SCLK edges

2. **Add Debug Prints**:
```c
void CS_Low(void)
{
    DEBUG_PRINT("CS LOW");
    GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void CS_High(void)
{
    DEBUG_PRINT("CS HIGH");
    GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
```

3. **Read Status Register**:
   - If status reads as 0xFF, communication never reached slave
   - If garbage, CS timing is wrong

4. **Test with Simple Pattern**:
```c
// Try to read status (simplest operation)
uint8_t status = EEPROM_ReadStatus();
if (status == 0xFF) // Communication failed
if (status == 0x00) // Communication success but wrong value
```

### Q7: How would you handle multiple EEPROM devices on the same SPI bus?

**Answer**:
**Multi-Device Architecture**:
```
MCU ──┬──► EEPROM1 (CS on PA4)
      │
      ├──► EEPROM2 (CS on PA5)
      │
      └──► EEPROM3 (CS on PA6)
      
All share: SCLK, MOSI, MISO
```

**Implementation**:
```c
typedef enum {
    EEPROM_DEVICE_1 = 4,  // PA4
    EEPROM_DEVICE_2 = 5,  // PA5
    EEPROM_DEVICE_3 = 6   // PA6
} eeprom_device_t;

typedef struct {
    eeprom_device_t device_id;
    uint16_t address;
    uint16_t size;
} eeprom_context_t;

eeprom_status_t EEPROM_ReadMulti(eeprom_context_t *ctx, 
                                  uint8_t *data, uint16_t size)
{
    uint8_t cmd_buf[3] = {EEPROM_CMD_READ, ...};
    
    // Select specific device
    GPIO_WritePin(GPIOA, ctx->device_id, GPIO_PIN_RESET);
    SPI_Transmit(SPI1, cmd_buf, 3, TIMEOUT);
    SPI_Receive(SPI1, data, size, TIMEOUT);
    GPIO_WritePin(GPIOA, ctx->device_id, GPIO_PIN_SET);
    
    return EEPROM_OK;
}
```

**Key Considerations**:
- Each device needs separate CS pin
- SPI frequency must be acceptable to all devices
- Code becomes more complex
- Consider using I2C if multiple memory devices needed

---

## Performance Optimization Tips

1. **Use DMA for Large Transfers**: Offload SPI data to DMA controller
2. **Implement Page Write**: Dramatically faster than byte-by-byte
3. **Minimize CS Toggles**: Reduces overhead
4. **Cache Frequently Accessed Data**: Read once, cache in RAM
5. **Use Fast Clock**: Many EEPROMs support 25-40 MHz

---

## Common Pitfalls & Solutions

| Pitfall | Solution |
|---------|----------|
| Data corruption | Implement read-after-write verification |
| Timeout waiting for write | Increase wait time, check power supply |
| Partial writes | Verify page boundary handling |
| CS timing issues | Use oscilloscope to verify timing |
| Address overflow | Check addr + size ≤ EEPROM_SIZE |
| Write enable not set | Ensure WREN command before write |

---

**End of SPI EEPROM Communication Guide**

This comprehensive guide covers protocol details, implementation, testing, and common interview questions. Practice implementing this from scratch to master SPI communication!
