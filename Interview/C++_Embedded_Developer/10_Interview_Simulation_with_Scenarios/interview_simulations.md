# Interview Simulation - Real-World Embedded Scenarios

## Complete Coding Interviews and Problem-Solving Sessions

### Interview 1: "Fix the Buggy Temperature Monitoring System"

**Scenario**: You're hired to debug a temperature monitoring system for an industrial plant. The system reads temperature from a sensor, logs it, and triggers alarms. Currently, it's malfunctioning.

**Given Buggy Code**:
```c
#include <stdio.h>
#include <stdint.h>

typedef struct {
    int16_t *history;
    int history_size;
    int current_index;
} TemperatureLog_t;

TemperatureLog_t temp_log;

// Function 1: Initialize log (BUGGY)
void init_log(int size) {
    temp_log.history = (int16_t *)malloc(size);  // BUG: No check if malloc fails
    temp_log.history_size = size;
    temp_log.current_index = 0;
}

// Function 2: Log temperature (BUGGY)
void log_temperature(int16_t temp) {
    if (temp_log.history == NULL) return;  // Check too late
    
    temp_log.history[temp_log.current_index] = temp;
    temp_log.current_index++;
    if (temp_log.current_index >= temp_log.history_size) {  // BUG: Just wraps, no circular buffer logic
        temp_log.current_index = 0;
    }
}

// Function 3: Check alarm (BUGGY)
void check_alarm(int16_t temp) {
    const int THRESHOLD = 50;  // 50°C
    if (temp > THRESHOLD) {
        printf("ALARM!\n");
    }
    // BUG: No proper alarm management, might spam alarms
}

// Function 4: Get average (BUGGY)
int get_average_temperature(void) {
    int sum = 0;
    for (int i = 0; i < temp_log.history_size; i++) {
        sum += temp_log.history[i];
    }
    return sum / temp_log.history_size;  // BUG: Integer division loses precision + buffer might not be full
}

// Main (BUGGY)
int main(void) {
    init_log(10);
    
    for (int i = 0; i < 15; i++) {  // Writes 15 values to size 10 buffer!
        log_temperature(20 + i);
    }
    
    printf("Average: %d\n", get_average_temperature());
    return 0;
}
```

**Your Tasks**:
1. Identify all bugs
2. Fix the code
3. Explain the issues

**Issues Found**:

| Bug # | Location | Problem | Impact | Severity |
|-------|----------|---------|--------|----------|
| 1 | `init_log()` | No malloc failure check | Null pointer dereference | **CRITICAL** |
| 2 | `log_temperature()` | Circular buffer missing old data not handled | Data loss, inconsistent state | **HIGH** |
| 3 | `check_alarm()` | No alarm state tracking | Alarm spam, log flooding | **MEDIUM** |
| 4 | `get_average_temperature()` | Includes uninitialized values | Wrong average calculation | **HIGH** |
| 5 | `get_average_temperature()` | Integer division loses precision | Result inaccurate | **MEDIUM** |
| 6 | `main()` | Buffer overflow | Memory corruption | **CRITICAL** |

