# Dynamic Logging System - Complete Implementation Guide

## Project Overview

Dynamic logging is a critical feature in embedded systems for real-time debugging, monitoring, and performance analysis. This project implements a sophisticated logging system with configurable log levels, runtime level adjustment, multiple output channels, timestamp integration, and memory-efficient buffering.

## Learning Objectives

- Implement runtime configurable logging levels
- Create efficient circular buffer for log storage
- Integrate timestamp functionality
- Support multiple output channels (UART, Flash, SD-Card)
- Implement log filtering and level management
- Handle concurrent logging in RTOS environments

---

## Table of Contents

1. [Theory & Concepts](#theory--concepts)
2. [Architecture Design](#architecture-design)
3. [Code Implementation](#code-implementation)
4. [Usage Examples](#usage-examples)
5. [Interview Q&A](#interview-qa)

---

## Theory & Concepts

### Logging Levels

Logging systems use severity levels to categorize messages:

| Level | Value | Purpose | Example |
|-------|-------|---------|---------|
| DEBUG | 0 | Detailed diagnostic info | Variable values, function entry/exit |
| INFO | 1 | General information | System startup, state changes |
| WARN | 2 | Warning messages | Potential issues, anomalies |
| ERROR | 3 | Error conditions | Failed operations, exceptions |
| FATAL | 4 | Critical failures | System halt required |
| OFF | 5 | Disable all logging | Testing, performance measurement |

### Logging Strategies

**1. Ring Buffer (Circular Buffer)**
- Fixed memory footprint
- Efficient memory usage
- Overwrites oldest logs when full
- Fast insertion and retrieval

**2. Timestamping**
- Identifies when events occur
- Based on system tick or RTC
- Helps correlate events across subsystems

**3. Filtering**
- Reduces output based on level
- Prioritizes important messages
- Saves bandwidth and storage

**4. Multiple Channels**
- UART: Real-time monitoring
- Flash: Persistent storage
- Memory: High-speed buffering
- File: SD-Card logging

---

## Architecture Design

### System Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                     Application Layer                            │
│  (Various modules calling LOG_DEBUG, LOG_INFO, LOG_ERROR, etc)   │
└─────────────────┬───────────────────────────────────────────────┘
                  │
                  ▼
         ┌────────────────────┐
         │  Logging Facade    │
         │ (log_debug, etc)   │
         └─────────┬──────────┘
                   │
    ┌──────────────┼──────────────┐
    │              │              │
    ▼              ▼              ▼
┌────────┐   ┌─────────┐   ┌────────────┐
│ Level  │   │ Circular│   │ Timestamp  │
│Filter  │   │ Buffer  │   │ Generator  │
└───┬────┘   └────┬────┘   └─────┬──────┘
    │             │              │
    └──────┬──────┴──────┬───────┘
           ▼             ▼
      ┌──────────────────────────┐
      │  Log Processing Engine   │
      │  (Format, Filter, Store) │
      └─────┬───────────┬───┬────┘
            │           │   │
      ┌─────▼─┐   ┌─────▼─┐ └─────────────┐
      │ UART  │   │ Flash │       │Network
      │Output │   │Output │       │Output
      └───────┘   └───────┘ ┌──────────┐
                            │SD Card   │
                            │Output    │
                            └──────────┘
```

### Buffer Structure

```
┌────────────────────────────────────────────────────────┐
│ Circular Buffer (Fixed Size = 4KB typical)             │
├────────────────────────────────────────────────────────┤
│ [0]  [1]  [2] ... [4094] [0]  [1]  [2] ...  [4094]    │
│  ↑                        ↑                    ↑        │
│  └── Write Pointer        └─ Read Pointer     EOF      │
│                                                         │
│ When Write catches Read: Buffer Full                   │
└────────────────────────────────────────────────────────┘
```

---

## Code Implementation

### Header File: dynamic_logger.h

```c
#ifndef DYNAMIC_LOGGER_H
#define DYNAMIC_LOGGER_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/* Log Level Definitions */
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_ERROR = 3,
    LOG_LEVEL_FATAL = 4,
    LOG_LEVEL_OFF = 5
} log_level_t;

/* Log Output Channels */
typedef enum {
    LOG_CHANNEL_UART = (1 << 0),     // 0x01
    LOG_CHANNEL_FLASH = (1 << 1),    // 0x02
    LOG_CHANNEL_SDCARD = (1 << 2),   // 0x04
    LOG_CHANNEL_MEMORY = (1 << 3),   // 0x08
    LOG_CHANNEL_ALL = 0x0F
} log_channel_t;

/* Configuration Constants */
#define LOG_BUFFER_SIZE                 4096
#define LOG_MAX_MESSAGE_LENGTH          256
#define LOG_MAX_ENTRIES                 64
#define LOG_TIMESTAMP_FORMAT_LENGTH     20

/* Log Entry Structure */
typedef struct {
    uint32_t timestamp_ms;           // System tick when logged
    log_level_t level;               // Log severity level
    char message[LOG_MAX_MESSAGE_LENGTH];  // Log message
} log_entry_t;

/* Public Function Prototypes */

/**
 * @brief Initialize the logging system
 * @param initial_level: Initial log level
 * @param channels: Output channels to enable
 * @return Status code
 */
int LOG_Init(log_level_t initial_level, log_channel_t channels);

/**
 * @brief Set the current log level
 * @param level: New log level
 */
void LOG_SetLevel(log_level_t level);

/**
 * @brief Get the current log level
 * @return Current log level
 */
log_level_t LOG_GetLevel(void);

/**
 * @brief Enable/disable specific output channels
 * @param channels: Bitmask of channels to modify
 * @param enable: true to enable, false to disable
 */
void LOG_SetChannels(log_channel_t channels, bool enable);

/**
 * @brief Main logging function (internal use)
 * @param level: Message severity level
 * @param format: printf-style format string
 * @param args: Variable arguments
 */
void LOG_Printf(log_level_t level, const char *format, ...);

/* Convenience Macros */
#define LOG_DEBUG(fmt, ...)   LOG_Printf(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)    LOG_Printf(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)    LOG_Printf(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)   LOG_Printf(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...)   LOG_Printf(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)

/**
 * @brief Retrieve log from circular buffer
 * @param index: Log index (0 = oldest, -1 = newest)
 * @param entry: Pointer to store log entry
 * @return true if successful, false if invalid index
 */
bool LOG_GetEntry(int32_t index, log_entry_t *entry);

/**
 * @brief Get total number of logged entries
 * @return Number of entries in buffer
 */
uint32_t LOG_GetEntryCount(void);

/**
 * @brief Dump all logs to specified output
 * @param output_channel: Where to dump logs
 */
void LOG_DumpAll(log_channel_t output_channel);

/**
 * @brief Clear all logs from buffer
 */
void LOG_Clear(void);

/**
 * @brief Enable/disable timestamp in logs
 * @param enable: true to include timestamp, false otherwise
 */
void LOG_EnableTimestamp(bool enable);

#endif /* DYNAMIC_LOGGER_H */
```

### Source File: dynamic_logger.c

```c
#include "dynamic_logger.h"
#include "uart.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include "hal_systick.h"

/* Private Data Structures */
typedef struct {
    log_entry_t entries[LOG_MAX_ENTRIES];
    uint32_t write_idx;
    uint32_t read_idx;
    uint32_t total_count;
    bool is_full;
} log_circular_buffer_t;

typedef struct {
    log_level_t current_level;
    log_channel_t enabled_channels;
    bool use_timestamp;
    log_circular_buffer_t buffer;
} logger_context_t;

/* Global Logger Context */
static logger_context_t g_logger = {
    .current_level = LOG_LEVEL_DEBUG,
    .enabled_channels = LOG_CHANNEL_UART,
    .use_timestamp = true,
    .buffer = {
        .write_idx = 0,
        .read_idx = 0,
        .total_count = 0,
        .is_full = false
    }
};

/* Private Function Prototypes */
static const char* LOG_LevelToString(log_level_t level);
static void LOG_OutputToChannel(const log_entry_t *entry, 
                                 log_channel_t channel);
static void LOG_OutputToUART(const log_entry_t *entry);
static void LOG_OutputToFlash(const log_entry_t *entry);
static void LOG_FormatMessage(char *buffer, size_t size, 
                               const log_entry_t *entry);

/**
 * @brief Convert log level to human-readable string
 */
static const char* LOG_LevelToString(log_level_t level)
{
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO ";
        case LOG_LEVEL_WARN:  return "WARN ";
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_FATAL: return "FATAL";
        case LOG_LEVEL_OFF:   return "OFF  ";
        default:              return "????";
    }
}

/**
 * @brief Format log entry into printable string
 */
static void LOG_FormatMessage(char *buffer, size_t size, 
                               const log_entry_t *entry)
{
    if (buffer == NULL || size == 0) return;
    
    int len = 0;
    
    // Add timestamp if enabled
    if (g_logger.use_timestamp) {
        len = snprintf(buffer, size, "[%10lu] ", entry->timestamp_ms);
    }
    
    // Add level
    len += snprintf(buffer + len, size - len, "[%-5s] ", 
                   LOG_LevelToString(entry->level));
    
    // Add message
    len += snprintf(buffer + len, size - len, "%s\r\n", 
                   entry->message);
}

/**
 * @brief Output log entry to UART
 */
static void LOG_OutputToUART(const log_entry_t *entry)
{
    char formatted[LOG_MAX_MESSAGE_LENGTH + 64];
    LOG_FormatMessage(formatted, sizeof(formatted), entry);
    UART_SendString(UART1, formatted);
}

/**
 * @brief Output log entry to Flash memory
 */
static void LOG_OutputToFlash(const log_entry_t *entry)
{
    // Implementation would store to flash storage
    // For now, placeholder
    // FLASH_Write(FLASH_LOG_SECTOR, entry, sizeof(log_entry_t));
}

/**
 * @brief Route log to specified output channel
 */
static void LOG_OutputToChannel(const log_entry_t *entry, 
                                 log_channel_t channel)
{
    switch (channel) {
        case LOG_CHANNEL_UART:
            LOG_OutputToUART(entry);
            break;
        case LOG_CHANNEL_FLASH:
            LOG_OutputToFlash(entry);
            break;
        case LOG_CHANNEL_MEMORY:
            // Already in circular buffer
            break;
        case LOG_CHANNEL_SDCARD:
            // Write to SD card
            break;
        default:
            break;
    }
}

/**
 * @brief Initialize logging system
 */
int LOG_Init(log_level_t initial_level, log_channel_t channels)
{
    g_logger.current_level = initial_level;
    g_logger.enabled_channels = channels;
    
    // Initialize circular buffer
    memset(&g_logger.buffer.entries, 0, sizeof(g_logger.buffer.entries));
    g_logger.buffer.write_idx = 0;
    g_logger.buffer.read_idx = 0;
    g_logger.buffer.total_count = 0;
    g_logger.buffer.is_full = false;
    
    LOG_INFO("Logging system initialized (Level: %s)", 
             LOG_LevelToString(initial_level));
    
    return 0;
}

/**
 * @brief Set log level
 */
void LOG_SetLevel(log_level_t level)
{
    if (level < LOG_LEVEL_OFF) {
        g_logger.current_level = level;
        LOG_INFO("Log level changed to: %s", 
                LOG_LevelToString(level));
    }
}

/**
 * @brief Get current log level
 */
log_level_t LOG_GetLevel(void)
{
    return g_logger.current_level;
}

/**
 * @brief Set output channels
 */
void LOG_SetChannels(log_channel_t channels, bool enable)
{
    if (enable) {
        g_logger.enabled_channels |= channels;
    } else {
        g_logger.enabled_channels &= ~channels;
    }
}

/**
 * @brief Enable/disable timestamps
 */
void LOG_EnableTimestamp(bool enable)
{
    g_logger.use_timestamp = enable;
}

/**
 * @brief Main logging function
 */
void LOG_Printf(log_level_t level, const char *format, ...)
{
    // Check if message should be logged
    if (level < g_logger.current_level || level >= LOG_LEVEL_OFF) {
        return;
    }
    
    if (format == NULL) {
        return;
    }
    
    // Create log entry
    log_entry_t entry;
    entry.level = level;
    entry.timestamp_ms = HAL_GetTick();
    
    // Format message
    va_list args;
    va_start(args, format);
    vsnprintf(entry.message, LOG_MAX_MESSAGE_LENGTH, format, args);
    va_end(args);
    
    // Store in circular buffer
    g_logger.buffer.entries[g_logger.buffer.write_idx] = entry;
    
    // Advance write pointer
    g_logger.buffer.write_idx++;
    if (g_logger.buffer.write_idx >= LOG_MAX_ENTRIES) {
        g_logger.buffer.write_idx = 0;
        g_logger.buffer.is_full = true;
    }
    
    // Update read pointer if buffer is full
    if (g_logger.buffer.is_full) {
        g_logger.buffer.read_idx++;
        if (g_logger.buffer.read_idx >= LOG_MAX_ENTRIES) {
            g_logger.buffer.read_idx = 0;
        }
    } else {
        g_logger.buffer.total_count++;
    }
    
    // Output to enabled channels
    if (g_logger.enabled_channels & LOG_CHANNEL_UART) {
        LOG_OutputToChannel(&entry, LOG_CHANNEL_UART);
    }
    
    if (g_logger.enabled_channels & LOG_CHANNEL_FLASH) {
        LOG_OutputToChannel(&entry, LOG_CHANNEL_FLASH);
    }
}

/**
 * @brief Get log entry by index
 */
bool LOG_GetEntry(int32_t index, log_entry_t *entry)
{
    if (entry == NULL) {
        return false;
    }
    
    uint32_t actual_idx;
    
    if (index >= 0) {
        // Index from oldest entry
        if (g_logger.buffer.is_full) {
            actual_idx = (g_logger.buffer.read_idx + index) % LOG_MAX_ENTRIES;
            if (index >= LOG_MAX_ENTRIES) {
                return false;
            }
        } else {
            if (index >= g_logger.buffer.total_count) {
                return false;
            }
            actual_idx = index;
        }
    } else {
        // Index from newest entry (negative)
        uint32_t count = LOG_GetEntryCount();
        if ((uint32_t)(-index) > count) {
            return false;
        }
        actual_idx = (g_logger.buffer.write_idx - 1 + index + 
                     LOG_MAX_ENTRIES) % LOG_MAX_ENTRIES;
    }
    
    *entry = g_logger.buffer.entries[actual_idx];
    return true;
}

/**
 * @brief Get total entry count
 */
uint32_t LOG_GetEntryCount(void)
{
    if (g_logger.buffer.is_full) {
        return LOG_MAX_ENTRIES;
    }
    return g_logger.buffer.total_count;
}

/**
 * @brief Dump all logs
 */
void LOG_DumpAll(log_channel_t output_channel)
{
    uint32_t count = LOG_GetEntryCount();
    
    for (uint32_t i = 0; i < count; i++) {
        log_entry_t entry;
        if (LOG_GetEntry(i, &entry)) {
            LOG_OutputToChannel(&entry, output_channel);
        }
    }
}

/**
 * @brief Clear all logs
 */
void LOG_Clear(void)
{
    memset(&g_logger.buffer.entries, 0, sizeof(g_logger.buffer.entries));
    g_logger.buffer.write_idx = 0;
    g_logger.buffer.read_idx = 0;
    g_logger.buffer.total_count = 0;
    g_logger.buffer.is_full = false;
}
```

### Usage Example

```c
#include "dynamic_logger.h"

int main(void)
{
    // Initialize logging system
    LOG_Init(LOG_LEVEL_DEBUG, LOG_CHANNEL_UART | LOG_CHANNEL_MEMORY);
    
    LOG_INFO("Application started");
    
    // Change log level dynamically
    if (debug_mode) {
        LOG_SetLevel(LOG_LEVEL_DEBUG);
        LOG_DEBUG("Debug mode enabled");
    } else {
        LOG_SetLevel(LOG_LEVEL_INFO);
        LOG_DEBUG("This won't be logged");  // Below threshold
    }
    
    // Log with various levels
    LOG_DEBUG("Variable x = %d, y = %f", 42, 3.14);
    LOG_INFO("Device initialized successfully");
    LOG_WARN("Battery voltage low: %duV", 3200000);
    LOG_ERROR("I2C communication failed, device 0x%02X", 0xA0);
    
    // Retrieve logs
    log_entry_t entry;
    for (int i = 0; i < LOG_GetEntryCount(); i++) {
        if (LOG_GetEntry(i, &entry)) {
            printf("[%s] %s\n", 
                   LOG_LevelToString(entry.level), 
                   entry.message);
        }
    }
    
    // Dump all logs to UART
    LOG_DumpAll(LOG_CHANNEL_UART);
    
    return 0;
}
```

---

## Advanced Features

### Thread-Safe Logging (RTOS)

```c
#include "cmsis_os.h"

static osMutexId g_log_mutex;

void LOG_PrintfThreadSafe(log_level_t level, const char *format, ...)
{
    osMutexWait(g_log_mutex, osWaitForever);
    
    // Format and log message
    va_list args;
    va_start(args, format);
    // ... logging implementation ...
    va_end(args);
    
    osMutexRelease(g_log_mutex);
}
```

### Log with Conditional Filtering

```c
// Log only modules that interest us
#define LOG_FILTER_MODULE(module) \
    if (g_enabled_modules & MODULE_##module)

void LOG_ModuleDebug(uint32_t module, const char *format, ...)
{
    LOG_FILTER_MODULE(module) {
        va_list args;
        va_start(args, format);
        LOG_Printf(LOG_LEVEL_DEBUG, format, args);
        va_end(args);
    }
}
```

---

## Interview Q&A

### Q1: What is dynamic logging and why is it important in embedded systems?

**Answer**:
Dynamic logging is a flexible logging system that allows runtime adjustment of logging behavior without recompilation. It's important because:

1. **Development Phase**:
   - Enable DEBUG logs for detailed diagnostics
   - Quickly identify bugs and understand code flow
   - Reduce iteration time between compile and debug

2. **Production Phase**:
   - Disable DEBUG logs to save bandwidth/power
   - Keep ERROR/FATAL logs for issue tracking
   - Adjust levels based on field issues

3. **System Monitoring**:
   - Real-time visibility into system operation
   - Performance analysis and profiling
   - Anomaly detection

4. **Cost Savings**:
   - Minimal code duplication (vs multiple builds)
   - Reduced storage with selective logging
   - Lower power consumption

### Q2: Explain the circular buffer concept and its advantages.

**Answer**:
A circular (ring) buffer is a fixed-size buffer where write/read pointers wrap around:

```
Initial:    W    R
           [_][_][_][_][_]
           
After 3:    W         R
writes:    [A][B][C][_][_]

After 5:        W     R
writes:    [A][B][C][D][E]

After 7:    W         R
overwrites:[F][B][C][D][E]
             ↑
        Oldest entry
```

**Advantages**:
- **Fixed Memory**: No dynamic allocation, predictable RAM usage
- **Fast Operations**: O(1) insertion/retrieval
- **Overflow Handling**: Automatically discards oldest entries
- **Real-Time Safe**: No blocking, deterministic timing

**Disadvantages**:
- **Data Loss**: Oldest logs overwritten
- **Fixed Size**: Must estimate maximum needed logs

### Q3: How would you make the logging system thread-safe in an RTOS?

**Answer**:
Use mutex to serialize access to the circular buffer:

```c
static osMutexId g_log_mutex;

void LOG_Init(...) {
    g_log_mutex = osMutexCreate(osMutex(osMutexRecursive));
}

void LOG_Printf(log_level_t level, const char *format, ...) {
    if (osMutexWait(g_log_mutex, osWaitForever) != osOK) 
        return;
    
    // Protected section - only one task at a time
    va_list args;
    va_start(args, format);
    // ... write to buffer ...
    va_end(args);
    
    osMutexRelease(g_log_mutex);
}
```

**Why necessary**:
- Without mutex, multiple tasks might corrupt buffer pointers
- Race conditions on write_idx updates
- Potential for reading partial/corrupted entries

### Q4: How does timestamp integration improve log analysis?

**Answer**:
Timestamps enable several analysis techniques:

1. **Event Correlation**: Link events across multiple systems
   ```
   [00100] Task A started measurement
   [00120] Sensor ADC value: 2048
   [00145] Calculation complete: 15°C
   ```

2. **Performance Analysis**: Measure operation duration
   ```
   [10000] Function_X started
   [10045] Function_X ended  // 45ms execution time
   ```

3. **Root Cause Analysis**: Find event sequence that caused failure
   ```
   [50000] Temperature warning
   [50050] Fan activated
   [50075] Fan failed  // Likely related
   ```

4. **Debugging State Machines**: Track state transitions with timing

**Implementation**:
- Use HAL_GetTick() for relative timestamps
- Use RTC for absolute timestamps
- Store both for flexibility

### Q5: What strategies optimize logging for low-power embedded systems?

**Answer**:

1. **Level-Based Control**: Disable DEBUG/INFO in production
   ```c
   #ifdef DEBUG_BUILD
       LOG_SetLevel(LOG_LEVEL_DEBUG);
   #else
       LOG_SetLevel(LOG_LEVEL_ERROR);
   #endif
   ```

2. **Conditional Compilation**: Remove logging code entirely
   ```c
   #define LOG_DEBUG(...) do {} while(0)  // No code generated
   ```

3. **Buffered Output**: Write in batches, not character-by-character
   ```c
   // Bad: 50 UART interrupts for 50 bytes
   for (int i = 0; i < 50; i++)
       UART_SendByte(data[i]);
   
   // Good: 1 DMA transfer
   UART_SendDMA(data, 50);
   ```

4. **Selective Output**: Only enable essential channels
   ```c
   LOG_SetChannels(LOG_CHANNEL_FLASH, true);   // Persistent storage
   LOG_SetChannels(LOG_CHANNEL_UART, false);   // Disable real-time
   ```

5. **CBuffer with Overflow**: Stop logging when full instead of overwriting
   ```c
   if (is_buffer_full) {
       stop_logging();  // Preserve last state
   }
   ```

---

## Performance Metrics

| Operation | Time | Memory |
|-----------|------|--------|
| LOG_Printf (simple) | ~50-100µs | 64 bytes entry |
| Circular buffer insert | O(1) | Fixed 4KB |
| LOG_GetEntry(idx) | O(1) | 0 bytes |
| LOG_DumpAll(64 entries) | ~5-10ms | 0 bytes |

---

## Common Issues & Solutions

| Issue | Cause | Solution |
|-------|-------|----------|
| Missing logs | Buffer overflow | Increase buffer size or reduce message frequency |
| Performance degradation | High log volume | Reduce log level or use selective output |
| Corrupted timestamps | Timer overflow | Use 32-bit counter or handle wraparound |
| Thread safety issues | Race conditions | Add mutex protection |
| Memory not freed | Leak in formatter | Check sprintf buffer boundaries |

---

**End of Dynamic Logging Guide**

Continue with the remaining 8 projects...
