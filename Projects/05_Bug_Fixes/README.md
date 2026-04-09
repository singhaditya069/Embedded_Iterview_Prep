# Bug Fixes & Debugging - Complete Implementation Guide

## Project Overview

This project documents systematic approaches to identifying, diagnosing, and fixing bugs in embedded systems. Effective debugging is a crucial skill for senior embedded engineers.

---

## Debugging Methodology

### 1. Bug Identification Phase

**Symptoms to Investigate**:
- System crashes/hangs
- Incorrect output values
- Timing issues (too fast/slow)
- Memory corruption
- Power consumption anomalies
- Intermittent failures

**Data Collection**:
```c
/* Add diagnostic logging */
#define DEBUG_LOG(fmt, ...) \
    do { \
        printf("[%s:%d] " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while(0)

/* Monitor critical variables */
volatile uint32_t debug_timestamp;
volatile uint32_t debug_gpio_state;
volatile uint32_t debug_adc_value;
```

### 2. Root Cause Analysis Techniques

**A. Binary Search (Divide & Conquer)**
```
1. Disable half the code
2. Does bug still exist?
   - Yes: Bug in enabled half
   - No: Bug in disabled half
3. Repeat with half
```

**B. Timeline Analysis**
```
Correlate events with timestamps:
[1000ms] ADC reading: 2048
[1005ms] Threshold crossed
[1010ms] LED toggled
[1015ms] I2C write failed  ← Likely cause
[1020ms] Relay stuck
```

**C. State Machine Validation**
```c
/* Track state transitions */
#define STATE_LOG(old, new) \
    printf("[%lu] State: %s -> %s\n", HAL_GetTick(), \
           state_to_string(old), state_to_string(new))
```

### 3. Common Bug Categories

| Category | Cause | Detection | Fix |
|----------|-------|-----------|-----|
| **Memory Issues** | Buffer overflow, Use-after-free | ASAN, Valgrind | Bounds checking |
| **Timing Issues** | Race conditions, Deadlocks | Logic analyzer | Synchronization |
| **Hardware Handshake** | Wrong GPIO/clock timing | Oscilloscope | Protocol analysis |
| **Interrupt Issues** | Missed interrupts, Priority | Interrupt counter | ISR analysis |
| **Floating Point** | Precision loss, NaN propagation | Unit tests | Fixed-point math |

---

## Common Bug Patterns & Solutions

### Pattern 1: Uninitialized Variables

**Bug**:
```c
void sensor_read(void) {
    uint16_t value;  // Uninitialized!
    ADC_Read(&value);
    if (value > threshold) ...  // Garbage value!
}
```

**Fix**:
```c
void sensor_read(void) {
    uint16_t value = 0;  // Explicitly initialize
    ADC_Read(&value);
    if (value > threshold) ...
}
```

### Pattern 2: Off-by-One Errors

**Bug**:
```c
for (int i = 0; i < ARRAY_SIZE; i++)  // i goes 0 to SIZE-1 OK
    data[i] = buffer[i];  // But...

if (latest_index >= ARRAY_SIZE)  // Wrong!
    latest_index = 0;
```

**Fix**:
```c
for (int i = 0; i < ARRAY_SIZE; i++)
    data[i] = buffer[i];

if (latest_index >= ARRAY_SIZE)  // Correct
    latest_index = 0;
    
// Or use modulo
latest_index = (latest_index + 1) % ARRAY_SIZE;
```

### Pattern 3: NULL Pointer Dereference

**Bug**:
```c
device_t *dev = find_device(id);
dev->state = ACTIVE;  // Crashes if dev==NULL!
```

**Fix**:
```c
device_t *dev = find_device(id);
if (dev == NULL) {
    LOG_ERROR("Device not found");
    return ERROR;
}
dev->state = ACTIVE;
```

### Pattern 4: Missing Function Initialization

**Bug**:
```c
void main(void) {
    result = I2C_Read(...);  // I2C not initialized!
}
```

**Fix**:
```c
void main(void) {
    I2C_Init();          // Initialize first
    result = I2C_Read(...);
}
```

### Pattern 5: Interrupt Race Condition

**Bug**:
```c
volatile uint16_t sensor_value = 0;

void ADC_IRQ(void) {
    sensor_value = ADC_GetResult();
}

void main(void) {
    uint16_t local = sensor_value;  // May change mid-assignment!
}
```

**Fix**:
```c
volatile uint16_t sensor_value = 0;

void ADC_IRQ(void) {
    sensor_value = ADC_GetResult();
}

void main(void) {
    DisableInterrupts();
    uint16_t local = sensor_value;  // Atomic read
    EnableInterrupts();
}

// Or use volatile with care
void main(void) {
    uint16_t local = sensor_value;  // Compiler handles atomicity
}
```

### Pattern 6: Resource Leak

**Bug**:
```c
for (int i = 0; i < count; i++) {
    buffer_t *buf = malloc(SIZE);
    if (process(buf) == ERROR)
        return ERROR;  // Memory leaked!
    free(buf);
}
```

**Fix**:
```c
for (int i = 0; i < count; i++) {
    buffer_t *buf = malloc(SIZE);
    if (process(buf) == ERROR) {
        free(buf);  // Clean up before return
        return ERROR;
    }
    free(buf);
}

// Or use stack allocation
buffer_t buf[SIZE];
if (process(buf) == ERROR)
    return ERROR;  // No leak
```

---

## Debugging Tools & Techniques