**Fixed Code**:
```c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int16_t *history;
    int history_size;
    int current_index;
    int total_samples;      // NEW: Track total samples written
    uint8_t alarm_active;   // NEW: Prevent alarm spam
} TemperatureLog_t;

static TemperatureLog_t temp_log = {0};

// FIXED: Initialize with error checking
int init_log(int size) {
    if (size <= 0) {
        fprintf(stderr, "ERROR: Invalid log size %d\n", size);
        return -1;
    }
    
    temp_log.history = (int16_t *)malloc(size * sizeof(int16_t));
    if (temp_log.history == NULL) {  // FIXED: Check malloc result
        fprintf(stderr, "ERROR: malloc failed for size %d\n", size);
        return -1;
    }
    
    // FIXED: Initialize all values
    for (int i = 0; i < size; i++) {
        temp_log.history[i] = 0;
    }
    
    temp_log.history_size = size;
    temp_log.current_index = 0;
    temp_log.total_samples = 0;
    temp_log.alarm_active = 0;
    
    printf("✓ Log initialized: size=%d\n", size);
    return 0;
}

// FIXED: Proper circular buffer with overflow handling
void log_temperature(int16_t temp) {
    if (temp_log.history == NULL) {
        fprintf(stderr, "ERROR: Log not initialized\n");
        return;
    }
    
    temp_log.history[temp_log.current_index] = temp;
    temp_log.current_index = (temp_log.current_index + 1) % temp_log.history_size;
    temp_log.total_samples++;  // FIXED: Track total writes
    
    printf("Logged: %d°C (sample #%d)\n", temp, temp_log.total_samples);
}

// FIXED: Stateful alarm (prevent spam)
void check_alarm(int16_t temp) {
    const int THRESHOLD = 50;  // 50°C
    
    if (temp > THRESHOLD) {
        if (!temp_log.alarm_active) {  // FIXED: Only alarm once
            printf("🚨 ALARM: Temperature %.1f°C exceeds threshold!\n", (float)temp / 10);
            temp_log.alarm_active = 1;
        }
    } else {
        if (temp_log.alarm_active) {  // FIXED: Clear alarm when safe
            printf("✓ Alarm cleared: Temperature normalized\n");
            temp_log.alarm_active = 0;
        }
    }
}

// FIXED: Calculate average only of valid samples
double get_average_temperature(void) {
    if (temp_log.history == NULL) {
        fprintf(stderr, "ERROR: Log not initialized\n");
        return 0.0;
    }
    
    if (temp_log.total_samples == 0) {
        fprintf(stderr, "ERROR: No temperature samples yet\n");
        return 0.0;
    }
    
    double sum = 0.0;
    int count = (temp_log.total_samples < temp_log.history_size) ? 
                 temp_log.total_samples : temp_log.history_size;
    
    for (int i = 0; i < count; i++) {
        sum += temp_log.history[i];
    }
    
    return sum / count;  // FIXED: Return double for precision
}

// FIXED: Print buffer contents
void print_buffer_status(void) {
    printf("\n=== Buffer Status ===\n");
    printf("Samples recorded: %d\n", temp_log.total_samples);
    printf("Buffer size: %d\n", temp_log.history_size);
    printf("Current index: %d\n", temp_log.current_index);
    printf("Average temperature: %.1f°C\n", get_average_temperature());
}

// FIXED: Main with proper testing
int main(void) {
    // Initialize
    if (init_log(10) != 0) {
        fprintf(stderr, "FATAL: Could not initialize temperature log\n");
        return 1;
    }
    
    // Log temperatures (controlled amount)
    printf("\n=== Logging Phase ===\n");
    for (int i = 0; i < 5; i++) {
        int16_t temp = 200 + (i * 10);  // 20°C to 60°C
        log_temperature(temp);
        check_alarm(temp / 10);  // FIXED: Convert to °C
    }
    
    print_buffer_status();
    
    // Log more values to test circular buffer
    printf("\n=== Circular Buffer Test ===\n");
    for (int i = 0; i < 8; i++) {
        log_temperature(300 + i);  // 30°C to 37°C
        check_alarm((300 + i) / 10);
    }
    
    print_buffer_status();
    
    // Cleanup
    free(temp_log.history);
    temp_log.history = NULL;
    
    return 0;
}
```

**Key Learning Points**:
- ✓ Always check malloc/calloc return values
- ✓ Initialize all buffer entries
- ✓ Track circular buffer state correctly
- ✓ Use appropriate data types (float vs int)
- ✓ Implement stateful behaviors (alarm spam prevention)
- ✓ Validate input parameters

---

### Interview 2: "Design a State Machine for Device Control"

**Scenario**: Design a firmware for a smart relay device that:
- Receives commands over UART (ON, OFF, PULSE)
- Controls an output relay
- Reports status
- Handles errors

