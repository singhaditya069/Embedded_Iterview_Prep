# Complete RTOS (Real-Time Operating System) Study Guide
**Comprehensive Q&A for Embedded Systems & Firmware Development Interviews**

---

## 📋 Table of Contents
1. [RTOS Fundamentals](#rtos-fundamentals)
2. [Task Management](#task-management)
3. [Scheduling & Priority](#scheduling--priority)
4. [Synchronization Mechanisms](#synchronization-mechanisms)
5. [Inter-Task Communication](#inter-task-communication)
6. [FreeRTOS Integration](#freertos-integration)
7. [Advanced Topics](#advanced-topics)
8. [Real-World Scenarios](#real-world-scenarios)
9. [Code Examples](#code-examples)

---

## 🔹 RTOS FUNDAMENTALS

### Q1: What is an RTOS and why use it in embedded systems?

**Answer:** An RTOS (Real-Time Operating System) is a system that manages task execution with deterministic timing guarantees. It enables multitasking while meeting real-time constraints.

**Key Characteristics:**
- **Deterministic**: Task scheduling is predictable
- **Multitasking**: Multiple tasks execute concurrently
- **Preemptive**: Higher priority tasks interrupt lower priority ones
- **Synchronization**: Mechanisms to coordinate task interactions

**Why Use RTOS:**
1. **Complexity Management** - Multiple independent tasks easier than state machines
2. **Timing Guarantees** - Predictable latency for critical operations
3. **Responsive** - Interrupt-driven, responsive to events
4. **Hardware Utilization** - Better use of resources through context switching

**When NOT to Use RTOS:**
- Simple single-task systems (direct firmware simpler)
- Memory-constrained devices (RTOS adds 5-20 KB overhead)
- Extreme latency requirements (bare-metal faster)
- Legacy systems with strict determinism requirements

**Common RTOS Examples:**
```
✓ FreeRTOS (open-source, STM32, ARM, ESP32)
✓ RTOS from STMicroelectronics (proprietary)
✓ ThreadX (Microsoft, real-time, small footprint)
✓ Zephyr (Linux Foundation, modular)
✓ SAFERTOS (safety-critical systems)
```

---

### Q2: General-Purpose OS vs RTOS - Key Differences

| Feature | General-Purpose OS | RTOS |
|---------|-------------------|------|
| **Scheduling** | Fair, time-sliced | Priority-based, preemptive |
| **Determinism** | Non-deterministic latency | Bounded, predictable latency |
| **Resource Usage** | Large (GB of RAM, MB of code) | Small (KB range) |
| **Example** | Linux, Windows | FreeRTOS, SAFERTOS |
| **Task switching** | Milliseconds | Microseconds |
| **Use case** | Desktop, servers | Embedded, real-time control |
| **Overhead** | Significant | Minimal |

---

### Q3: Hard vs Soft Real-Time Systems

**Hard Real-Time Systems:**
```
Definition: Missing deadline = system failure
Examples:
- Airbag deployment (must fire in 30ms)
- Anti-lock braking systems (must respond in 10ms)
- Aircraft flight control (sub-millisecond requirements)

Consequences: Safety-critical, must use deterministic RTOS
Cost: Higher (SAFERTOS, certified systems)
```

**Soft Real-Time Systems:**
```
Definition: Missing deadline = degraded performance, not failure
Examples:
- Video streaming (some frames can be dropped)
- Audio playback (occasional stutter acceptable)
- Industrial sensors (slightly delayed readings)

Consequences: Can use standard RTOS with monitoring
Cost: Lower (FreeRTOS acceptable)
```

**Firm Real-Time Systems:**
```
Definition: Missing deadline = value drops to zero
Examples:
- Online video: late frame = no value
- Real-time stock trading: late data = worthless
```

---

## 🔹 TASK MANAGEMENT

### Q4: Task vs Process vs Thread

```c
// RTOS Terminology:

// TASK: Basic unit of execution in RTOS
typedef struct {
    TaskHandle_t handle;
    int priority;
    uint8_t state;          // READY, RUNNING, BLOCKED, SUSPENDED
    uint32_t stack_size;
} RTOSTask;

// Key characteristics:
// - Has own stack space and registers
// - Managed by RTOS scheduler
// - Share same address space
// - Lightweight (typically 44-64 bytes TCB + stack)

// PROCESS (more in OS/kernel context)
// - Separate memory space (address space isolation)
// - Can have multiple threads
// - Heavier resource usage
// - Protected from other processes

// THREAD (in RTOS context)
// - Usually synonym for task in RTOS
// - Lighter than OS process
// - Shares same address space as parent
// - Faster context switching

// FreeRTOS Example:
void task_function(void *param) {
    while (1) {
        // Task code
        vTaskDelay(100);  // Sleep for 100ms
    }
}

// Create task
TaskHandle_t xTaskHandle;
xTaskCreate(
    task_function,          // Function pointer
    "TaskName",             // Descriptive name
    256,                    // Stack size in words
    NULL,                   // Parameter
    2,                      // Priority (0=idle, higher=more important)
    &xTaskHandle            // Handle
);
```

**Comparison Table:**
```
              Task (RTOS)   Thread         Process
Memory space  Shared        Shared         Separate
Created size  Lightweight   Lightweight    Heavyweight
Scheduled by  RTOS          Kernel         Kernel
Context time  ~50 cycles    ~100 cycles    ~1000 cycles
Stack typical 256-2048B     1-4KB          4-64MB
Data sharing  Direct        Mutex/queue    IPC (pipes, sockets)
Protection    None          None           Full isolation
```

---

### Q5: Task States and State Transitions

```
Complete Task State Diagram:

                    ┌─────────────┐
                    │   CREATED   │
                    └──────┬──────┘
                           │ xTaskCreate()
            ┌──────────────►├──────────────┐
            │               │              │
        SUSPENDED    (not in ready list)  READY
            │               │              │
            │ vTaskResume() └──────────────┤
            │                    xTaskCreate
            │                    or unblock
            │
           ▲
           │ vTaskSuspend()
           │
    ┌────────────────┐      scheduler picks it
    │   RUNNING      │◄──────────────────────┐
    └────────────────┘(highest priority ready)
            │                                 │
        timeout/                          higher priority
        xTaskYield()                       task ready
            │                                 │
            ▼                                 │
        ┌────────────┐                       │
        │   READY    │───────────────────────┘
        └────────────┘
            │
        xSemaphoreTake(timeout=0)
        xQueueReceive(timeout=0)
        vTaskDelay()
        xMutexTake()
            │
            ▼
        ┌────────────┐
        │  BLOCKED   │
        └────────────┘
            │
        unblock condition
        fulfilled / timeout
            │
            ▼
        ┌────────────┐
        │   READY    │
        └────────────┘
```

**Detailed State Descriptions:**

| State | What task is doing | How it exits |
|-------|--------------------|-------------|
| **CREATED** | Just created, waiting to be scheduled | Scheduler runs it → READY |
| **READY** | Waiting for CPU to be allocated | Scheduler picks it → RUNNING |
| **RUNNING** | Actively executing on CPU | Time slice exhausted/yield → READY, or blocks → BLOCKED |
| **BLOCKED** | Waiting for event (semaphore, queue, timeout) | Event occurs or timeout → READY |
| **SUSPENDED** | Explicitly suspended, not in scheduler | vTaskResume() → READY |

**State Transition Code:**
```c
void task_lifecycle_example(void *param) {
    printf("CREATED and now RUNNING\n");
    
    // RUNNING state
    printf("Task started\n");
    
    for (int i = 0; i < 5; i++) {
        vTaskDelay(100);  // RUNNING → BLOCKED → READY → RUNNING cycle
        printf("Iteration %d\n", i);
    }
    
    vTaskSuspend(NULL);  // RUNNING → SUSPENDED (explicit)
    printf("This won't print - suspended\n");
    
    // Must be resumed by another task
    // xTaskResume(task_handle);
}
```

---

## 🔹 SCHEDULING & PRIORITY

### Q6: Priority Levels and Scheduling Algorithm

**FreeRTOS Priority System:**
```c
// Priority levels: 0 (idle) to configMAX_PRIORITIES-1 (highest)
#define PRIORITY_IDLE        0   // Idle task
#define PRIORITY_LOW         1   // Background tasks
#define PRIORITY_MEDIUM      2   // Normal tasks
#define PRIORITY_HIGH        3   // Time-critical tasks
#define PRIORITY_CRITICAL    4   // Emergency/safety

// Task creation with priorities
TaskHandle_t h1, h2, h3;

xTaskCreate(background_task,   "BACKGROUND", 256, NULL, PRIORITY_LOW,      &h1);
xTaskCreate(normal_task,       "NORMAL",     256, NULL, PRIORITY_MEDIUM,   &h2);
xTaskCreate(critical_task,     "CRITICAL",   512, NULL, PRIORITY_CRITICAL, &h3);

// Preemptive scheduling:
// Higher priority task ALWAYS interrupts lower priority task
// Same priority tasks share CPU with round-robin (time-slicing)
```

**Scheduling Algorithm:**

```
FreeRTOS uses: Priority-based Preemptive Scheduling with Time Slicing

Rules:
1. Always run HIGHEST priority ready task
2. Among same priority, use round-robin (time slice = tick period)
3. Lower priority tasks run only when higher are BLOCKED

Example Timeline (assuming 1ms ticks):
Time (ms)  Task      Priority  Reason
0          CRITICAL  4         Ready, highest priority
2          CRITICAL  4         Still running (owns CPU)
5          HIGH      3         CRITICAL blocked on mutex → HIGH runs
10         HIGH      3         Time slice expired (5ms)
10         CRITICAL  4         CRITICAL ready again → preempt HIGH
15         CRITICAL  4         Time slice expired
15         HIGH      3         Continue round-robin
20         HIGH      3         Blocks on queue
20         MEDIUM    2         Highest ready now
30         MEDIUM    2         Time slice expired
30         LOW       1         No higher priority ready
40         LOW       1         Time slice expired
40         IDLE      0         All tasks blocked
50         LOW       1         Event occurs, L ready again
```

**Priority Levels Based on Task Type:**
```
Priority  Use For              Example
4         Safety/Emergency     Watchdog, Emergency stop
3         Hard real-time       Motor control, sensor interrupt handler
2         Soft real-time       Data processing, communication
1         Background           Logging, housekeeping
0         Idle                 (Reserved for idle task)
```

**Worst-case scheduling delay:**
```
Response time = 
    Interrupt latency (few microseconds)
    + Preemption delay (current task finishing)
    + Task execution time

Example:
- Interrupt latency: 2 μs
- Current low-priority task in critical section: 10 ms
- High-priority task execution: 500 μs
- Total worst case: ~10.5 ms
```

---

### Q7: Context Switching - How It Works

**Context Switch Process:**
```
Step 1: Save Current Task Context
  - Save all CPU registers (R0-R12, SP, PC, PSR)
  - Save stack pointer location
  - Store in TCB (Task Control Block)

Step 2: Load Next Task Context
  - Get next task TCB
  - Load registers from TCB
  - Set stack pointer to task's stack
  - Set program counter to task's entry point

Step 3: Return from Interrupt/Function
  - CPU returns to new task
  - New task resumes execution

Hardware support:
  - ARM Cortex-M: Automatic stacking on SysTick interrupt
  - Some registers saved by hardware, others by software
```

**Context Switch Timing:**
```
Context Switch Overhead: 10-100 CPU cycles
  - STM32L0 (80 MHz): ~50 cycles = ~625 nanoseconds
  - STM32H7 (200 MHz): ~100 cycles = ~500 nanoseconds
  
Full task switch including ISR:
  - ARM Cortex-M4: ~50-200 cycles (~1-5 microseconds)
  - Depends on number of registers to save
  
Minimize overhead by:
  1. Reduce task count (combine related tasks)
  2. Use correct priorities (avoid unnecessary switches)
  3. Increase time slices
  4. Minimize critical sections (mutex held time)
  5. Avoid frequent task switching
```

**Measuring Context Switch Overhead:**
```c
static volatile uint32_t context_switch_count = 0;

// Called by idle task hook
void vApplicationIdleHook(void) {
    context_switch_count++;
}

// In main:
printf("Context switches per second: %d\n", context_switch_count);

// Or measure in interrupt handler:
static uint32_t last_time = 0;
uint32_t current_time = DWT_CYCCNT;  // Debug Watch Trace
uint32_t switch_time = current_time - last_time;
printf("Switch time: %d cycles\n", switch_time);
```

---

## 🔹 SYNCHRONIZATION MECHANISMS

### Q8: Semaphores vs Mutexes

**Semaphore:**
```c
// Binary semaphore: 0 or 1 (like a lock)
SemaphoreHandle_t sem = xSemaphoreCreateBinary();

// Counting semaphore: 0 to N (like resource counter)
SemaphoreHandle_t sem = xSemaphoreCreateCounting(
    10,   // Max count
    0     // Initial count
);

// Give (V operation - increment counter)
xSemaphoreGive(sem);

// Take (P operation - decrement counter, block if 0)
if (xSemaphoreTake(sem, pdMS_TO_TICKS(100)) == pdTRUE) {
    // Acquired semaphore
} else {
    // Timeout - couldn't acquire
}

// Use case: Resource signaling, producer-consumer
// Multiple tasks can both give and take
```

**Mutex (Mutual Exclusion):**
```c
// Binary semaphore for mutual exclusion
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

// Differences from semaphore:
// 1. Only the task that takes it can give it back
// 2. Prevents priority inversion (priority inheritance)
// 3. Prevents deadlock from accidental give by wrong task

// Protect critical section
if (xSemaphoreTake(mutex, portMAX_DELAY)) {
    // CRITICAL SECTION - Only one task at a time
    shared_variable = 42;
    some_global_struct.value = 123;
    
    xSemaphoreGive(mutex);  // Must be same task
}

// Use case: Mutual exclusion, protecting shared resources
```

**Comparison:**

| Feature | Semaphore | Mutex |
|---------|-----------|-------|
| **Counter range** | 0 to N | 0 or 1 |
| **Who can give** | Any task (or any task) | Only task that took it |
| **Priority inversion** | Can occur | Prevented (priority inheritance) |
| **Use case** | Signaling, counting resources | Mutual exclusion |
| **Overhead** | Minimal | Slightly higher |
| **Deadlock risk** | Low (if careful) | Higher (wrong order) |

**Code Example - Semaphore vs Mutex:**
```c
// ===== SEMAPHORE EXAMPLE (Resource tracking) =====
SemaphoreHandle_t buffer_slots = xSemaphoreCreateCounting(10, 10);

void producer(void *param) {
    for (int i = 0; i < 20; i++) {
        xSemaphoreTake(buffer_slots, portMAX_DELAY);  // Wait for free slot
        put_data_in_buffer(i);
        xSemaphoreGive(empty_slots);                  // Signal data available
    }
}

void consumer(void *param) {
    for (int i = 0; i < 20; i++) {
        xSemaphoreTake(empty_slots, portMAX_DELAY);   // Wait for data
        int data = get_data_from_buffer();
        xSemaphoreGive(buffer_slots);                 // Signal free slot
    }
}

// ===== MUTEX EXAMPLE (Shared resource protection) =====
SemaphoreHandle_t data_mutex = xSemaphoreCreateMutex();
shared_struct global_data = {0};

void task_a(void *param) {
    if (xSemaphoreTake(data_mutex, portMAX_DELAY)) {
        global_data.counter++;
        global_data.timestamp = get_time();
        xSemaphoreGive(data_mutex);
    }
}

void task_b(void *param) {
    if (xSemaphoreTake(data_mutex, portMAX_DELAY)) {
        int counter = global_data.counter;  // Safe read
        xSemaphoreGive(data_mutex);
    }
}
```

---

### Q9: Priority Inversion Problem and Solutions

**The Problem:**

```
Scenario: 3 tasks with priorities: 
  H (High=3) > M (Medium=2) > L (Low=1)

PROBLEM TIMELINE:
Time  Task   Priority  Action
0ms   L      1         Takes mutex for resource
5ms   H      3         Needs resource, blocks on mutex
       └──→ WAITING for L
10ms  M      2         Becomes ready
       └──→ Since H is blocked, scheduler runs M (2 > 1)
       
RESULT: H (highest priority!) blocked by L (lowest priority) via M
        This violates priority ordering!
        Called: PRIORITY INVERSION
```

**Solution 1: Priority Inheritance**
```
FreeRTOS Mutex (automatic):

When H needs mutex held by L:
  - Temporarily boost L's priority to match H's priority
  - L executes at priority 3
  - L finishes quickly
  - L returns to original priority
  - H can now acquire mutex

Code:
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
// Priority inheritance is AUTOMATIC with mutex!

Time  Task   Priority  Action
0ms   L      1         Takes mutex
5ms   H      3         Needs mutex, blocks
        └──→ L priority boosted to 3!
        └──→ M cannot run (only priority 2)
10ms  L      3         Executes at boosted priority
15ms  L      1         Completes, returns to priority 1
15ms  H      3         Acquires mutex, continues
```

**Solution 2: Priority Ceiling Protocol**
```c
// Manually set all shared resources to highest priority that uses them
#define RESOURCE_PRIORITY 3  // Highest task using this resource

// Before taking resource, raise task priority
uint8_t original_priority;
vTaskPrioritySet(NULL, RESOURCE_PRIORITY);

// Use resource
critical_section();

// Restore original priority
vTaskPrioritySet(NULL, original_priority);
```

**Solution 3: Structured to Avoid Nesting**
```c
// Design: Don't hold multiple mutexes at same time
// Or always acquire in same order

// WRONG (can cause inversion):
void task_a(void) {
    xSemaphoreTake(mutex_1, portMAX_DELAY);
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    // Use resources
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);
}

void task_b(void) {
    xSemaphoreTake(mutex_2, portMAX_DELAY);  // Different order!
    xSemaphoreTake(mutex_1, portMAX_DELAY);  // Can cause deadlock
    // Use resources
    xSemaphoreGive(mutex_1);
    xSemaphoreGive(mutex_2);
}

// RIGHT: Always acquire in same order
void task_a(void) {
    xSemaphoreTake(mutex_1, portMAX_DELAY);
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    // Use resources
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);
}

void task_b(void) {
    xSemaphoreTake(mutex_1, portMAX_DELAY);  // Same order!
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    // Use resources
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);
}
```

---

### Q10: Deadlock - Detection and Prevention

**What is Deadlock?**
```
Circular wait situation where multiple tasks block each other:

Task A holds Resource 1, waits for Resource 2
Task B holds Resource 2, waits for Resource 1

Neither can proceed → DEADLOCK (system hangs)
```

**Deadlock Scenario Code:**
```c
SemaphoreHandle_t R1 = xSemaphoreCreateMutex();
SemaphoreHandle_t R2 = xSemaphoreCreateMutex();

// WRONG - Can cause deadlock
void task_a(void *param) {
    while (1) {
        xSemaphoreTake(R1, portMAX_DELAY);   // Take R1
        vTaskDelay(100);  // Simulate work, task B might run here
        xSemaphoreTake(R2, portMAX_DELAY);   // Wait for R2 (BLOCKED!)
        printf("Task A using both resources\n");
        xSemaphoreGive(R2);
        xSemaphoreGive(R1);
    }
}

void task_b(void *param) {
    while (1) {
        xSemaphoreTake(R2, portMAX_DELAY);   // Take R2 (DEADLOCK!)
        vTaskDelay(100);
        xSemaphoreTake(R1, portMAX_DELAY);   // Wait for R1 (Already held by A!)
        printf("Task B using both resources\n");
        xSemaphoreGive(R1);
        xSemaphoreGive(R2);
    }
}
```

**Deadlock Prevention Rules:**

| Rule | Explanation |
|------|-------------|
| **Resource Ordering** | Always acquire resources in SAME order across all tasks |
| **Timeout** | Never use `portMAX_DELAY`, always use timeout |
| **No Nesting** | Avoid holding multiple locks simultaneously |
| **Minimize Critical Sections** | Hold locks only as long as needed |
| **Detection** | Monitor task states, use watchdog |

**CORRECT - Prevents Deadlock:**
```c
// Solution 1: Always acquire in same order
void task_a(void *param) {
    while (1) {
        xSemaphoreTake(R1, portMAX_DELAY);  // First
        xSemaphoreTake(R2, portMAX_DELAY);  // Then
        // Use resources
        xSemaphoreGive(R2);
        xSemaphoreGive(R1);
    }
}

void task_b(void *param) {
    while (1) {
        xSemaphoreTake(R1, portMAX_DELAY);  // Same order!
        xSemaphoreTake(R2, portMAX_DELAY);
        // Use resources
        xSemaphoreGive(R2);
        xSemaphoreGive(R1);
    }
}

// Solution 2: Use timeouts
void task_with_timeout(void *param) {
    while (1) {
        if (xSemaphoreTake(R1, pdMS_TO_TICKS(100)) == pdTRUE) {
            if (xSemaphoreTake(R2, pdMS_TO_TICKS(100)) == pdTRUE) {
                // Use resources
                xSemaphoreGive(R2);
            } else {
                // Timeout on R2, release R1
                xSemaphoreGive(R1);
                vTaskDelay(10);  // Back off and retry
                continue;
            }
            xSemaphoreGive(R1);
        } else {
            printf("Timeout on R1\n");
            vTaskDelay(10);
        }
    }
}

// Solution 3: Separate into independent tasks
void task_using_r1(void *param) {
    xSemaphoreTake(R1, portMAX_DELAY);
    // Use only R1
    xSemaphoreGive(R1);
}

void task_using_r2(void *param) {
    xSemaphoreTake(R2, portMAX_DELAY);
    // Use only R2
    xSemaphoreGive(R2);
}
```

---

## 🔹 INTER-TASK COMMUNICATION

### Q11: Message Queues

**Queue Concept:**
```
Producer                    Queue                  Consumer
┌─────────┐            ┌──────────────┐          ┌─────────┐
│ Task A  │ xQueueSend │   [1][2][3]  │xQueueRcv │ Task B  │
│ sends   │──────────►│   [4][5]     │◄────────│ receives│
│ data    │            │  [MAX=10]    │          │ data    │
└─────────┘            └──────────────┘          └─────────┘

Features:
- FIFO (First In, First Out)
- Fixed size (items)
- Fixed item size
- Blocking operations
- Thread-safe
```

**Queue Code Example:**
```c
#include "FreeRTOS.h"
#include "queue.h"

// Define message structure
typedef struct {
    uint16_t temperature;
    uint16_t humidity;
    uint32_t timestamp;
} SensorData;

// Create queue
QueueHandle_t sensor_queue;

// In main:
sensor_queue = xQueueCreate(10, sizeof(SensorData));

// Producer: Sensor reading task
void sensor_reader_task(void *param) {
    SensorData data;
    
    while (1) {
        // Read sensors
        data.temperature = read_temperature_sensor();  // Value 0-50°C
        data.humidity = read_humidity_sensor();        // Value 0-100%
        data.timestamp = xTaskGetTickCount();
        
        // Send to queue (blocks if full, or timeout)
        BaseType_t result = xQueueSend(
            sensor_queue,           // Queue handle
            &data,                  // Pointer to data
            pdMS_TO_TICKS(100)      // Timeout: 100ms (can be portMAX_DELAY)
        );
        
        if (result == pdFAIL) {
            printf("Queue full! Data dropped\n");
        }
        
        // Sample every 100ms
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Consumer: Data processing task
void process_data_task(void *param) {
    SensorData data;
    
    while (1) {
        // Wait for queue data (blocks until available)
        if (xQueueReceive(
            sensor_queue,           // Queue handle
            &data,                  // Pointer to receive buffer
            pdMS_TO_TICKS(500)      // Timeout: 500ms (wait max 500ms)
        ) == pdTRUE) {
            
            // Data received
            printf("Temp: %d°C, Humidity: %d%%, Time: %d\n", 
                   data.temperature, data.humidity, data.timestamp);
            
            // Process data
            if (data.temperature > 40) {
                alert_high_temperature();
            }
            
        } else {
            // Timeout - no data received in 500ms
            printf("No sensor data received for 500ms\n");
        }
    }
}

// Queue operations:
// xQueueSend() - Add to back (blocking)
// xQueueSendToFront() - Add to front (for urgent messages)
// xQueueReceive() - Take from front (blocking)
// xQueuePeek() - Look at data without removing
// uxQueueMessagesWaiting() - Check items in queue
```

**Queue Sending Variants:**
```c
// 1. xQueueSend - Blocking send (add to back)
xQueueSend(queue, &data, timeout);

// 2. xQueueSendToFront - Urgent message (add to front)
xQueueSendToFront(queue, &data, timeout);

// 3. xQueueSendFromISR - Send from interrupt (fast)
BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
xQueueSendFromISR(queue, &data, &pxHigherPriorityTaskWoken);
if (pxHigherPriorityTaskWoken == pdTRUE) {
    taskYIELD();  // Switch to higher priority task
}

// Wait modes:
// pdMS_TO_TICKS(100) - Wait max 100ms
// portMAX_DELAY      - Wait indefinitely
// 0                  - Don't wait (return immediately)
```

**Multi-Producer/Consumer Example:**
```c
QueueHandle_t command_queue;

// Setup:
command_queue = xQueueCreate(20, sizeof(uint32_t));

// Producer 1: Button ISR
void button_isr(void) {
    uint32_t command = CMD_BUTTON1_PRESSED;
    xQueueSendFromISR(command_queue, &command, NULL);
}

// Producer 2: Timeout event
void timeout_task(void *param) {
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
        uint32_t command = CMD_PERIODIC_TIMEOUT;
        xQueueSend(command_queue, &command, pdMS_TO_TICKS(100));
    }
}

// Single Consumer (dispatcher)
void command_dispatcher(void *param) {
    uint32_t command;
    
    while (1) {
        if (xQueueReceive(command_queue, &command, portMAX_DELAY)) {
            switch (command) {
                case CMD_BUTTON1_PRESSED:
                    handle_button_press();
                    break;
                case CMD_PERIODIC_TIMEOUT:
                    handle_timeout();
                    break;
            }
        }
    }
}
```

---

### Q12: Task Notifications and Event Flags

**Task Notifications:**
```c
// Lightweight alternative to queues/semaphores
// Each task has a notification value (32-bit)
// Extremely fast - no memory allocation needed

TaskHandle_t receiver_task = NULL;

// Sender sends notification
void sender_task(void *param) {
    while (1) {
        // Method 1: Simple signal (increment)
        xTaskNotifyGive(receiver_task);  // Sets count to 1
        
        // Method 2: Send specific value
        xTaskNotify(receiver_task, 0x01, eSetBits);  // OR with 0x01
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Receiver waits for notification
void receiver_task_func(void *param) {
    uint32_t notification_value;
    
    receiver_task = xTaskGetCurrentTaskHandle();
    
    while (1) {
        // Wait for notification (blocks)
        // Return value: 0 = timeout, >0 = notification received
        uint32_t count = ulTaskNotifyTake(
            pdTRUE,             // Clear count on exit
            pdMS_TO_TICKS(500)  // Timeout
        );
        
        if (count > 0) {
            printf("Notification received, value: %d\n", count);
        } else {
            printf("Timeout - no notification\n");
        }
    }
}

// Use in ISR:
void external_interrupt_handler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    xTaskNotifyGiveFromISR(receiver_task, &xHigherPriorityTaskWoken);
    
    if (xHigherPriorityTaskWoken == pdTRUE) {
        taskYIELD();  // Switch to waiting task
    }
}
```

**Event Flags (EventGroup):**
```c
// Multiple event bits (max 24 usable bits)
EventGroupHandle_t events;

// Create event group
events = xEventGroupCreate();

#define EVENT_SENSOR_READY    (1 << 0)   // Bit 0
#define EVENT_USB_DATA        (1 << 1)   // Bit 1
#define EVENT_TIMER_EXPIRED   (1 << 2)   // Bit 2

// Set events (from anywhere including ISR)
xEventGroupSetBits(events, EVENT_SENSOR_READY);

// Clear events
xEventGroupClearBits(events, EVENT_USB_DATA);

// Wait for events
void application_task(void *param) {
    EventBits_t bits;
    
    while (1) {
        // Wait for multiple events
        bits = xEventGroupWaitBits(
            events,                           // Event group
            EVENT_SENSOR_READY | EVENT_USB_DATA,  // Bits to wait for
            pdTRUE,                          // Clear bits after wait
            pdFALSE,                         // Don't require all bits
            pdMS_TO_TICKS(500)               // Timeout
        );
        
        if (bits & EVENT_SENSOR_READY) {
            printf("Sensor is ready\n");
        }
        if (bits & EVENT_USB_DATA) {
            printf("USB data available\n");
        }
    }
}

// From interrupt:
void sensor_ready_interrupt(void) {
    xEventGroupSetBitsFromISR(events, EVENT_SENSOR_READY, NULL);
}
```

**Comparison: Queue vs Notification vs EventGroup:**

| Feature | Queue | Notification | EventGroup |
|---------|-------|--------------|-----------|
| **Data transfer** | Yes (any size) | 32-bit value | Status bits only |
| **Multiple waiters** | No | No | Yes |
| **Memory overhead** | High (buffer) | Minimal | Minimal |
| **Speed** | Slower | Fast | Fast |
| **Use case** | Messages | Simple signals | Status/flags |
| **ISR support** | Yes | Yes | Yes |

---

## 🔹 FreeRTOS INTEGRATION

### Q13: Porting FreeRTOS to New Platform

**Porting Steps:**

1. **Understand Hardware Architecture**
   ```
   - Processor: ARM Cortex-M, RISC-V, x86?
   - Clock speed: MHz/Speed?
   - Available RAM/Flash?
   - Timer/tick source?
   - Interrupts: Nested vs flat?
   ```

2. **Set Up Core Files**
   ```
   FreeRTOS/Source/
   ├── FreeRTOS.h          (Main include)
   ├── FreeRTOSConfig.h    (Configuration)
   ├── portable/
   │   ├── GCC/
   │   │   └── ARM_CM4/     (Your processor port)
   │   │       ├── port.c
   │   │       └── portmacro.h
   └── kernel/
       ├── tasks.c
       ├── queue.c
       └── timers.c
   ```

3. **Configure FreeRTOSConfig.h**
   ```c
   #define configCPU_CLOCK_HZ              ( 80000000 )
   #define configTICK_RATE_HZ              ( 1000 )  // 1ms ticks
   #define configMAX_PRIORITIES            ( 5 )
   #define configMINIMAL_STACK_SIZE        ( 128 )
   #define configTOTAL_HEAP_SIZE           ( 32000 )  // 32KB
   #define configMAX_TASK_NAME_LEN         ( 16 )
   #define configQUEUE_REGISTRY_SIZE       ( 8 )
   #define configUSE_TICK_HOOK             ( 1 )
   #define configUSE_IDLE_HOOK             ( 1 )
   #define configUSE_MALLOC_FAILED_HOOK    ( 1 )
   ```

4. **Implement Port-Specific Functions**
   ```c
   // In port.c:
   
   // Initialize tick timer
   void vPortSetupTimerInterrupt(void) {
       // Configure SysTick timer for 1kHz (1ms)
       SysTick_Config(80000000 / 1000);  // 80MHz / 1000 = 80k cycles
   }
   
   // Context switch interrupt
   void SysTick_Handler(void) {
       xTaskIncrementTick();
   }
   
   // Task switch (usually called by scheduler)
   void vPortYield(void) {
       // Trigger PendSV interrupt for context switch
       SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
   }
   
   // Stack initialization for new task
   StackType_t *pxPortInitialiseStack(
       StackType_t *pxTopOfStack,
       TaskFunction_t pxCode,
       void *pvParameters) {
       // Initialize stack frame as if task was interrupted
       // Return adjusted stack pointer
   }
   ```

5. **Interrupt Handling**
   ```c
   // ISR that calls FreeRTOS functions must be wrapped
   
   void ISR_Handler(void) {
       // Disable scheduler while in ISR
       taskDISABLE_INTERRUPTS();
       
       // Do ISR work
       handle_interrupt();
       
       // Notify tasks
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
       
       // Enable scheduler
       taskENABLE_INTERRUPTS();
       
       // Context switch if higher priority task woken
       if (xHigherPriorityTaskWoken == pdTRUE) {
           taskYIELD();
       }
   }
   ```

6. **Testing the Port**
   ```c
   void test_rtos(void) {
       // Test 1: Create and run simple task
       xTaskCreate(test_task, "TEST", 256, NULL, 1, NULL);
       
       vTaskStartScheduler();
       
       // If here, scheduler failed
       printf("Scheduler error!\n");
   }
   ```

---

### Q14: Tick Timer Configuration

**Tick Timer Purpose:**
```
- Drives the scheduler (1 tick = context switch opportunity)
- Controls time delays (vTaskDelay)
- Manages timeouts
- Updates system time
```

**Configuration in FreeRTOSConfig.h:**
```c
// Tick rate: Higher = more responsive, more overhead
#define configTICK_RATE_HZ          ( 1000 )   // 1000 Hz = 1ms ticks

// Other timer configurations
#define configTICKLESS_IDLE_DEPTH   ( 2 )      // Low-power mode
```

**Tick Timer Implementation (ARM STM32):**
```c
// Using SysTick timer (available on all Cortex-M devices)

void vPortSetupTimerInterrupt(void) {
    // Set reload register to generate interrupt every millisecond
    // 80 MHz clock / 1000 Hz = 80,000 cycles
    
    uint32_t ulReloadValue = (configCPU_CLOCK_HZ / configTICK_RATE_HZ) - 1;
    
    SysTick->LOAD = ulReloadValue;
    SysTick->VAL = 0;
    
    // Enable SysTick interrupt and timer
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
    
    // Set interrupt priority
    NVIC_SetPriority(SysTick_IRQn, configKERNEL_INTERRUPT_PRIORITY);
}

// SysTick interrupt handler
void SysTick_Handler(void) {
    // Increment tick counter
    if (xTaskIncrementTick() != pdFALSE) {
        // Switch context
        taskYIELD();
    }
}
```

**Tick Rate Trade-offs:**

| Tick Rate | Pros | Cons |
|-----------|------|------|
| **1000 Hz (1ms)** | Responsive, smooth | Higher overhead, more context switches |
| **100 Hz (10ms)** | Good compromise | Less responsive |
| **10 Hz (100ms)** | Low overhead | Poor responsiveness, coarse timing |

**Tickless Idle (Low-Power Mode):**
```c
// CPU can sleep when no tasks are ready
// Reduces power consumption, extends battery life

#define configUSE_TICKLESS_IDLE     ( 1 )

void vPortSuppressTicksAndSleep(uint32_t ulExpectedIdleTime) {
    // Calculate safe sleep time
    // Disable SysTick
    // Enter sleep mode
    // Wake on timer interrupt or external interrupt
    // Resume normal operation
}
```

---

### Q15: ISR (Interrupt Service Routine) Handling in FreeRTOS

**ISR Rules in FreeRTOS:**

```
1. ISRs should be SHORT (just signal a task, defer work)
2. Never call blocking calls in ISR (no xSemaphoreTake)
3. Use _FromISR versions of FreeRTOS functions
4. Check xHigherPriorityTaskWoken to trigger context switch
5. Minimize interrupts disabled time
```

**Interrupt Priorities (ARM Cortex-M):**
```
Interrupt priority 0-31 (lower number = higher priority)

FreeRTOS uses:
- Kernel interrupt priority: Reserved for FreeRTOS timer/scheduler
- Device interrupts: Can be lower or equal to kernel priority
  
configKERNEL_INTERRUPT_PRIORITY = (configMAX_SYSCALL_INTERRUPT_PRIORITY << 5)
// Typically bits [7:5] used for priority

Exceptions < Kernel < Other device interrupts
```

**Proper ISR Pattern:**
```c
// Timer interrupt (hardware timer)
void TIMER_ISR_Handler(void) {
    uint32_t interrupt_id = get_interrupt_id();
    
    // Clear interrupt flag FIRST
    clear_interrupt_flag();
    
    // Do minimal ISR work
    handle_interrupt();
    
    // Signal tasks from ISR
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
    // or
    xQueueSendFromISR(queue, &data, &xHigherPriorityTaskWoken);
    // or
    xTaskNotifyGiveFromISR(task_handle, &xHigherPriorityTaskWoken);
    
    // If a higher priority task was woken, context switch
    if (xHigherPriorityTaskWoken == pdTRUE) {
        // On ARM, trigger PendSV for context switch
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
}

// Long processing deferred to task
void interrupt_handler_task(void *param) {
    while (1) {
        // Wait for interrupt signal
        if (xSemaphoreTake(interrupt_sem, portMAX_DELAY) == pdTRUE) {
            // Do long processing here (not in ISR)
            long_processing_function();
            update_database();
            send_to_uart();
        }
    }
}
```

**Interrupt-Safe vs Non-Safe Functions:**

| Safe in ISR | NOT Safe in ISR |
|------------|---------------|
| `xSemaphoreGiveFromISR()` | `xSemaphoreTake()` |
| `xQueueSendFromISR()` | `xQueueReceive()` |
| `xTaskNotifyGiveFromISR()` | `vTaskDelay()` |
| `xEventGroupSetBitsFromISR()` | `vTaskSuspend()` |

---

## 🔹 ADVANCED TOPICS

### Q16: Memory Management in RTOS

**Memory Allocation Sources:**
```
1. Stack Allocation (automatic)
   - Each task gets fixed stack
   - Grows/shrinks with function calls
   - Fast allocation
   
2. Heap Allocation (dynamic)
   - FreeRTOS kernel heap
   - Task creation, queue creation
   - xTaskCreate uses heap for TCB + stack
```

**Heap Configuration:**
```c
#define configTOTAL_HEAP_SIZE  ( 32000 )  // 32 KB

// Memory layout:
static uint8_t heap[configTOTAL_HEAP_SIZE];

// Task creation cost:
task_size = task_stack_size + TCB_size + queue_overhead
         = 256 + 64 + queue_items * item_size
```

**Stack Size Calculation:**
```c
// Conservative approach: Monitor with tools or estimate

// Factors:
// - Local variables
// - Function call depth (nesting)
// - ISR context save (20-40 bytes)
// - Stack margin (safety)

// Example:
void sensor_task(void *param) {
    // Local variables: ~50 bytes
    uint8_t buffer[128];         // 128 bytes
    struct sensorData data;      // 16 bytes
    
    // Function call stack (kernel, sensors)
    // Estimate: ~100 bytes
    
    // Total estimate: 128 + 16 + 100 = 244 bytes
    // With margin: 256 or 512 bytes
}

xTaskCreate(sensor_task, "SENSOR", 512, NULL, 2, NULL);

// Detect stack overflow (enable in config):
#define configCHECK_FOR_STACK_OVERFLOW  ( 2 )

// Implement hook:
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    printf("Stack overflow in task: %s\n", pcTaskName);
    while(1);  // Halt system
}
```

**Preventing Memory Fragmentation:**
```c
// 1. Pre-allocate all objects at startup
QueueHandle_t queues[10];
TaskHandle_t tasks[5];

void setup_memory(void) {
    for (int i = 0; i < 10; i++) {
        queues[i] = xQueueCreate(20, sizeof(uint16_t));
    }
    
    xTaskCreate(task1, "T1", 256, NULL, 1, &tasks[0]);
    xTaskCreate(task2, "T2", 256, NULL, 1, &tasks[1]);
    // etc.
}

// 2. Never delete/recreate in loops
// Instead: suspend/resume

// WRONG:
while (1) {
    xTaskCreate(temp_task, ...);  // Creates memory
    vTaskDelay(100);
    // Task never deleted - LEAK!
}

// RIGHT: Static allocation
void temp_task(void *param) {
    while (1) {
        // Work
    }
}

xTaskCreate(temp_task, "TEMP", 256, NULL, 1, NULL);

// 3. Use static allocation
static StaticTask_t task_buffer;
static StackType_t task_stack[256];

xTaskCreateStatic(
    temp_task,
    "STATIC",
    256,
    NULL,
    1,
    task_stack,
    &task_buffer
);
```

---

### Q17: Real-Time Analysis and Worst-Case Execution Time (WCET)

**Key Concepts:**

```
Response Time = Interrupt Latency + Preemption Delay + Task Execution Time

Where:
- Interrupt Latency: Time from interrupt to ISR entry
- Preemption Delay: Time before high-priority task runs
- Task Execution Time: How long task takes to complete
```

**WCET of Task:**
```c
// Measurement approach:

#include <time.h>

#define MEASURE_START  { start_time = xTaskGetTickCount();
#define MEASURE_END    end_time = xTaskGetTickCount(); \
                       wcet = (end_time - start_time > wcet) ? \
                              (end_time - start_time) : wcet; }

volatile uint32_t wcet = 0;

void critical_task(void *param) {
    uint32_t start_time, end_time;
    
    while (1) {
        MEASURE_START
        
        // Critical processing
        read_sensor();
        apply_filter();
        update_output();
        
        MEASURE_END
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    printf("Max execution time: %d ms\n", wcet);
}

// Or using DWT (Debug Watch Trace) for microsecond accuracy:
#define DWT_CTRL     (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT   (*(volatile uint32_t *)0xE0001004)
#define DWT_LAR      (*(volatile uint32_t *)0xE0001FB0)

void init_dwt(void) {
    DWT_LAR = 0xC5ACCE55;  // Unlock
    DWT_CTRL |= 1;         // Enable cycle counter
}

uint32_t get_cycle_count(void) {
    return DWT_CYCCNT;
}

void measured_task(void *param) {
    uint32_t start, end, duration_us;
    
    init_dwt();
    
    while (1) {
        start = get_cycle_count();
        
        // Work to measure
        critical_processing();
        
        end = get_cycle_count();
        duration_us = (end - start) / (configCPU_CLOCK_HZ / 1000000);
        
        printf("Duration: %d us\n", duration_us);
    }
}
```

**Deadline Analysis:**
```
Task must complete in X ms:

Required Deadline = X ms
Available Time = Interval - System Overhead - Other Tasks

If Real-Time <= Required Deadline AND
   All higher priority tasks complete in time
   → Task meets deadline (SCHEDULABLE)
```

**Example Schedulability Analysis:**
```
Tasks (on 100 MHz STM32):
- Task H: Period 100ms, WCET 40ms, Priority 3
- Task M: Period 200ms, WCET 60ms, Priority 2
- Task L: Period 500ms, WCET 80ms, Priority 1

Response time for M:
= WCET(M) + (WCET(H) × ceil(Period(M)/Period(H)))
= 60 + (40 × ceil(200/100))
= 60 + (40 × 2)
= 140 ms

Required deadline for M: 200 ms
140 < 200 → M MEETS DEADLINE ✓

Response time for L:
= WCET(L) + WCET(M) × ceil(500/200) + WCET(H) × ceil(500/100)
= 80 + 60 × 3 + 40 × 5
= 80 + 180 + 200
= 460 ms

Required deadline for L: 500 ms
460 < 500 → L MEETS DEADLINE ✓
```

---

## 🔹 REAL-WORLD SCENARIOS

### Scenario 1: Migrating Bare Metal Firmware to FreeRTOS

**Problem Statement:**
```
Current System: State machine-based, bare-metal code
- Read temperature sensor every 100ms
- Read pressure sensor every 200ms
- Read humidity sensor every 150ms
- Send UART data every 500ms
- Monitor button press (interrupt)
- Drive LED output based on sensor values
- No OS overhead
```

**Bare-Metal Architecture:**
```c
// Current approach: Timer interrupt + main loop
volatile uint16_t temp, pressure, humidity;
volatile uint8_t button_pressed = 0;

void SysTick_Handler(void) {
    static int tick_count = 0;
    tick_count++;
    
    if (tick_count % 10 == 0) {  // Every 100ms
        temp = read_temp();
    }
    if (tick_count % 20 == 0) {  // Every 200ms
        pressure = read_pressure();
    }
    if (tick_count % 15 == 0) {  // Every 150ms
        humidity = read_humidity();
    }
}

void BUTTON_ISR(void) {
    button_pressed = 1;
}

int main(void) {
    while (1) {
        // Process data
        if (button_pressed) {
            handle_button();
            button_pressed = 0;
        }
        
        update_led();
        
        // Every 500ms
        static int count = 0;
        count++;
        if (count >= 50) {
            send_uart_data();
            count = 0;
        }
    }
}

// Problems with bare-metal:
// - Hard to maintain (mix of interrupt + main logic)
// - Difficult to add features
// - Tight timing constraints
// - Hard to debug interdependencies
```

**RTOS Architecture Solution:**
```c
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

// Queues and events
QueueHandle_t sensor_data_queue;
EventGroupHandle_t button_events;

#define BUTTON_PRESSED_EVENT (1 << 0)

// Task 1: Sensor sampling (medium priority)
void sensor_sampling_task(void *param) {
    struct {
        uint16_t temp;
        uint16_t pressure;
        uint16_t humidity;
        uint32_t timestamp;
    } data;
    
    while (1) {
        // Sample temperature every 100ms
        data.temp = read_temp();
        
        // Sample pressure every 200ms
        static int count = 0;
        if (count++ % 2 == 0) {
            data.pressure = read_pressure();
        }
        
        // Sample humidity every 150ms
        data.humidity = read_humidity();
        data.timestamp = xTaskGetTickCount();
        
        xQueueSend(sensor_data_queue, &data, pdMS_TO_TICKS(100));
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Task 2: Data processing (high priority)
void data_processing_task(void *param) {
    struct sensor_data data;
    
    while (1) {
        if (xQueueReceive(sensor_data_queue, &data, pdMS_TO_TICKS(500))) {
            // Process sensor data
            update_led_based_on_sensors(data.temp);
            
            // Send to UART (lower priority can do this)
        }
    }
}

// Task 3: UART communication (low priority)
void uart_task(void *param) {
    while (1) {
        // Send status every 500ms
        struct {
            uint16_t temp;
            uint16_t pressure;
        } report;
        
        // (In real system, would get data from queue)
        send_uart_report(&report);
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Task 4: Button handler (high priority)
void button_handler_task(void *param) {
    while (1) {
        EventBits_t bits = xEventGroupWaitBits(
            button_events,
            BUTTON_PRESSED_EVENT,
            pdTRUE,
            pdFALSE,
            portMAX_DELAY
        );
        
        if (bits & BUTTON_PRESSED_EVENT) {
            handle_button_press();
        }
    }
}

// Button ISR
void BUTTON_ISR_Handler(void) {
    xEventGroupSetBitsFromISR(button_events, BUTTON_PRESSED_EVENT, NULL);
}

// Main setup
int main(void) {
    sensor_data_queue = xQueueCreate(10, sizeof(struct sensor_data));
    button_events = xEventGroupCreate();
    
    xTaskCreate(sensor_sampling_task,   "SENSOR",  256, NULL, 1, NULL);
    xTaskCreate(data_processing_task,   "PROCESS", 256, NULL, 2, NULL);
    xTaskCreate(button_handler_task,    "BUTTON",  256, NULL, 3, NULL);
    xTaskCreate(uart_task,              "UART",    256, NULL, 0, NULL);
    
    vTaskStartScheduler();
    
    return 0;
}

// Benefits of RTOS approach:
// - Each task handles one responsibility (cleaner logic)
// - Easy to add new features (new tasks)
// - Better timing guarantees
// - Easier to debug (isolated tasks)
// - Scalable (can add more sensors/tasks)
// - Built-in synchronization (queues, events)
```

**Migration Challenges:**
```
1. Stack Overflow Detection
   #define configCHECK_FOR_STACK_OVERFLOW (2)

2. Interrupt Safe Code
   - Convert ISRs to use _FromISR functions
   - Minimize ISR time

3. Deadlock Prevention
   - Use timeouts
   - Consistent resource ordering

4. Testing
   - Test each task independently
   - Test task interactions
   - Stress test system
```

---

### Scenario 2: Multi-Sensor IoT Application

**Requirements:**
```
- 5 sensors at different sample rates
- Real-time data processing
- Cloud transmission (WiFi, intermittent)
- Low power mode capability
- Fault tolerance (sensor failures)
```

**Design:**
```c
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

// Data structures
typedef struct {
    uint8_t sensor_id;
    uint16_t value;
    uint8_t status;
    uint32_t timestamp;
} SensorReading;

// Sensor sampling task
void sensor_task(void *param) {
    uint8_t sensor_id = (uint8_t)(uintptr_t)param;
    SensorReading reading;
    
    // Different periods for different sensors
    uint32_t period = (sensor_id == 0) ? 100 :  // Temp: 100ms
                      (sensor_id == 1) ? 200 :  // Pressure: 200ms
                      (sensor_id == 2) ? 150 :  // Humidity: 150ms
                      (sensor_id == 3) ? 300 :  // Air quality: 300ms
                                        500;    // UV: 500ms
    
    while (1) {
        // Read sensor with error checking
        int raw_value = read_sensor_adc(sensor_id);
        
        if (raw_value < 0) {
            reading.status = SENSOR_ERROR;
        } else {
            reading.value = convert_to_units(raw_value);
            reading.status = SENSOR_OK;
        }
        
        reading.sensor_id = sensor_id;
        reading.timestamp = xTaskGetTickCount();
        
        // Queue for processing
        xQueueSend(sensor_queue, &reading, pdMS_TO_TICKS(100));
        
        vTaskDelay(pdMS_TO_TICKS(period));
    }
}

// Data aggregation and processing
void data_processor_task(void *param) {
    SensorReading reading;
    struct {
        uint16_t temp, pressure, humidity, air_quality, uv;
        uint32_t timestamp;
        uint8_t status;
    } processed_data = {0};
    
    while (1) {
        // Wait for sensor data
        if (xQueueReceive(sensor_queue, &reading, pdMS_TO_TICKS(500)) == pdTRUE) {
            
            // Validate data
            if (reading.status == SENSOR_OK) {
                
                // Update processed data
                switch (reading.sensor_id) {
                    case 0: processed_data.temp = reading.value; break;
                    case 1: processed_data.pressure = reading.value; break;
                    case 2: processed_data.humidity = reading.value; break;
                    case 3: processed_data.air_quality = reading.value; break;
                    case 4: processed_data.uv = reading.value; break;
                }
                
                processed_data.timestamp = reading.timestamp;
                
                // Send to cloud if WiFi connected
                if (is_wifi_connected()) {
                    xQueueSend(cloud_queue, &processed_data, 0);
                }
                
            } else {
                printf("Sensor %d error\n", reading.sensor_id);
            }
        }
    }
}

// Cloud communication task
void cloud_task(void *param) {
    struct processed_data_t data;
    
    while (1) {
        if (xQueueReceive(cloud_queue, &data, pdMS_TO_TICKS(5000)) == pdTRUE) {
            
            // Try to send with retries
            for (int retry = 0; retry < 3; retry++) {
                if (wifi_send_data(&data) == SUCCESS) {
                    printf("Data sent to cloud\n");
                    break;
                }
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
    }
}

// Watchdog task
void watchdog_task(void *param) {
    TickType_t last_activity[5] = {0};
    
    while (1) {
        for (int i = 0; i < 5; i++) {
            TickType_t now = xTaskGetTickCount();
            
            // Check if sensor task is running
            if ((now - last_activity[i]) > pdMS_TO_TICKS(2000)) {
                printf("Sensor %d timeout - possible failure\n", i);
                // Take action: alert user, switch to safe mode
            }
        }
        
        // Feed hardware watchdog
        kick_watchdog();
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Setup
int main(void) {
    sensor_queue = xQueueCreate(20, sizeof(SensorReading));
    cloud_queue = xQueueCreate(10, sizeof(struct processed_data_t));
    
    // Create sensor sampling tasks
    for (int i = 0; i < 5; i++) {
        char task_name[16];
        sprintf(task_name, "SENSOR_%d", i);
        xTaskCreate(sensor_task, task_name, 256, (void *)(uintptr_t)i, 2, NULL);
    }
    
    // Processing and communication tasks
    xTaskCreate(data_processor_task, "PROCESS", 512, NULL, 2, NULL);
    xTaskCreate(cloud_task,          "CLOUD",   512, NULL, 1, NULL);
    xTaskCreate(watchdog_task,       "WATCH",   256, NULL, 3, NULL);
    
    vTaskStartScheduler();
    
    return 0;
}
```

---

## 🔹 CODE EXAMPLES

### Complete FreeRTOS Template

```c
#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

// ===== CONFIGURATION =====
#define STACK_SIZE_STANDARD  256
#define PRIORITY_HIGH        3
#define PRIORITY_MEDIUM      2
#define PRIORITY_LOW         1

// ===== GLOBAL OBJECTS =====
QueueHandle_t data_queue;
SemaphoreHandle_t resource_mutex;

// ===== TASK 1: Producer =====
void producer_task(void *param) {
    int data = 0;
    
    while (1) {
        data++;
        
        // Send data to queue
        if (xQueueSend(data_queue, &data, pdMS_TO_TICKS(100)) == pdTRUE) {
            printf("[PRODUCER] Sent: %d\n", data);
        } else {
            printf("[PRODUCER] Queue full!\n");
        }
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// ===== TASK 2: Consumer =====
void consumer_task(void *param) {
    int data;
    
    while (1) {
        // Receive data from queue
        if (xQueueReceive(data_queue, &data, pdMS_TO_TICKS(1000)) == pdTRUE) {
            printf("[CONSUMER] Received: %d\n", data);
        } else {
            printf("[CONSUMER] Queue timeout\n");
        }
    }
}

// ===== TASK 3: Protective Resource Access =====
int shared_resource = 0;

void resource_user_task(void *param) {
    while (1) {
        // Protect access to shared resource
        if (xSemaphoreTake(resource_mutex, portMAX_DELAY)) {
            shared_resource++;
            printf("[RESOURCE] Value: %d\n", shared_resource);
            
            vTaskDelay(pdMS_TO_TICKS(100));  // Use resource
            
            xSemaphoreGive(resource_mutex);
        }
        
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// ===== MAIN SETUP =====
int main(void) {
    printf("Starting FreeRTOS Application...\n");
    
    // Create queue
    data_queue = xQueueCreate(10, sizeof(int));
    
    // Create mutex
    resource_mutex = xSemaphoreCreateMutex();
    
    // Check creation success
    if (data_queue == NULL || resource_mutex == NULL) {
        printf("Failed to create queue/mutex!\n");
        return 1;
    }
    
    // Create tasks
    xTaskCreate(producer_task,        "PRODUCER", STACK_SIZE_STANDARD, NULL, PRIORITY_HIGH,   NULL);
    xTaskCreate(consumer_task,        "CONSUMER", STACK_SIZE_STANDARD, NULL, PRIORITY_HIGH,   NULL);
    xTaskCreate(resource_user_task,   "RESOURCE", STACK_SIZE_STANDARD, NULL, PRIORITY_MEDIUM, NULL);
    
    // Start scheduler
    printf("Starting scheduler...\n");
    vTaskStartScheduler();
    
    // Should never reach here
    printf("ERROR: Scheduler failed!\n");
    return 1;
}

// ===== OPTIONAL HOOKS =====

// Called when task runs out of stack
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    printf("ERROR: Stack overflow in task: %s\n", pcTaskName);
    while(1) {
        // Halt
    }
}

// Called when malloc fails
void vApplicationMallocFailedHook(void) {
    printf("ERROR: Memory allocation failed!\n");
    while(1) {
        // Halt
    }
}

// Called every tick
void vApplicationTickHook(void) {
    //printf(".");
}
```

---

## 📚 Quick Reference Summary

| Topic | Key Points |
|-------|-----------|
| **RTOS** | Deterministic, multitasking, priority-based scheduler |
| **Tasks** | Basic execution unit, own stack, managed by scheduler |
| **Priority** | 0 = idle, higher = more important, preemptive |
| **Semaphore** | Counter (0-N), for signaling, any task can give |
| **Mutex** | Binary (0-1), for mutual exclusion, same task releases |
| **Queue** | FIFO buffer for inter-task messages, thread-safe |
| **Notifications** | Lightweight signals (32-bit), no memory overhead |
| **EventGroup** | Multiple event flags, wait for combinations |
| **Deadlock** | Avoid: resource ordering, timeouts, minimal nesting |
| **Priority Inversion** | Prevent: priority inheritance (mutex), structure |
| **Context Switch** | 50-200 cycles, saves/loads registers |
| **ISR** | Use _FromISR versions, minimize time, check xHigherPriorityTaskWoken |
| **Memory** | Pre allocate, static allocation, stack overflow detection |
| **Tick Timer** | Drives scheduler, typically 1-1000 Hz |

---

**This comprehensive guide covers all RTOS topics needed for embedded systems and firmware development interviews!** 🚀

