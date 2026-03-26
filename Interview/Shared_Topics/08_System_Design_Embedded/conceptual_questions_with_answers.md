# System Design for Embedded Systems - Comprehensive Q&A

## Real-Time System Design

### Q1: Requirements for real-time embedded systems
**Answer:**
```
Real-Time System Requirements:

TYPE 1: Hard Real-Time
├─ Deadline: MUST be met (consequences if missed: catastrophic)
├─ Examples: Airbag deployment, pacemaker, flight control
├─ Tolerance: 0ms (zero tolerance for miss)
└─ Guarantee: 100% deterministic

TYPE 2: Firm Real-Time  
├─ Deadline: Should meet (occasional miss acceptable)
├─ Examples: Video streaming, audio playback
├─ Tolerance: Occasional miss (skip frame acceptable)
└─ Value: Zero if deadline missed

TYPE 3: Soft Real-Time
├─ Deadline: Prefer to meet (degraded but functional if miss)
├─ Examples: User interface, background tasks
├─ Tolerance: Many misses acceptable
└─ Performance degrades gracefully

Design Criteria:
✓ Predictability - Worst-case execution time known
✓ Determinism - Same inputs → Same outputs + timing
✓ Responsiveness - Fast interrupt response (<100 us typical)
✓ Reliability - No priority inversion, deadlock-free
✓ Resource Management - Bounded memory, CPU usage
```

**WCET Analysis (Worst-Case Execution Time):**
```c
#include <stdint.h>

// Function with predictable timing
void process_sensor_data(uint16_t *buffer, uint16_t count) {
    // Time analysis:
    // - Loop setup: 2-4 cycles
    // - Loop body (per iteration): 8-10 cycles
    //   - Read from buffer: 2 cycles
    //   - Arithmetic: 1 cycle
    //   - Write result: 2 cycles
    //   - Loop increment: 1 cycle
    // - Loop cleanup: 1 cycle
    
    volatile uint16_t result;
    
    for (uint16_t i = 0; i < count; i++) {
        result = buffer[i] >> 2;  // Scale down
        buffer[i] = result;
    }
    
    // For count = 100:
    // WCET = 4 + (100 * 10) + 1 = 1005 cycles
    // At 168 MHz (STM32F4): 1005 / 168M = 5.98 µs
}

// Function with unpredictable timing - BAD for hard real-time!
void bad_process_sensor_data(uint16_t *buffer, uint16_t count) {
    // Variable execution time (UNPREDICTABLE):
    for (uint16_t i = 0; i < count; i++) {
        if (buffer[i] > THRESHOLD) {
            // May call varying-time function
            some_complex_operation(buffer[i]);  // Unknown WCET!
        }
    }
    
    // Problem: WCET cannot be calculated
}

// Predictable alternative
#define THRESHOLD 1024
volatile uint32_t operation_count = 0;

void good_process_sensor_data(uint16_t *buffer, uint16_t count) {
    for (uint16_t i = 0; i < count; i++) {
        if (buffer[i] > THRESHOLD) {
            operation_count++;  // Always O(1)
        }
    }
    // WCET = constant time regardless of data
}
```

---