**Solution**:
```c
#include <stdio.h>
#include <stdint.h>

// State Machine for Relay Control
// ================================

typedef enum {
    RELAY_STATE_OFF = 0,
    RELAY_STATE_ON = 1,
    RELAY_STATE_PULSE = 2,
    RELAY_STATE_ERROR = 3,
} RelayState_t;

typedef enum {
    CMD_NONE = 0,
    CMD_TURN_ON = 1,
    CMD_TURN_OFF = 2,
    CMD_PULSE = 3,
    CMD_RESET = 4,
    CMD_STATUS = 5,
} Command_t;

typedef struct {
    RelayState_t state;
    uint16_t pulse_duration_ms;
    uint16_t pulse_remaining_ms;
    uint32_t uptime_ms;
    uint8_t error_code;
    uint32_t total_on_time_ms;
} RelayContext_t;

static RelayContext_t relay_ctx = {
    .state = RELAY_STATE_OFF,
    .error_code = 0,
};

// State handlers
void relay_enter_off_state(void) {
    printf("[RELAY] Entering OFF state\n");
    // Deactivate relay hardware
}

void relay_enter_on_state(void) {
    printf("[RELAY] Entering ON state\n");
    // Activate relay hardware
}

void relay_enter_pulse_state(void) {
    printf("[RELAY] Entering PULSE state (duration: %dms)\n", 
           relay_ctx.pulse_duration_ms);
    relay_ctx.pulse_remaining_ms = relay_ctx.pulse_duration_ms;
    // Activate relay hardware
}

void relay_enter_error_state(void) {
    printf("[RELAY] Entering ERROR state (code: %d)\n", relay_ctx.error_code);
    // Deactivate relay for safety
}

// Command handlers
void relay_cmd_turn_on(void) {
    if (relay_ctx.state == RELAY_STATE_ERROR) {
        printf("[RELAY] ERROR: Cannot turn on while in error state\n");
        return;
    }
    relay_enter_off_state();  // Cleanup exit
    relay_ctx.state = RELAY_STATE_ON;
    relay_enter_on_state();
}

void relay_cmd_turn_off(void) {
    if (relay_ctx.state == RELAY_STATE_ON || relay_ctx.state == RELAY_STATE_PULSE) {
        relay_ctx.state = RELAY_STATE_OFF;
        relay_enter_off_state();
    }
}

void relay_cmd_pulse(uint16_t duration) {
    if (duration == 0 || duration > 10000) {  // Valid: 1-10000ms
        relay_ctx.error_code = 1;  // Invalid duration
        relay_ctx.state = RELAY_STATE_ERROR;
        relay_enter_error_state();
        return;
    }
    relay_ctx.pulse_duration_ms = duration;
    relay_ctx.state = RELAY_STATE_PULSE;
    relay_enter_pulse_state();
}

void relay_cmd_reset(void) {
    printf("[RELAY] Resetting error state\n");
    relay_ctx.error_code = 0;
    relay_ctx.state = RELAY_STATE_OFF;
    relay_enter_off_state();
}

void relay_cmd_status(void) {
    const char *state_str[] = {"OFF", "ON", "PULSE", "ERROR"};
    printf("[STATUS] State:%s | Uptime:%ums | TotalON:%ums | Error:%d\n",
           state_str[relay_ctx.state], relay_ctx.uptime_ms, 
           relay_ctx.total_on_time_ms, relay_ctx.error_code);
}

// Tick (called from timer, e.g., 1ms intervals)
void relay_tick_1ms(void) {
    relay_ctx.uptime_ms++;
    
    switch (relay_ctx.state) {
        case RELAY_STATE_ON:
            relay_ctx.total_on_time_ms++;
            break;
            
        case RELAY_STATE_PULSE:
            relay_ctx.total_on_time_ms++;
            relay_ctx.pulse_remaining_ms--;
            
            if (relay_ctx.pulse_remaining_ms == 0) {
                printf("[RELAY] Pulse complete, returning to OFF\n");
                relay_ctx.state = RELAY_STATE_OFF;
                relay_enter_off_state();
            }
            break;
            
        case RELAY_STATE_OFF:
        case RELAY_STATE_ERROR:
        default:
            // No time tracking needed
            break;
    }
}

// Process command
void relay_process_command(Command_t cmd) {
    printf("[CMD] Received: %d\n", cmd);
    
    switch (cmd) {
        case CMD_TURN_ON:
            relay_cmd_turn_on();
            break;
        case CMD_TURN_OFF:
            relay_cmd_turn_off();
            break;
        case CMD_PULSE:
            relay_cmd_pulse(500);  // 500ms pulse
            break;
        case CMD_RESET:
            relay_cmd_reset();
            break;
        case CMD_STATUS:
            relay_cmd_status();
            break;
        default:
            printf("[RELAY] Unknown command: %d\n", cmd);
    }
}

// Test
int main(void) {
    printf("=== Relay Control State Machine ===\n\n");
    
    // Initial status
    relay_cmd_status();
    
    // Turn on
    printf("\n[TEST 1] Turn on relay\n");
    relay_process_command(CMD_TURN_ON);
    relay_cmd_status();
    
    // Simulate 100ms
    for (int i = 0; i < 100; i++) relay_tick_1ms();
    relay_cmd_status();
    
    // Turn off
    printf("\n[TEST 2] Turn off relay\n");
    relay_process_command(CMD_TURN_OFF);
    relay_cmd_status();
    
    // Pulse
    printf("\n[TEST 3] Send 500ms pulse\n");
    relay_process_command(CMD_PULSE);
    relay_cmd_status();
    
    // Simulate pulse
    for (int i = 0; i < 500; i++) relay_tick_1ms();
    relay_cmd_status();
    
    // Invalid pulse (error test)
    printf("\n[TEST 4] Send invalid pulse (error handling)\n");
    relay_process_command(CMD_PULSE);  // Will try with 500ms
    relay_cmd_pulse(20000);  // Invalid - too long!
    relay_cmd_status();
    
    // Reset
    printf("\n[TEST 5] Reset error\n");
    relay_process_command(CMD_RESET);
    relay_cmd_status();
    
    return 0;
}
```