### 1. Debugger Usage

```gdb
(gdb) break main
(gdb) run
(gdb) step           # Step into
(gdb) next           # Step over
(gdb) continue       # Run to next breakpoint
(gdb) print variable # Inspect value
(gdb) watch variable # Stop when changes
```

### 2. Logic Analyzer

```
Capture protocol timing:
SDA: ─────┐─┐───────┐─┐─────────┐─┬─────
SCL: ─────┐─┴─┐─────┐─┴─┐───────┐─┴─────
          S   A     A   A       N S

S = START
A = ACK
N = NACK
```

### 3. Oscilloscope Measurements

```
Measuring SPI timing:
CS:   ─────────┐───────────────────────┐─────
SCLK: ────────┬┴┬┴┬┴┬┴┬┴┬┴┬┴┬┴┬─────────┬───
MOSI: ───[D7 D6 D5 D4 D3 D2 D1 D0]─────────
MISO: ────────────[D7 D6 D5 D4 D3 D2 D1 D0]

Measure setup times, hold times, clock period
```

### 4. Assertion-Based Debugging

```c
#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            LOG_FATAL("ASSERT FAILED: %s", message); \
            while(1);  /* Halt for debugging */ \
        } \
    } while(0)

ASSERT(sensor_value > 0, "Sensor not reading");
ASSERT(config_ptr != NULL, "Config not initialized");
```

---

## Real-World Bug Examples

### Example 1: SPI Read Corruption

**Symptom**: EEPROM reads return garbage

**Investigation**:
```c
// Monitor all reads
printf("ADDR: 0x%04X, Data: ", address);
for (int i = 0; i < 4; i++)
    printf("%02X ", data[i]);
printf("\n");

// Results show pattern
ADDR: 0x0000, Data: 12 34 56 78  // Good
ADDR: 0x0004, Data: ?? ?? ?? ??  // Bad
```

**Root Cause**: CS timing wrong - toggling too early

**Fix**:
```c
void CS_Low(void)  {
    GPIO_WritePin(GPIOA, PIN, LOW);
    delay_us(10);  // Wait for slave to react
}

void CS_High(void) {
    delay_us(10);  // Wait before deselect
    GPIO_WritePin(GPIOA, PIN, HIGH);
}
```

### Example 2: Intermittent I2C Failures

**Symptom**: Random I2C timeouts

**Investigation**:
```c
// Log every I2C attempt
i2c_status = I2C_Read(...);
if (i2c_status != I2C_OK) {
    failures++;
    if (failures > threshold)
        enter_error_state();
}

// Correlation: failures spike at specific times
```

**Root Cause**: Stack overflow corrupting I2C state machine

**Fix**:
```c
// Increase stack size in linker script
/* From */  __initial_sp = 0x20005000;  /* 20KB */
/* To   */  __initial_sp = 0x20008000;  /* 32KB */
```

### Example 3: ADC Noise Issues

**Symptom**: ADC readings fluctuate wildly

**Investigation**:
```
No filter:
  2048, 2051, 2045, 2049, 2046, 2050  // Zig-zag pattern

Average filter (N=16):
  2047.5, 2048.1, 2048.3  // Stable!
```

**Root Cause**: Ground plane insufficient, noise coupling

**Fix**:
```c
// Software filter
#define FILTER_SIZE 16
uint16_t AdcFilteredRead(void) {
    uint32_t sum = 0;
    for (int i = 0; i < FILTER_SIZE; i++) {
        sum += ADC_Read();
    }
    return sum / FILTER_SIZE;
}

// Hardware: Add capacitor on ADC input
```

---

## Interview Q&A

### Q1: Walk through your debugging process for a mysterious hardware failure.

**Answer**:

**Phase 1: Reproduce**
- Determine if failure is deterministic or random
- Document exact conditions (temperature, voltage, time)
- Collect failing unit for inspection

**Phase 2: Isolate**
```c
// Disable subsystems one by one
disable_wifi();     // Still fail?
disable_ble();      // Still fail?
disable_sensors();  // Still fail?
// Narrows down culprit
```

**Phase 3: Investigate**
- Add logging at critical points
- Measure voltage/current with multimeter
- Use oscilloscope for timing issues
- Check temperature with thermal camera

**Phase 4: Verify Fix**
- Implement minimal change
- Run 1000+ cycles to confirm
- Test edge cases (temperature extremes, etc.)

**Phase 5: Document**
- Record root cause
- Update design documentation
- Add unit test to prevent regression

### Q2: How do you debug timing-critical code?

**Answer**:
```c
/* Technique 1: Toggling GPIO for timing analysis */
#define TIMING_START() GPIO_WritePin(GPIOA, PIN, HIGH)
#define TIMING_END()   GPIO_WritePin(GPIOA, PIN, LOW)

void critical_function(void) {
    TIMING_START();
    // ... code ...
    TIMING_END();
}
// Measure with oscilloscope

/* Technique 2: Interrupt counter */
volatile uint32_t interrupt_count = 0;
void ISR(void) {
    interrupt_count++;
}

// Check count periodically - should increment predictably

/* Technique 3: CRC verification */
void process_data(uint8_t *data, size_t len) {
    uint32_t crc_before = CRC32(data, len);
    
    // Processing
    // ...
    
    uint32_t crc_after = CRC32(data, len);
    ASSERT(crc_before == crc_after, "Data corrupted");
}
```

---

**End of Bug Fixes Guide**