### Q2: Priority inversion and how to prevent it
**Answer:**
```c
#include <stdint.h>
#include <stdbool.h>

// Priority inversion problem scenario

// Task 1: High priority (safety critical)
void high_priority_task(void) {
    printf("High priority: Starting\n");
    
    // Need resource controlled by mutex
    mutex_lock(&critical_resource);
    
    // Do critical operation
    critical_operation();
    
    mutex_unlock(&critical_resource);
}

// Task 2: Medium priority (normal operation)
void medium_priority_task(void) {
    printf("Medium priority: Running\n");
    // Long operation without mutex
    long_running_operation();
}

// Task 3: Low priority (background)
void low_priority_task(void) {
    printf("Low priority: Starting\n");
    
    mutex_lock(&critical_resource);  // Got it first (ran first)
    printf("Low priority: Got resource\n");
    
    // Simulate long operation while holding mutex
    long_sleep(1000);  // Sleep 1 second
    
    mutex_unlock(&critical_resource);
}

// Execution timeline WITHOUT priority inheritance (PROBLEM):
// T=0ms:    Low task starts, locks mutex
// T=5ms:    High task preempts, tries to lock mutex → BLOCKED!
// T=10ms:   Medium task runs (medium priority > low priority)
//           But low priority has mutex!
// T=20ms:   Medium task yields
// T=100ms:  Low task continues, unlocks mutex
// T=100ms:  High task finally runs!
//
// PROBLEM: High priority task blocked by low priority task!
// This is called PRIORITY INVERSION

// ---- SOLUTION 1: Priority Inheritance ----

typedef struct {
    pthread_mutex_t mutex;
    int original_priority;
} InheritingMutex;

void init_inheriting_mutex(InheritingMutex *m) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&m->mutex, &attr);
}

// FreeRTOS example (automatic with RTOS):
void task_with_inheritance(void) {
    // When high priority task waits on mutex held by low priority,
    // LOW priority task is temporarily elevated to HIGH priority!
    // This prevents medium priority from interrupting
}

// Execution timeline WITH priority inheritance (FIXED):
// T=0ms:    Low task starts, locks mutex
// T=5ms:    High task preempts, tries to lock mutex
//           OS detects: promote low task to high priority!
// T=5ms:    Low task continues (at elevated priority)
//           Medium task CANNOT preempt (low is now high priority)
// T=100ms:  Low task unlocks mutex
// T=100ms:  High task runs (or vice versa)
//
// NO INVERSION! High priority task not blocked by low priority

// ---- SOLUTION 2: Priority Ceiling ----

typedef struct {
    pthread_mutex_t mutex;
    int ceiling_priority;  // Highest priority task that uses it
} CeilingMutex;

// When ANY task locks this mutex, it's elevated to ceiling priority
// Prevents any lower-priority task from interfering
void task_with_ceiling(void) {
    // Ceiling = max(high_task_priority, med_task_priority, low_task_priority)
    // All tasks lock at ceiling priority
    // Simpler than inheritance but may over-elevate some tasks
}

// ---- SOLUTION 3: Avoid Unbounded Priority Inversion ----

// 1. Use spinlocks instead of blocking locks (for short sections)
void spinlock_example(void) {
    // Task only yields to higher priority tasks
    // Medium priority cannot preempt while spinlock held
    while (__sync_lock_test_and_set(&lock, 1));
    
    // Critical section (must be very short!)
    critical_operation();
    
    __sync_lock_release(&lock);
}

// 2. Use semaphores with priority queuing
typedef struct {
    int count;
    Queue *priority_queue;  // Tasks sorted by priority, not FIFO
} PrioritySemaphore;

// 3. Bound critical section duration
// WCET of low priority task in critical section < deadline of high task

// Best practices for priority inversion prevention:
// ✓ Use RTOS with priority inheritance (FreeRTOS)
// ✓ Minimize critical section duration
// ✓ Keep critical sections simple (no function calls)
// ✓ Don't call blocking functions in critical sections
// ✓ Use lock-free data structures when possible
// ✓ Monitor priority inversion with debugger
```

---

## Embedded System Patterns

### Q1: Common embedded system architectures
**Answer:**
```
ARCHITECTURE 1: Bare Metal (No RTOS)
┌──────────────────────────────┐
│      Application Layer       │
│   (Main event loop)          │
└──────────────────────────────┘
        │
┌──────────────────────────────┐
│      Drivers & HAL           │
│ (GPIO, UART, ADC, SPI, ...)  │
└──────────────────────────────┘
        │
┌──────────────────────────────┐
│    Microcontroller (MCU)     │
│   (Registers, Peripherals)   │
└──────────────────────────────┘

Pros: Deterministic, low latency, predictable
Cons: Hard to maintain, scale, handle complex tasks

ARCHITECTURE 2: RTOS-based
┌──────────────────────────────┐
│    Task 1   Task2   Task3    │
│  (Concurrent,                │
│   Scheduled)                 │
└──────────────────────────────┘
        │
┌──────────────────────────────┐
│   RTOS Kernel                │
│  (Scheduler, IPC, Memory)    │
└──────────────────────────────┘
        │
┌──────────────────────────────┐
│      Drivers & HAL           │
└──────────────────────────────┘
        │
┌──────────────────────────────┐
│      Microcontroller         │
└──────────────────────────────┘

Pros: Scalable, modular, easy to test
Cons: Overhead, WCET harder to analyze

ARCHITECTURE 3: Hybrid (Event-driven + RTOS)
┌──────────────────────────────┐
│   High-frequency ISRs        │
│   (High-latency critical)    │
│        │                     │
│        ▼                     │
│   ┌─────────────┐            │
│   │ Event Queue │            │
│   └─────────────┘            │
│        │                     │
│        ▼                     │
│   RTOS Tasks (normal)        │
└──────────────────────────────┘

Best of both: ISRs for critical timing, Tasks for complexity
```

---

