# SPI Flash Memory (W25Q Series) - Complete Implementation Guide

## Project Overview

SPI Flash memory is commonly used in embedded systems for storing large amounts of data (firmware, logs, configuration, media files). Unlike EEPROM which stores small configuration data, Flash memory provides higher density storage with different programming characteristics and use cases.

## Learning Objectives

- Understand Flash vs EEPROM architecture differences
- Implement W25Q series SPI Flash communication
- Handle Flash-specific operations (sector erase, page write)
- Manage Flash wear-leveling and lifetime
- Optimize read/write performance
- Handle multiple Flash devices on same SPI bus

---

## Table of Contents

1. [Flash vs EEPROM Comparison](#flash-vs-eeprom-comparison)
2. [Flash Memory Architecture](#flash-memory-architecture)
3. [W25Q Series Overview](#w25q-series-overview)
4. [Code Implementation](#code-implementation)
5. [Advanced Features](#advanced-features)
6. [Interview Q&A](#interview-qa)

---

## Flash vs EEPROM Comparison

| Aspect | EEPROM (AT25256) | SPI Flash (W25Q64) |
|--------|------------------|-------------------|
| **Capacity** | 32KB | 8MB (256x larger) |
| **Write Granularity** | Byte/word | Page (256 bytes) |
| **Erase Unit** | N/A (in-place write) | Sector (4KB) or Block (64KB) |
| **Write Time** | ~5ms per byte | ~1-3ms per 256 byte page |
| **Erase Time** | N/A | ~50ms sector / ~500ms block |
| **Cost per MB** | ~$5 | ~$0.50 (10x cheaper) |
| **Use Cases** | Configuration, small data | Firmware, logs, large files |
| **Interface** | SPI, I2C | SPI, Dual-SPI, Quad-SPI |
| **Wear Leveling** | Not typically needed | Required for longevity |
| **Power** | Low current | Higher during erase |

**Decision Matrix**:
- **Use EEPROM** if: Small data (<64KB), byte-level updates needed, frequent random writes
- **Use Flash** if: Large storage (>1MB), sequential writes acceptable, cost-sensitive

---

## Flash Memory Architecture

### Internal Structure

```
W25Q64 Flash (8MB total)

Physical Layout:
┌─────────────────────────────────────┐
│ Flash Chip (8MB = 8,388,608 bytes)  │
├─────────────────────────────────────┤
│ ┌─ Block 0 (64KB)        [Pages 0-255]
│ │
│ │ ┌─ Sector 0 (4KB)      [Pages 0-15]
│ │ │  ┌─ Page 0 (256B)    [Bytes 0-255]
│ │ │  ├─ Page 1 (256B)    [Bytes 256-511]
│ │ │  ├─ ...
│ │ │  └─ Page 15 (256B)   [Bytes 3840-4095]
│ │ │
│ │ ├─ Sector 1 (4KB)      [Pages 16-31]
│ │ └─ ... (12 more sectors)
│ │
│ ├─ Block 1 (64KB)        [Pages 256-511]
│ ├─ Block 2 (64KB)
│ └─ ... (127 more blocks)
└─────────────────────────────────────┘

Total: 128 Blocks or 2048 Sectors
```

### Memory Organization

```
W25Q64 Address Mapping:

Address Bits: A23-A0 (24-bit addressing for 16MB max)

Block:  A23-A16 (128 blocks maximum)
Sector: A15-A12 (16 sectors per block)
Page:   A11-A8  (16 pages per sector)
Byte:   A7-A0   (256 bytes per page)

Example: Address 0x001234 (4660 bytes)
├─ Block:  0x00 (Block 0)
├─ Sector: 0x01 (Sector 1, offset 4096 bytes)
├─ Page:   0x02 (Page 2, offset 512 bytes)
└─ Byte:   0x34 (Byte 52 in page)
```

### Write/Erase Operations

```
Write Operation (Similar to EEPROM):
1. Send WREN command (Write Enable)
2. Set status register if needed
3. Send PP (Page Program) command + address
4. Transmit up to 256 bytes
5. Device programs internally (~1-3ms)
6. Send RDSR to check completion

Erase Operation (Different from EEPROM):
┌─ Sector Erase (4KB, ~50ms)
│  └─ Used for targeted data deletion
├─ Block Erase (64KB, ~500ms)
│  └─ Faster for large areas
└─ Chip Erase (Full 8MB, ~20s)
   └─ Reset to factory state

IMPORTANT: Must erase before writing to specific location!
```

---

## W25Q Series Overview

### Common Models

| Model | Capacity | Voltage | Speed |
|-------|----------|---------|-------|
| W25Q16 | 2MB | 2.7-3.6V | 25MHz |
| W25Q32 | 4MB | 2.7-3.6V | 40MHz |
| W25Q64 | 8MB | 2.7-3.6V | 40MHz |
| W25Q128 | 16MB | 2.7-3.6V | 50MHz |
| W25Q256 | 32MB | 2.7-3.6V | 50MHz |

### Pin Configuration

```
W25Q64 SOIC-8 Package:

    ┌─────────────┐
    │  GND   1  8 VCC   │
    │  /CS   2  7 /HOLD │
    │  MOSI  3  6 /WP   │
    │  MISO  4  5 SCK   │
    └─────────────┘

Pin Descriptions:
- GND:   Ground
- VCC:   Power (2.7-3.6V)
- /CS:   Chip Select (active low)
- SCK:   Serial Clock
- MOSI:  Master Out Slave In
- MISO:  Master In Slave Out
- /WP:   Write Protect (active low)
- /HOLD: Hold (active low)
```

### W25Q Commands

| Command | Code | Description | Bytes |
|---------|------|-------------|-------|
| **WREN** | 0x06 | Write Enable | 1 |
| **WRDI** | 0x04 | Write Disable | 1 |
| **PP** | 0x02 | Page Program | 4+ |
| **SE** | 0x20 | Sector Erase (4KB) | 4 |
| **BE** | 0xD8 | Block Erase (64KB) | 4 |
| **CE** | 0xC7 | Chip Erase | 1 |
| **RDSR** | 0x05 | Read Status Register | 2 |
| **WRSR** | 0x01 | Write Status Register | 3 |
| **JEDEC ID** | 0x9F | Read JEDEC ID | 4 |
| **RD** | 0x03 | Read Data | 4+ |
| **HS** | 0x0B | High-Speed Read | 5+ |

---

## Code Implementation

### Header File: spi_flash.h

```c
#ifndef SPI_FLASH_H
#define SPI_FLASH_H

#include <stdint.h>
#include <stdbool.h>

/* Flash Configuration */
#define FLASH_SIZE              (8 * 1024 * 1024)  // 8MB for W25Q64
#define FLASH_PAGE_SIZE         256                 // Bytes per page
#define FLASH_SECTOR_SIZE       (4 * 1024)         // Bytes per sector
#define FLASH_BLOCK_SIZE        (64 * 1024)        // Bytes per block

/* Flash Commands */
#define FLASH_CMD_WREN          0x06               // Write Enable
#define FLASH_CMD_WRDI          0x04               // Write Disable
#define FLASH_CMD_RDSR          0x05               // Read Status
#define FLASH_CMD_WRSR          0x01               // Write Status
#define FLASH_CMD_WRITE         0x02               // Page Program
#define FLASH_CMD_READ          0x03               // Read
#define FLASH_CMD_FAST_READ     0x0B               // Fast Read
#define FLASH_CMD_SE            0x20               // Sector Erase
#define FLASH_CMD_BE            0xD8               // Block Erase
#define FLASH_CMD_CE            0xC7               // Chip Erase
#define FLASH_CMD_ID            0x9F               // JEDEC ID
#define FLASH_CMD_PUQID         0x4B               // Unique ID

/* Status Register Bits */
#define FLASH_SR_WIP            (1 << 0)           // Write In Progress
#define FLASH_SR_WEL            (1 << 1)           // Write Enable Latch
#define FLASH_SR_BP0            (1 << 2)           // Block Protect Bit 0
#define FLASH_SR_BP1            (1 << 3)           // Block Protect Bit 1
#define FLASH_SR_BP2            (1 << 4)           // Block Protect Bit 2

/* Error Codes */
typedef enum {
    FLASH_OK = 0,
    FLASH_ERROR,
    FLASH_BUSY,
    FLASH_TIMEOUT,
    FLASH_ADDRESS_INVALID,
    FLASH_SIZE_INVALID,
    FLASH_NOT_READY,
    FLASH_ERASE_FAILED,
    FLASH_WRITE_FAILED,
    FLASH_ID_MISMATCH
} flash_status_t;

/* JEDEC ID Structure */
typedef struct {
    uint8_t manufacturer;
    uint8_t type;
    uint8_t capacity;
} flash_jedec_id_t;

/* Public Function Prototypes */
flash_status_t FLASH_Init(void);
flash_status_t FLASH_ReadID(flash_jedec_id_t *id);
flash_status_t FLASH_Read(uint32_t address, uint8_t *data, uint32_t size);
flash_status_t FLASH_Write(uint32_t address, const uint8_t *data, uint32_t size);
flash_status_t FLASH_EraseSector(uint32_t address);
flash_status_t FLASH_EraseBlock(uint32_t address);
flash_status_t FLASH_EraseChip(void);
flash_status_t FLASH_WaitReady(uint32_t timeout_ms);
bool FLASH_IsBusy(void);
flash_status_t FLASH_WritePage(uint32_t address, const uint8_t *data, uint16_t size);
flash_status_t FLASH_WriteEnable(void);
flash_status_t FLASH_WriteDisable(void);
uint8_t FLASH_ReadStatus(void);
float FLASH_GetUtilization(void);

#endif /* SPI_FLASH_H */
```

### Source File: spi_flash.c

```c
#include "spi_flash.h"
#include "spi_driver.h"
#include "hal_gpio.h"
#include "hal_systick.h"

/* Private Variables */
static volatile uint32_t g_flash_write_operations = 0;
static volatile uint32_t g_flash_erase_operations = 0;

/* Chip Select Control */
static inline void CS_Low(void)
{
    GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

static inline void CS_High(void)
{
    GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

/**
 * @brief Initialize SPI Flash
 */
flash_status_t FLASH_Init(void)
{
    // Initialize SPI (same as EEPROM)
    SPI_Config_t spi_config;
    spi_config.clock_polarity = SPI_CPOL_LOW;
    spi_config.clock_phase = SPI_CPHA_1EDGE;
    spi_config.baudrate_prescaler = SPI_BAUDRATE_DIV_4;  // 25MHz for W25Q
    spi_config.data_size = SPI_DATA_8BIT;
    spi_config.bit_order = SPI_BIT_ORDER_MSB;
    
    if (SPI_Init(SPI1, &spi_config) != SPI_OK) {
        return FLASH_ERROR;
    }
    
    CS_High();  // Inactive by default
    
    // Verify device ID
    flash_jedec_id_t id;
    if (FLASH_ReadID(&id) != FLASH_OK) {
        return FLASH_ID_MISMATCH;
    }
    
    // Verify it's W25Q (0xEF = Winbond)
    if (id.manufacturer != 0xEF) {
        return FLASH_ID_MISMATCH;
    }
    
    return FLASH_OK;
}

/**
 * @brief Read JEDEC ID
 */
flash_status_t FLASH_ReadID(flash_jedec_id_t *id)
{
    if (id == NULL) return FLASH_ERROR;
    
    uint8_t cmd = FLASH_CMD_ID;
    uint8_t rx_data[4] = {0};
    
    CS_Low();
    SPI_Transmit(SPI1, &cmd, 1, 100);
    SPI_Receive(SPI1, rx_data, 3, 100);
    CS_High();
    
    id->manufacturer = rx_data[0];
    id->type = rx_data[1];
    id->capacity = rx_data[2];
    
    return FLASH_OK;
}

/**
 * @brief Read Flash status register
 */
uint8_t FLASH_ReadStatus(void)
{
    uint8_t cmd = FLASH_CMD_RDSR;
    uint8_t status = 0;
    
    CS_Low();
    SPI_Transmit(SPI1, &cmd, 1, 100);
    SPI_Receive(SPI1, &status, 1, 100);
    CS_High();
    
    return status;
}

/**
 * @brief Check if Flash is busy
 */
bool FLASH_IsBusy(void)
{
    return (FLASH_ReadStatus() & FLASH_SR_WIP) != 0;
}

/**
 * @brief Wait for Flash to be ready
 */
flash_status_t FLASH_WaitReady(uint32_t timeout_ms)
{
    uint32_t start_time = HAL_GetTick();
    
    while (FLASH_IsBusy()) {
        if ((HAL_GetTick() - start_time) > timeout_ms) {
            return FLASH_TIMEOUT;
        }
    }
    return FLASH_OK;
}

/**
 * @brief Enable write operations
 */
flash_status_t FLASH_WriteEnable(void)
{
    uint8_t cmd = FLASH_CMD_WREN;
    
    CS_Low();
    SPI_Transmit(SPI1, &cmd, 1, 100);
    CS_High();
    
    // Verify WEL bit set
    HAL_Delay(1);
    if (!(FLASH_ReadStatus() & FLASH_SR_WEL)) {
        return FLASH_WRITE_FAILED;
    }
    
    return FLASH_OK;
}

/**
 * @brief Disable write operations
 */
flash_status_t FLASH_WriteDisable(void)
{
    uint8_t cmd = FLASH_CMD_WRDI;
    
    CS_Low();
    SPI_Transmit(SPI1, &cmd, 1, 100);
    CS_High();
    
    return FLASH_OK;
}

/**
 * @brief Read data from Flash
 */
flash_status_t FLASH_Read(uint32_t address, uint8_t *data, uint32_t size)
{
    if (address + size > FLASH_SIZE) {
        return FLASH_ADDRESS_INVALID;
    }
    if (data == NULL || size == 0) {
        return FLASH_SIZE_INVALID;
    }
    
    // Use fast read for better performance
    uint8_t cmd_buf[5] = {
        FLASH_CMD_FAST_READ,
        (address >> 16) & 0xFF,
        (address >> 8) & 0xFF,
        address & 0xFF,
        0x00  // Dummy byte for fast read
    };
    
    CS_Low();
    SPI_Transmit(SPI1, cmd_buf, 5, 100);
    SPI_Receive(SPI1, data, size, 1000);
    CS_High();
    
    return FLASH_OK;
}

/**
 * @brief Write page to Flash (internal use)
 */
flash_status_t FLASH_WritePage(uint32_t address, const uint8_t *data, uint16_t size)
{
    if (size > FLASH_PAGE_SIZE) {
        return FLASH_SIZE_INVALID;
    }
    
    // Enable write
    if (FLASH_WriteEnable() != FLASH_OK) {
        return FLASH_WRITE_FAILED;
    }
    
    // Prepare command
    uint8_t cmd_buf[4 + FLASH_PAGE_SIZE];
    cmd_buf[0] = FLASH_CMD_WRITE;
    cmd_buf[1] = (address >> 16) & 0xFF;
    cmd_buf[2] = (address >> 8) & 0xFF;
    cmd_buf[3] = address & 0xFF;
    
    // Copy data
    for (uint16_t i = 0; i < size; i++) {
        cmd_buf[4 + i] = data[i];
    }
    
    // Transmit
    CS_Low();
    SPI_Transmit(SPI1, cmd_buf, 4 + size, 100);
    CS_High();
    
    // Wait for write to complete
    if (FLASH_WaitReady(10) != FLASH_OK) {
        return FLASH_TIMEOUT;
    }
    
    g_flash_write_operations++;
    return FLASH_OK;
}

/**
 * @brief Write data to Flash (handles page boundaries)
 */
flash_status_t FLASH_Write(uint32_t address, const uint8_t *data, uint32_t size)
{
    if (address + size > FLASH_SIZE) {
        return FLASH_ADDRESS_INVALID;
    }
    if (data == NULL || size == 0) {
        return FLASH_SIZE_INVALID;
    }
    
    uint32_t offset = 0;
    uint16_t bytes_to_write;
    uint16_t page_offset;
    
    while (offset < size) {
        // Calculate bytes to write in this page
        page_offset = (address + offset) % FLASH_PAGE_SIZE;
        bytes_to_write = FLASH_PAGE_SIZE - page_offset;
        if (bytes_to_write > (size - offset)) {
            bytes_to_write = size - offset;
        }
        
        // Write this page
        if (FLASH_WritePage(address + offset, &data[offset], bytes_to_write) 
            != FLASH_OK) {
            return FLASH_WRITE_FAILED;
        }
        
        offset += bytes_to_write;
    }
    
    return FLASH_OK;
}

/**
 * @brief Erase a sector (4KB)
 */
flash_status_t FLASH_EraseSector(uint32_t address)
{
    if (address >= FLASH_SIZE) {
        return FLASH_ADDRESS_INVALID;
    }
    
    // Align to sector boundary
    address = address - (address % FLASH_SECTOR_SIZE);
    
    // Enable write
    if (FLASH_WriteEnable() != FLASH_OK) {
        return FLASH_ERASE_FAILED;
    }
    
    // Send erase command
    uint8_t cmd_buf[4] = {
        FLASH_CMD_SE,
        (address >> 16) & 0xFF,
        (address >> 8) & 0xFF,
        address & 0xFF
    };
    
    CS_Low();
    SPI_Transmit(SPI1, cmd_buf, 4, 100);
    CS_High();
    
    // Wait for erase (up to 100ms for sector)
    if (FLASH_WaitReady(150) != FLASH_OK) {
        return FLASH_TIMEOUT;
    }
    
    g_flash_erase_operations++;
    return FLASH_OK;
}

/**
 * @brief Erase a block (64KB)
 */
flash_status_t FLASH_EraseBlock(uint32_t address)
{
    if (address >= FLASH_SIZE) {
        return FLASH_ADDRESS_INVALID;
    }
    
    // Align to block boundary
    address = address - (address % FLASH_BLOCK_SIZE);
    
    // Enable write
    if (FLASH_WriteEnable() != FLASH_OK) {
        return FLASH_ERASE_FAILED;
    }
    
    // Send erase command
    uint8_t cmd_buf[4] = {
        FLASH_CMD_BE,
        (address >> 16) & 0xFF,
        (address >> 8) & 0xFF,
        address & 0xFF
    };
    
    CS_Low();
    SPI_Transmit(SPI1, cmd_buf, 4, 100);
    CS_High();
    
    // Wait for erase (up to 1 second for block)
    if (FLASH_WaitReady(1000) != FLASH_OK) {
        return FLASH_TIMEOUT;
    }
    
    g_flash_erase_operations++;
    return FLASH_OK;
}

/**
 * @brief Erase entire Flash chip
 */
flash_status_t FLASH_EraseChip(void)
{
    // Enable write
    if (FLASH_WriteEnable() != FLASH_OK) {
        return FLASH_ERASE_FAILED;
    }
    
    // Send chip erase command
    uint8_t cmd = FLASH_CMD_CE;
    
    CS_Low();
    SPI_Transmit(SPI1, &cmd, 1, 100);
    CS_High();
    
    // Wait for chip erase (up to 30 seconds for 8MB)
    if (FLASH_WaitReady(30000) != FLASH_OK) {
        return FLASH_TIMEOUT;
    }
    
    return FLASH_OK;
}

/**
 * @brief Get Flash utilization percentage
 */
float FLASH_GetUtilization(void)
{
    // Track via write counter (simplified)
    // In production, would scan flash for actual usage
    return 0.0f;
}
```

### Usage Example

```c
#include "spi_flash.h"
#include "stdio.h"

int main(void)
{
    // Initialize Flash
    if (FLASH_Init() != FLASH_OK) {
        printf("Flash initialization failed\n");
        return -1;
    }
    
    // Read device ID
    flash_jedec_id_t id;
    FLASH_ReadID(&id);
    printf("Manufacturer: 0x%02X, Type: 0x%02X, Capacity: 0x%02X\n",
           id.manufacturer, id.type, id.capacity);
    
    // Erase sector before writing
    uint32_t address = 0x1000;  // Sector 1
    if (FLASH_EraseSector(address) != FLASH_OK) {
        printf("Erase failed\n");
        return -1;
    }
    
    // Write data
    uint8_t write_data[] = "Hello Flash Memory!";
    if (FLASH_Write(address, write_data, sizeof(write_data)) != FLASH_OK) {
        printf("Write failed\n");
        return -1;
    }
    
    // Read back data
    uint8_t read_data[64] = {0};
    if (FLASH_Read(address, read_data, sizeof(write_data)) != FLASH_OK) {
        printf("Read failed\n");
        return -1;
    }
    
    // Verify
    if (memcmp(write_data, read_data, sizeof(write_data)) == 0) {
        printf("Verification successful!\n");
        printf("Read: %s\n", (char*)read_data);
    } else {
        printf("Data mismatch!\n");
        return -1;
    }
    
    return 0;
}
```

---

## Advanced Features

### Wear-Leveling Implementation

```c
typedef struct {
    uint32_t write_count;
    uint32_t wear_level;
    uint32_t primary_addr;
    uint32_t backup_addr;
} wear_level_t;

#define WEAR_LEVEL_THRESHOLD 10000

void Flash_WearLevel_Write(wear_level_t *ctx, const uint8_t *data, uint32_t size)
{
    if (ctx->write_count >= WEAR_LEVEL_THRESHOLD) {
        // Move data to backup location
        FLASH_EraseSector(ctx->backup_addr);
        FLASH_Write(ctx->backup_addr, data, size);
        
        // Swap addresses
        uint32_t temp = ctx->primary_addr;
        ctx->primary_addr = ctx->backup_addr;
        ctx->backup_addr = temp;
        
        ctx->write_count = 0;
        ctx->wear_level++;
    }
    
    // Write to primary
    FLASH_Write(ctx->primary_addr, data, size);
    ctx->write_count++;
}
```

### Bad Block Management

```c
#define MAX_BAD_BLOCKS 10

typedef struct {
    uint32_t bad_blocks[MAX_BAD_BLOCKS];
    uint8_t count;
} bad_block_table_t;

flash_status_t Flash_IsBadBlock(bad_block_table_t *table, uint32_t block_addr)
{
    for (int i = 0; i < table->count; i++) {
        if (table->bad_blocks[i] == block_addr) {
            return FLASH_ERROR;  // Bad block
        }
    }
    return FLASH_OK;  // Good block
}
```

---

## Interview Q&A

### Q1: Explain the key differences between Flash and EEPROM memory.

**Answer**:

| Aspect | EEPROM | Flash |
|--------|--------|-------|
| **Capacity** | Small (64KB typical) | Large (8MB+) |
| **Write Unit** | Byte (1 byte = 1 write) | Page (256 bytes = 1 write) |
| **Erase Unit** | N/A (no erase needed) | Sector (4KB) or Block (64KB) |
| **Cost** | High (~$5/MB) | Low (~$0.50/MB) |
| **Write Speed** | ~5ms per byte | ~1ms per 256 bytes |
| **Use Case** | Configuration parameters | Firmware, logs, files |

**Key Implication**: Flash requires erase-before-write. You cannot arbitrarily update one byte without erasing 4KB sector first.

### Q2: Why is wear-leveling necessary for Flash memory?

**Answer**:
Flash cells have limited erase cycles (typically 100,000-1,000,000 for MLC/TLC).

Without wear-leveling:
```
Frequency pattern:
Sector 0: Written 500,000 times (DEAD)
Sector 1: Written 100 times (OK)
Sector 2: Written 50 times (OK)
                    ↓
Result: Device fails prematurely
```

Wear-leveling solution:
```
Distribute writes across sectors:
Sector 0: 100,000 writes
Sector 1: 100,000 writes
Sector 2: 100,000 writes
Sector 3: 100,000 writes
Sector 4: 100,000 writes
                    ↓
Result: Device lasts 5x longer (500,000 total writes)
```

**Implementation Strategy**:
1. Track write count for each sector
2. Rotate to different sectors when threshold reached
3. Periodically migrate data to less-used sectors
4. Use wear-leveling algorithm (round-robin, weighted, etc.)

### Q3: Compare sequential vs random access performance in Flash.

**Answer**:

**Sequential Access** (Good):
```
Write 1MB continuously:
1. Erase Block 0 (64KB) ............ 500ms
2. Write 256 pages 256 bytes ....... 256ms
3. Erase Block 1 (64KB) ............ 500ms
4. ... repeat
Total: ~4 seconds for 1MB
```

**Random Access** (Bad):
```
Write 1MB randomly across sectors:
For each 4KB sector:
1. Read current 4KB (existing data)
2. Erase 4KB sector ................ 50ms
3. Merge old + new data
4. Write back 4KB .................. 10ms
5. Repeat
Total: ~200+ seconds for 1MB (50x slower!)
```

**Optimization Strategy**:
- Use **write buffer** to collect writes
- Flush buffer in sequential blocks
- Pre-erase sectors when possible
- Minimize erase operations

### Q4: How would you design a file system for SPI Flash?

**Answer**:

```
Flash Partitioning:
┌──────────────────┐ 0x000000
│ Boot Sector      │ (1 block = 64KB)
├──────────────────┤ 0x010000
│ FAT/FTL          │ (256KB)
│ (File allocation)│
├──────────────────┤ 0x050000
│ File Data Area   │ (remainder)
│                  │
│ (Wear-leveled)   │
└──────────────────┘

File Allocation Table (simplified):
Sector 0: FILE_A (occupied)
Sector 1: FILE_A (occupied)
Sector 2: FILE_B (occupied)
Sector 3: FREE
Sector 4: FILE_C (occupied)
          ↓
Allow writing to Sector 3 next
```

**Key Features**:
1. **Static allocation** for boot/config
2. **Dynamic allocation** with FTL (Flash Translation Layer)
3. **Wear-leveling** across all sectors
4. **Bad block handling** for defective sectors
5. **Integrity checks** (CRC/ECC) per block

### Q5: How detect and recover from Flash corruption?

**Answer**:

```c
typedef struct {
    uint32_t crc;           // CRC of data
    uint32_t timestamp;     // When written
    uint8_t version;        // Format version
} flash_header_t;

flash_status_t Flash_ReadWithVerification(uint32_t addr, uint8_t *data, 
                                          uint32_t size)
{
    // Read header
    flash_header_t header;
    FLASH_Read(addr, (uint8_t*)&header, sizeof(header));
    
    // Read data
    FLASH_Read(addr + sizeof(header), data, size);
    
    // Verify CRC
    uint32_t calc_crc = CRC32(data, size);
    if (calc_crc != header.crc) {
        // Corruption detected
        return FLASH_ERROR;
    }
    
    return FLASH_OK;
}

// Recovery strategy
typedef struct {
    uint32_t primary_addr;
    uint32_t backup_addr;
} redundant_storage_t;

flash_status_t Flash_ReadRedundant(redundant_storage_t *ctx, uint8_t *data)
{
    // Try primary
    if (Flash_ReadWithVerification(ctx->primary_addr, data, SIZE) == FLASH_OK)
        return FLASH_OK;
    
    // Try backup
    if (Flash_ReadWithVerification(ctx->backup_addr, data, SIZE) == FLASH_OK)
        return FLASH_OK;
    
    // Both failed
    return FLASH_ERROR;
}
```

**Recovery Methods**:
1. **Redundant Storage**: Store critical data in two locations
2. **ECC (Error Correcting Code)**: Detect and correct bit errors
3. **CRC Validation**: Detect corruption on read
4. **Journal/Log**: Atomic writes with commit markers
5. **Rollback**: Revert to last known-good state

---

## Performance Comparison

### Read Performance

```
Standard Read (0x03):      25+ MHz typical
Fast Read (0x0B):          40+ MHz typical
Dual I/O Read:             50+ MHz
Quad I/O Read:             100+ MHz
```

### Write/Erase Performance

| Operation | Time | Size |
|-----------|------|------|
| Page Write | 1-3ms | 256B |
| Sector Erase | ~50ms | 4KB |
| Block Erase | ~500ms | 64KB |
| Chip Erase | ~20s | 8MB |

### Optimization Tips

1. **Use Fast Read** (0x0B) instead of standard read (0x03)
2. **Batch writes** to reduce erase overhead
3. **Pre-erase sectors** during idle time
4. **Use block erase** for large deletions
5. **Enable inline ECC** for critical data

---

## Common Issues & Solutions

| Issue | Cause | Solution |
|-------|-------|----------|
| Data corruption | Interrupted write | Use atomic operations |
| Slow random writes | Frequent erase operations | Use write buffer |
| Device wear | Unbalanced write pattern | Implement wear-leveling |
| Timeout errors | Insufficient wait time | Increase timeout |
| Read failures | Bad blocks | Maintain bad block table |

---

**End of SPI Flash Memory Guide**

This document covers Flash memory which is ideal for large data storage, firmware, and logging - a perfect complement to EEPROM for small configuration data!
