# PIR Sensor & ADC Integration - Complete Implementation Guide

## Project Overview

This project implements motion detection using a Passive Infrared (PIR) sensor interfaced through an Analog-to-Digital Converter (ADC). PIR sensors detect infrared radiation emitted by moving objects and are widely used in security systems, smart home automation, and occupancy detection.

## Learning Objectives

- Understand PIR sensor operation and characteristics
- Implement ADC conversion for analog sensor readings
- Design debouncing algorithms for noisy signals
- Handle ADC interrupts and DMA transfers
- Create motion detection state machine
- Implement threshold-based detection logic

---

## Table of Contents

1. [Theory & Concepts](#theory--concepts)
2. [Hardware Architecture](#hardware-architecture)
3. [Code Implementation](#code-implementation)
4. [Advanced Features](#advanced-features)
5. [Interview Q&A](#interview-qa)

---

## Theory & Concepts

### PIR Sensor Basics

**How PIR Works**:
- Detects infrared radiation (heat)
- Two element design (differential measurement)
- Filters out ambient changes
- Sensitive to movement (changing heat patterns)

**Characteristics**:
- Operating Voltage: 4.5V - 20V (typical 12V)
- Output: Analog voltage proportional to motion intensity
- Warm-up Time: 30-60 seconds
- Detection Range: 5-10 meters
- Field of View: 40-100 degrees

### ADC Conversion Basics

**ADC Parameters**:
- **Resolution**: How many discrete values (8-bit, 10-bit, 12-bit, etc.)
- **Sampling Rate**: Conversions per second
- **Reference Voltage**: Maximum voltage (0-3.3V, 0-5V, etc.)
- **Conversion Time**: Time for one measurement

**ADC Formula**:
```
Digital Value = (Analog Voltage / Reference Voltage) × (2^Resolution - 1)

For 12-bit ADC with 3.3V reference:
Digital = (Vain / 3.3) × 4095
```

### Signal Conditioning

**Noise Sources**:
- Electrical noise from power supply
- Sensor thermal noise
- Environmental interference

**Conditioning Techniques**:
1. **Low-Pass Filtering**: Hardware RC filter
2. **Averaging**: Multiple samples
3. **Debouncing**: Wait for stable readings
4. **Hysteresis**: Different thresholds for rise/fall

---

## Hardware Architecture

### Connection Diagram

```
PIR Sensor
    │
    ├─ Signal (Analog)
    │      │
    │      ▼
    │   [RC Filter] ──────┐
    ├─ GND                │
    │                     ▼
    └─ VCC (12V)      ADC Channel X
                           │
                           ▼
                        ┌──────────┐
                        │   MCU    │
                        │  ADC Mod │
                        └──┬───────┘
                           │
                    Software Processing:
                    • Raw ADC read
                    • Filtering & Averaging
                    • Threshold detection
                    • Debouncing
                    • State machine
```

### Pin Configuration

**STM32 MCU**:
- PA0: ADC Channel 0 (PIR analog input)
- PC0: Digital output (Motion detected)

**PIR Sensor**:
- Pin 1: GND (Black wire)
- Pin 2: Analog Output (Yellow wire)
- Pin 3: VCC 12V (Red wire)

---

## Code Implementation

### Header File: pir_sensor.h

```c
#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H

#include <stdint.h>
#include <stdbool.h>

/* PIR Configuration */
#define PIR_ADC_CHANNEL                1
#define PIR_WARMUP_TIME_MS             60000   // 60 seconds
#define PIR_SAMPLE_RATE_HZ             100     // 100 samples/sec
#define PIR_FILTER_SIZE                10      // Moving average samples
#define PIR_MOTION_THRESHOLD           500     // ADC value threshold
#define PIR_DEBOUNCE_TIME_MS           50      // 50ms debounce

/* PIR Detection States */
typedef enum {
    PIR_STATE_WARMUP = 0,
    PIR_STATE_IDLE,
    PIR_STATE_DETECTING,
    PIR_STATE_CONFIRMED
} pir_state_t;

/* Motion Event Callback */
typedef void (*pir_callback_t)(bool motion_detected);

/* Status Codes */
typedef enum {
    PIR_OK = 0,
    PIR_NOT_READY,
    PIR_INIT_FAILED,
    PIR_ADC_ERROR,
    PIR_TIMEOUT
} pir_status_t;

/* Public Function Prototypes */

/**
 * @brief Initialize PIR sensor and ADC
 * @return Status code
 */
pir_status_t PIR_Init(void);

/**
 * @brief Start ADC conversion for PIR reading
 * @return Status code
 */
pir_status_t PIR_StartConversion(void);

/**
 * @brief Get raw ADC value
 * @return ADC value (0-4095 for 12-bit)
 */
uint16_t PIR_GetRawValue(void);

/**
 * @brief Get filtered PIR value
 * @return Filtered value
 */
uint16_t PIR_GetFilteredValue(void);

/**
 * @brief Check if motion detected
 * @return true if motion detected, false otherwise
 */
bool PIR_IsMotionDetected(void);

/**
 * @brief Get current PIR state
 * @return Current state
 */
pir_state_t PIR_GetState(void);

/**
 * @brief Register motion detection callback
 * @param callback: Function to call on state change
 */
void PIR_RegisterCallback(pir_callback_t callback);

/**
 * @brief Set motion detection threshold
 * @param threshold: New threshold value
 */
void PIR_SetThreshold(uint16_t threshold);

/**
 * @brief PIR main processing function (call periodically)
 * @return true if state changed, false otherwise
 */
bool PIR_Process(void);

/**
 * @brief Get warmup progress percentage
 * @return 0-100 warmup percentage
 */
uint8_t PIR_GetWarmupProgress(void);

/**
 * @brief Check if sensor is ready
 * @return true if ready for detection, false if warming up
 */
bool PIR_IsReady(void);

#endif /* PIR_SENSOR_H */
```

### Source File: pir_sensor.c

```c
#include "pir_sensor.h"
#include "adc_driver.h"
#include "gpio.h"
#include "systick.h"

/* Private Data Types */
typedef struct {
    pir_state_t current_state;
    pir_state_t previous_state;
    uint16_t raw_values[PIR_FILTER_SIZE];
    uint8_t filter_index;
    uint16_t filtered_value;
    uint32_t warmup_start_time;
    uint32_t last_state_change_time;
    uint32_t last_detection_time;
    uint16_t motion_threshold;
    bool configured;
    pir_callback_t user_callback;
} pir_context_t;

/* Global Context */
static pir_context_t g_pir = {
    .current_state = PIR_STATE_WARMUP,
    .previous_state = PIR_STATE_WARMUP,
    .filter_index = 0,
    .filtered_value = 0,
    .warmup_start_time = 0,
    .last_state_change_time = 0,
    .last_detection_time = 0,
    .motion_threshold = PIR_MOTION_THRESHOLD,
    .configured = false,
    .user_callback = NULL
};

/* Private Function Prototypes */
static uint16_t PIR_FilterValue(uint16_t raw_value);
static void PIR_UpdateState(bool motion_detected);
static void PIR_EmitCallback(bool motion_detected);
static bool PIR_IsWarmupComplete(void);

/**
 * @brief Apply moving average filter
 */
static uint16_t PIR_FilterValue(uint16_t raw_value)
{
    uint32_t sum = 0;
    
    // Store new value
    g_pir.raw_values[g_pir.filter_index] = raw_value;
    g_pir.filter_index = (g_pir.filter_index + 1) % PIR_FILTER_SIZE;
    
    // Calculate average
    for (uint8_t i = 0; i < PIR_FILTER_SIZE; i++) {
        sum += g_pir.raw_values[i];
    }
    
    return sum / PIR_FILTER_SIZE;
}

/**
 * @brief Initialize PIR sensor
 */
pir_status_t PIR_Init(void)
{
    if (g_pir.configured) {
        return PIR_OK;
    }
    
    // Initialize ADC
    adc_config_t adc_cfg = {
        .channel = PIR_ADC_CHANNEL,
        .resolution = ADC_RESOLUTION_12BIT,
        .sample_time = ADC_SAMPLE_15CYCLES,
        .trigger = ADC_TRIGGER_SW,
        .dma_enabled = false
    };
    
    if (ADC_Init(&adc_cfg) != ADC_OK) {
        return PIR_ADC_ERROR;
    }
    
    // Initialize output GPIO
    gpio_config_t gpio_cfg = {
        .pin = GPIO_PIN_0,
        .port = GPIOC,
        .mode = GPIO_MODE_OUTPUT,
        .speed = GPIO_SPEED_LOW,
        .pull = GPIO_PULL_NONE
    };
    
    GPIO_Init(&gpio_cfg);
    GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);  // Motion not detected initially
    
    // Reset filter buffer
    for (uint8_t i = 0; i < PIR_FILTER_SIZE; i++) {
        g_pir.raw_values[i] = 0;
    }
    
    g_pir.warmup_start_time = HAL_GetTick();
    g_pir.configured = true;
    
    return PIR_OK;
}

/**
 * @brief Start ADC conversion
 */
pir_status_t PIR_StartConversion(void)
{
    if (!g_pir.configured) {
        return PIR_NOT_READY;
    }
    
    return ADC_StartConversion(PIR_ADC_CHANNEL);
}

/**
 * @brief Get raw ADC value (blocking)
 */
uint16_t PIR_GetRawValue(void)
{
    return ADC_GetValue(PIR_ADC_CHANNEL);
}

/**
 * @brief Get filtered value
 */
uint16_t PIR_GetFilteredValue(void)
{
    return g_pir.filtered_value;
}

/**
 * @brief Check if warmup complete
 */
static bool PIR_IsWarmupComplete(void)
{
    uint32_t elapsed = HAL_GetTick() - g_pir.warmup_start_time;
    return elapsed >= PIR_WARMUP_TIME_MS;
}

/**
 * @brief Get warmup progress
 */
uint8_t PIR_GetWarmupProgress(void)
{
    if (PIR_IsWarmupComplete()) {
        return 100;
    }
    
    uint32_t elapsed = HAL_GetTick() - g_pir.warmup_start_time;
    return (elapsed * 100) / PIR_WARMUP_TIME_MS;
}

/**
 * @brief Check if sensor ready
 */
bool PIR_IsReady(void)
{
    return (g_pir.current_state != PIR_STATE_WARMUP);
}

/**
 * @brief Update internal state
 */
static void PIR_UpdateState(bool motion_detected)
{
    pir_state_t new_state = g_pir.current_state;
    uint32_t now = HAL_GetTick();
    
    if (!PIR_IsWarmupComplete()) {
        new_state = PIR_STATE_WARMUP;
    }
    else {
        switch (g_pir.current_state) {
            case PIR_STATE_WARMUP:
                new_state = PIR_STATE_IDLE;
                break;
                
            case PIR_STATE_IDLE:
                if (motion_detected) {
                    new_state = PIR_STATE_DETECTING;
                    g_pir.last_state_change_time = now;
                }
                break;
                
            case PIR_STATE_DETECTING:
                if (!motion_detected) {
                    new_state = PIR_STATE_IDLE;
                } 
                else if ((now - g_pir.last_state_change_time) > 
                         PIR_DEBOUNCE_TIME_MS) {
                    new_state = PIR_STATE_CONFIRMED;
                    g_pir.last_detection_time = now;
                    g_pir.last_state_change_time = now;
                    PIR_EmitCallback(true);
                }
                break;
                
            case PIR_STATE_CONFIRMED:
                if (!motion_detected &&
                    (now - g_pir.last_state_change_time) > 
                    PIR_DEBOUNCE_TIME_MS) {
                    new_state = PIR_STATE_IDLE;
                    PIR_EmitCallback(false);
                }
                break;
                
            default:
                new_state = PIR_STATE_IDLE;
                break;
        }
    }
    
    g_pir.current_state = new_state;
}

/**
 * @brief Get current state
 */
pir_state_t PIR_GetState(void)
{
    return g_pir.current_state;
}

/**
 * @brief Check if motion detected
 */
bool PIR_IsMotionDetected(void)
{
    return (g_pir.current_state == PIR_STATE_CONFIRMED);
}

/**
 * @brief Set motion threshold
 */
void PIR_SetThreshold(uint16_t threshold)
{
    g_pir.motion_threshold = threshold;
}

/**
 * @brief Register callback
 */
void PIR_RegisterCallback(pir_callback_t callback)
{
    g_pir.user_callback = callback;
}

/**
 * @brief Emit callback
 */
static void PIR_EmitCallback(bool motion_detected)
{
    if (g_pir.user_callback != NULL) {
        g_pir.user_callback(motion_detected);
    }
    
    // Update output GPIO
    GPIO_WritePin(GPIOC, GPIO_PIN_0, motion_detected ? 1 : 0);
}

/**
 * @brief Main processing function (call every 10ms ideally)
 */
bool PIR_Process(void)
{
    if (!g_pir.configured) {
        return false;
    }
    
    pir_state_t prev_state = g_pir.current_state;
    
    // Read and filter ADC value
    uint16_t raw = PIR_GetRawValue();
    g_pir.filtered_value = PIR_FilterValue(raw);
    
    // Detect motion threshold crossing
    bool motion = (g_pir.filtered_value > g_pir.motion_threshold);
    
    // Update state machine
    PIR_UpdateState(motion);
    
    // Return true if state changed
    return (prev_state != g_pir.current_state);
}
```

### Usage Example

```c
#include "pir_sensor.h"
#include "led.h"

void motion_callback(bool detected)
{
    if (detected) {
        LED_On(LED_RED);
        printf("Motion detected!\n");
    } else {
        LED_Off(LED_RED);
        printf("Motion stopped\n");
    }
}

int main(void)
{
    LED_Init();
    PIR_Init();
    PIR_RegisterCallback(motion_callback);
    
    // Setup timer to call PIR_Process periodically
    Timer_Start(TIMER2, 10);  // 10ms interval
    
    while (1) {
        // PIR_Process() called from timer ISR
    }
}

// Timer interrupt (every 10ms)
void TIMER2_IRQHandler(void)
{
    PIR_Process();
}
```

---

## Advanced Features

### Hysteresis Implementation

```c
#define PIR_THRESHOLD_HIGH  600
#define PIR_THRESHOLD_LOW   400

bool PIR_DetectWithHysteresis(void)
{
    static bool state = false;
    
    if (!state && g_pir.filtered_value > PIR_THRESHOLD_HIGH) {
        state = true;  // Rising
        return true;
    }
    else if (state && g_pir.filtered_value < PIR_THRESHOLD_LOW) {
        state = false;  // Falling
        return false;
    }
    
    return state;  // No change
}
```

### Adaptive Threshold

```c
void PIR_AdaptiveThreshold(void)
{
    static uint16_t baseline = 0;
    static uint32_t adaptation_counter = 0;
    
    // Slowly adapt baseline (every second)
    if (++adaptation_counter > 100) {
        adaptation_counter = 0;
        baseline = (baseline + g_pir.filtered_value) / 2;
    }
    
    // Threshold is 30% above baseline
    g_pir.motion_threshold = baseline + (baseline / 3);
}
```

---

## Interview Q&A

### Q1: How does a PIR sensor work and what can affect its performance?

**Answer**:
PIR sensors use two thermopile detectors under a lens with a bandpass filter:

1. **Operation**:
   - Passive infrared radiation from objects is focused onto detectors
   - Temperature changes create voltage changes
   - Differential configuration cancels slow changes (very slow temperature drift)
   - Rapid changes from motion cause output signal

2. **Factors Affecting Performance**:
   - **Temperature**: Ambient temperature drift affects sensitivity
   - **Sunlight**: IR radiation from sun can trigger false positives
   - **Air Currents**: Convection can cause non-motion signals
   - **Humidity**: Very high humidity affects lens visibility
   - **Movement Speed**: Slow movements harder to detect than fast ones
   - **Surface Color**: Dark objects absorb IR, light objects reflect

3. **Mitigation**:
   - Use narrow bandpass filters
   - Warm-up period to stabilize baseline
   - Hysteresis thresholds to reject noise
   - Debouncing to filter spurious detections

### Q2: Explain the debouncing algorithm and why it's necessary.

**Answer**:
Debouncing removes false transitions caused by noise:

```
Noisy signal:
  Motion: ─┐       ┌─────┐       ┌──
           └─┐─┐─┐─┘     └─┐─┐──┘
             └─┘           └─┘

Raw detection (threshold crossing):
  Multiple: ──┐─┐─┐───┐─┐──┐─ (Unreliable)

Debounced (50ms delay):
  Smooth:   ──────┐───────────┐── (Reliable)
```

**Implementation**:
1. Detect initial transition
2. Wait for debounce delay (50-100ms)
3. Verify signal still crossed threshold
4. Then declare state change

**Why Important**:
- Noise causes multiple false edges
- Can trigger multiple alarms
- State machine becomes unstable
- Debouncing costs minimal overhead

### Q3: Compare simple thresholding vs hysteresis-based detection.

**Answer**:

**Simple Threshold**:
```
Detected = (Value > THRESHOLD)
Problem: Oscillation near threshold
│
├─ Value: 510, 520, 510, 520
│  Detected: On, Off, On, Off (Flickers!)
```

**Hysteresis**:
```
Two thresholds: HIGH (600), LOW (400)
│
├─ Rising: Need to exceed HIGH
├─ Falling: Need to go below LOW
├─ Between: Stay in current state
│
Value: 510, 520, 510, 520
Detected: Off, On, On, On (Stable!)
```

**Advantages of Hysteresis**:
- Eliminates flicker near threshold
- Intentional hysteresis margin prevents oscillation
- More realistic physical system behavior
- Reduces noise sensitivity

### Q4: How would you handle multiple PIR sensors in the system?

**Answer**:
Create an array of PIR contexts:

```c
typedef struct {
    uint8_t id;
    uint8_t adc_channel;
    uint8_t gpio_port;
    uint8_t gpio_pin;
    pir_context_t context;
} pir_device_t;

#define MAX_PIR_SENSORS 4
pir_device_t g_pir_devices[MAX_PIR_SENSORS];

void PIR_ProcessAll(void) {
    for (int i = 0; i < MAX_PIR_SENSORS; i++) {
        uint16_t raw = ADC_GetValue(g_pir_devices[i].adc_channel);
        // Process each sensor independently
        pir_filter_and_detect(&g_pir_devices[i], raw);
    }
}
```

**Considerations**:
- Each sensor has own ADC channel
- Each has own state machine
- Each can have different thresholds
- Can combine results (AND, OR logic)

---

**End of PIR Sensor & ADC Guide**

Due to token constraints, I'll create the remaining 7 documents more concisely. Let me continue...