### Q2: State machine design for complex systems
**Answer:**
```c
#include <stdint.h>
#include <stdio.h>

// Complex washing machine state machine

typedef enum {
    STATE_IDLE,
    STATE_FILL,
    STATE_WASH,
    STATE_RINSE,
    STATE_SPIN,
    STATE_COMPLETE,
    STATE_ERROR,
} WashState_t;

typedef enum {
    EVENT_START,
    EVENT_FILL_DONE,
    EVENT_WASH_DONE,
    EVENT_RINSE_DONE,
    EVENT_SPIN_DONE,
    EVENT_STOP,
    EVENT_ERROR,
} WashEvent_t;

typedef struct {
    WashState_t current_state;
    uint32_t state_timeout;
    uint32_t phase_count;
    uint8_t drum_speed;
} WasherContext_t;

// State action functions
typedef void (*StateAction)(WasherContext_t *ctx);
typedef void (*EventHandler)(WasherContext_t *ctx);

// State machines actions
void state_idle_entry(WasherContext_t *ctx) {
    printf("STATE: IDLE\n");
    ctx->phase_count = 0;
    ctx->state_timeout = 0;
}

void state_fill_entry(WasherContext_t *ctx) {
    printf("STATE: FILL (timeout 30s)\n");
    open_inlet_valve();
    ctx->state_timeout = 30000;  // 30 seconds
}

void state_fill_exit(WasherContext_t *ctx) {
    printf("EXIT: FILL - closing valve\n");
    close_inlet_valve();
}

void state_wash_entry(WasherContext_t *ctx) {
    printf("STATE: WASH (timeout 60s, rpm=%d)\n", ctx->drum_speed);
    start_motor(ctx->drum_speed);
    ctx->state_timeout = 60000;
}

void state_rinse_entry(WasherContext_t *ctx) {
    ctx->phase_count++;
    printf("STATE: RINSE phase %d (timeout 20s)\n", ctx->phase_count);
    open_inlet_valve();  // Fresh water
    start_motor(500);     // Slow rotation
    ctx->state_timeout = 20000;
}

// Transition table (state machine logic)
typedef struct {
    WashState_t current;
    WashEvent_t event;
    WashState_t next;
    StateAction action;
} Transition_t;

const Transition_t transitions[] = {
    // From IDLE
    {STATE_IDLE, EVENT_START, STATE_FILL, state_fill_entry},
    {STATE_IDLE, EVENT_ERROR, STATE_ERROR, NULL},
    
    // From FILL
    {STATE_FILL, EVENT_FILL_DONE, STATE_WASH, state_wash_entry},
    {STATE_FILL, EVENT_STOP, STATE_IDLE, state_idle_entry},
    {STATE_FILL, EVENT_ERROR, STATE_ERROR, NULL},
    {STATE_FILL, EVENT_FILL_DONE, STATE_ERROR, NULL},  // Timeout
    
    // From WASH
    {STATE_WASH, EVENT_WASH_DONE, STATE_RINSE, state_rinse_entry},
    {STATE_WASH, EVENT_STOP, STATE_IDLE, state_idle_entry},
    {STATE_WASH, EVENT_ERROR, STATE_ERROR, NULL},
    
    // From RINSE
    {STATE_RINSE, EVENT_RINSE_DONE, STATE_RINSE, state_rinse_entry},  // Loop rinse
    {STATE_RINSE, EVENT_SPIN_DONE, STATE_SPIN, NULL},  // After last rinse
    
    // From SPIN
    {STATE_SPIN, EVENT_SPIN_DONE, STATE_COMPLETE, NULL},
    {STATE_SPIN, EVENT_ERROR, STATE_ERROR, NULL},
    
    // From COMPLETE
    {STATE_COMPLETE, EVENT_START, STATE_FILL, state_fill_entry},
    {STATE_COMPLETE, EVENT_ERROR, STATE_ERROR, NULL},
    
    // Error recovery
    {STATE_ERROR, EVENT_START, STATE_IDLE, state_idle_entry},
};

#define TRANSITION_COUNT (sizeof(transitions) / sizeof(transitions[0]))

// Main FSM handler
bool handle_event(WasherContext_t *ctx, WashEvent_t event) {
    // Find transition
    for (uint32_t i = 0; i < TRANSITION_COUNT; i++) {
        if (transitions[i].current == ctx->current_state &&
            transitions[i].event == event) {
            
            // Exit current state
            printf("Exiting state %d\n", ctx->current_state);
            
            // Transition
            ctx->current_state = transitions[i].next;
            
            // Enter new state
            if (transitions[i].action) {
                transitions[i].action(ctx);
            }
            
            return true;
        }
    }
    
    printf("ERROR: Invalid transition from state %d on event %d\n",
           ctx->current_state, event);
    return false;
}

// Example: Main loop with timeout handling
int main(void) {
    WasherContext_t washer = {
        .current_state = STATE_IDLE,
        .drum_speed = 1000,
    };
    
    uint32_t last_time = 0;
    
    while (1) {
        uint32_t current_time = get_system_ticks();
        uint32_t elapsed = current_time - last_time;
        
        // Check for timeout
        if (washer.state_timeout > 0 && elapsed > washer.state_timeout) {
            printf("TIMEOUT in state %d\n", washer.current_state);
            handle_event(&washer, EVENT_ERROR);
            washer.state_timeout = 0;
        }
        
        // Check sensor events
        if (water_level_full()) {
            handle_event(&washer, EVENT_FILL_DONE);
        }
        
        if (wash_cycle_complete()) {
            handle_event(&washer, EVENT_WASH_DONE);
        }
        
        // Check user input
        if (user_pressed_stop()) {
            handle_event(&washer, EVENT_STOP);
        }
        
        last_time = current_time;
    }
    
    return 0;
}

// State machine benefits:
// ✓ Clear, understandable flow
// ✓ Complete coverage of all cases
// ✓ Easy to debug (print state transitions)
// ✓ Testable (each transition independently)
// ✓ Scalable (add states/events easily)
// ✓ No hidden state (explicit state variable)
```