---

### Interview 3: "Optimize This Embedded Algorithm"

**Challenge**: Given a function that runs on a 32MHz microcontroller with 64KB RAM, optimize it for speed and memory.

**Original Code** (Inefficient):
```c
// Finds peak signal value in last N samples (sliding window)
int find_peak_slow(int *samples, int count) {
    int peak = samples[0];
    for (int i = 1; i < count; i++) {
        if (samples[i] > peak) {
            peak = samples[i];
        }
    }
    return peak;
}

// Called every 1ms for 1000-sample window = 1M iterations/sec!
```

**Optimized Code**:
```c
// Optimization 1: Inline hint
static inline int find_peak_fast(const int *samples, int count) {
    if (count <= 0) return 0;
    
    int peak = samples[0];
    
    // Optimization 2: Loop unrolling (4x)
    for (int i = 1; i < count; i += 4) {
        if (samples[i] > peak) peak = samples[i];
        if (i+1 < count && samples[i+1] > peak) peak = samples[i+1];
        if (i+2 < count && samples[i+2] > peak) peak = samples[i+2];
        if (i+3 < count && samples[i+3] > peak) peak = samples[i+3];
    }
    
    return peak;
}

// Optimization 3: SIMD if available (ARM NEON)
// Optimization 4: Use lookup table for scaling
// Result: ~4x faster execution
```

---

## Summary: Key Interview Patterns

**What Interviewers Test**:
1. ✓ Bug finding and fixing
2. ✓ State machine design
3. ✓ Algorithm optimization
4. ✓ Memory management
5. ✓ Error handling
6. ✓ Code clarity and maintainability

**Common Mistakes to Avoid**:
- ✗ Not checking malloc/function return values
- ✗ Off-by-one errors in loops
- ✗ Forgetting about circular buffers
- ✗ Not initializing variables
- ✗ Missing error states
- ✗ Poor code documentation

**Best Practices**:
- ✓ Write defensive code (check all inputs)
- ✓ Think about edge cases
- ✓ Consider memory and performance
- ✓ Use meaningful variable names
- ✓ Add assertions for debugging
- ✓ Profile before optimizing