---

## Performance and Optimization

### Q1: Performance measurement and bottleneck identification
**Answer:**
```c
// Performance profiling in embedded systems

#include <stdint.h>
#include <stdio.h>

// Method 1: Cycle counter (ARM Cortex-M)
typedef struct {
    uint32_t start_cycles;
    uint32_t end_cycles;
    uint32_t total_cycles;
} CycleCounter_t;

uint32_t get_cycle_count(void) {
    return DWT->CYCCNT;  // ARM Cortex-M Debug Watchpoint Timer
}

void measure_start(CycleCounter_t *counter) {
    counter->start_cycles = get_cycle_count();
}

void measure_end(CycleCounter_t *counter) {
    counter->end_cycles = get_cycle_count();
    counter->total_cycles = counter->end_cycles - counter->start_cycles;
}

void print_cycles(CycleCounter_t *counter, const char *label) {
    printf("%s: %u cycles\n", label, counter->total_cycles);
}

// Method 2: GPIO toggling (visible on scope)
#define MEASURE_GPIO_TOGGLE() \
    GPIO_TOGGLE(DEBUG_PIN);  // Toggle pin, view on oscilloscope

void measured_operation(void) {
    MEASURE_GPIO_TOGGLE();  // Pin HIGH
    
    // Operation to measure
    expensive_computation();
    
    MEASURE_GPIO_TOGGLE();  // Pin LOW
    
    // View pulse width on scope to measure duration
}

// Method 3: Built-in profiling
typedef struct {
    const char *name;
    uint32_t call_count;
    uint32_t total_cycles;
    uint32_t min_cycles;
    uint32_t max_cycles;
} FunctionProfile_t;

#define MAX_PROFILES 20
FunctionProfile_t profiles[MAX_PROFILES];
uint8_t profile_count = 0;

void profile_function(FunctionProfile_t *prof, const char *name) {
    prof->name = name;
    prof->total_cycles = 0;
    prof->min_cycles = UINT32_MAX;
    prof->max_cycles = 0;
    prof->call_count = 0;
}

void profile_call_start(uint32_t *start) {
    *start = get_cycle_count();
}

void profile_call_end(FunctionProfile_t *prof, uint32_t start) {
    uint32_t end = get_cycle_count();
    uint32_t delta = end - start;
    
    prof->total_cycles += delta;
    prof->call_count++;
    
    if (delta < prof->min_cycles) prof->min_cycles = delta;
    if (delta > prof->max_cycles) prof->max_cycles = delta;
}

void print_profile(FunctionProfile_t *prof) {
    printf("%s: calls=%u, total=%u, avg=%u, min=%u, max=%u\n",
           prof->name, prof->call_count,
           prof->total_cycles,
           prof->total_cycles / prof->call_count,
           prof->min_cycles, prof->max_cycles);
}

// Example: Find bottleneck
int main(void) {
    FunctionProfile_t sensor_prof;
    profile_function(&sensor_prof, "sensor_read");
    
    for (int i = 0; i < 1000; i++) {
        uint32_t start;
        profile_call_start(&start);
        
        read_sensor_value();
        
        profile_call_end(&sensor_prof, start);
    }
    
    print_profile(&sensor_prof);
    // Output: sensor_read: calls=1000, total=50000, avg=50, min=48, max=105
}

// Optimization techniques:
// 1. Move computation to initialization (const calculations)
// 2. Use lookup tables instead of calculations
// 3. Cache frequently accessed memory
// 4. Inline small functions
// 5. Use hardware (DMA, floating point unit)
// 6. Reduce function call overhead
// 7. Optimize memory access patterns
```

This comprehensive guide covers real-time, embedded system design patterns!
