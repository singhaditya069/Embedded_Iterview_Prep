# Performance Optimization - Comprehensive Q&A

## Code and Memory Optimization

### Q1: Optimization techniques for embedded systems
**Answer:**
```c
#include <stdint.h>
#include <string.h>

// Optimization Level 1: Algorithmic Optimization
// ============================================

// BEFORE: O(n²) bubble sort (BAD for large arrays)
void sort_slow(uint16_t *arr, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        for (uint16_t j = 0; j < len - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                uint16_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    // 100 elements = 10,000 comparisons
    // 1000 elements = 1,000,000 comparisons!
}

// AFTER: O(n log n) quicksort
// Already shown in DSA section - much faster!

// Optimization Level 2: Loop Unrolling
// =====================================

// BEFORE: Simple loop
void copy_data_slow(uint8_t *src, uint8_t *dst, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        dst[i] = src[i];
    }
    // Loop overhead: check, increment, branch (per iteration)
}

// AFTER: Loop unrolled 4x (fewer iterations, less overhead)
void copy_data_fast(uint8_t *src, uint8_t *dst, uint16_t len) {
    uint16_t i = 0;
    
    // Process 4 elements at a time
    while (i < (len & ~3)) {  // len rounded down to multiple of 4
        dst[i] = src[i];
        dst[i+1] = src[i+1];
        dst[i+2] = src[i+2];
        dst[i+3] = src[i+3];
        i += 4;
    }
    
    // Handle remaining 0-3 elements
    while (i < len) {
        dst[i] = src[i];
        i++;
    }
    // 4x fewer loop overhead!
}

// Optimization Level 3: Compiler Intrinsics/Inline Assembly
// ==========================================================

// BEFORE: Generic function
uint32_t count_ones_slow(uint32_t value) {
    uint32_t count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
    // 32 iterations in worst case!
}

// AFTER: Use CPU built-in instruction
#include <core_cm4.h>

uint32_t count_ones_fast(uint32_t value) {
    return __builtin_popcount(value);  // Hardware instruction (Cortex-M4)
    // 1 instruction!
}

// Optimization Level 4: Lookup Tables vs Calculation
// ==================================================

#define TABLE_SIZE 256

uint8_t sine_table[TABLE_SIZE];  // Pre-calculated sine values

void init_sine_table(void) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        // Convert sine (-1 to 1) to 0-255 range
        sine_table[i] = (sin(i * M_PI / TABLE_SIZE) + 1) * 127.5;
    }
}

// Fast sine lookup
uint8_t sine_lookup(uint8_t angle_index) {
    return sine_table[angle_index];
    // One array access vs sine calculation!
}

// Verification: Call this once at startup
uint8_t verify_sine(uint8_t angle_index) {
    uint8_t table_val = sine_table[angle_index];
    uint8_t calc_val = (sin(angle_index * M_PI / TABLE_SIZE) + 1) * 127.5;
    
    if (table_val != calc_val) {
        printf("ERROR: Table mismatch!\n");
    }
    return table_val;
}

// Optimization Level 5: Function Inlining
// ========================================

// BEFORE: Function call overhead
static uint16_t scale_value(uint16_t val) {
    return (val * 3) >> 2;  // val * 0.75
}

void process_data_slow(uint16_t *data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        data[i] = scale_value(data[i]);  // Function call per iteration
    }
}

// AFTER: Inline to eliminate call overhead
static inline uint16_t scale_value_inline(uint16_t val) {
    return (val * 3) >> 2;
}

void process_data_fast(uint16_t *data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        data[i] = (data[i] * 3) >> 2;  // Compiler inlines or optimizes away
    }
}

// Optimization Level 6: Memory Access Patterns
// =============================================

// BEFORE: Cache-unfriendly (2D array row-major)
#define ROWS 100
#define COLS 100

uint16_t matrix[ROWS][COLS];

uint32_t sum_matrix_slow(void) {
    uint32_t sum = 0;
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS; row++) {
            sum += matrix[row][col];  // Column-major access (cache misses!)
        }
    }
    return sum;
}

// AFTER: Cache-friendly (row-major access)
uint32_t sum_matrix_fast(void) {
    uint32_t sum = 0;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            sum += matrix[row][col];  // Row-major access (cache hits!)
        }
    }
    return sum;
    // 5-10x faster on real hardware!
}

// Optimization Level 7: Compiler Flags
// ====================================
/*
GCC Compiler flags for optimization:
-O0  = No optimization (debugging friendly, slow)
-O1  = Basic optimization (faster, still debuggable)
-O2  = Moderate optimization (good balance)
-O3  = Aggressive optimization (fastest, hard to debug)
-Osize = Size optimization (smallest binary)

Memory-specific flags:
-ffunction-sections  = Put each function in separate section
-fdata-sections      = Put each data in separate section
-Wl,--gc-sections    = Garbage collect unused sections
-Wl,-Map=output.map  = Generate memory map

Example for embedded:
arm-none-eabi-gcc -O2 -ffunction-sections -fdata-sections \
  -Wl,--gc-sections -Wl,-Map=firmware.map -o firmware.elf main.c
*/
```

---

### Q2: Memory footprint reduction
**Answer:**
```c
#include <stdint.h>

// Strategy 1: Use smaller data types
// ==================================

// BEFORE: Using int (32 bits) for values 0-100
typedef struct {
    int temperature;    // 32 bits (0-100 range)
    int humidity;       // 32 bits (0-100 range)
    int pressure;       // 32 bits (985-1050 range)
} SensorReading_Old_t;

// Size = 12 bytes

// AFTER: Use appropriate types
typedef struct {
    uint8_t temperature;    // 8 bits (0-100 range)
    uint8_t humidity;       // 8 bits (0-100 range)
    uint16_t pressure;      // 16 bits (985-1050 range)
} SensorReading_New_t;

// Size = 4 bytes (3x smaller!)

// Strategy 2: Bit fields for flags
// ================================

// BEFORE: Using bytes for individual flags
typedef struct {
    uint8_t initialized;
    uint8_t calibrated;
    uint8_t error;
    uint8_t warning;
    uint8_t enabled;
} Flags_Old_t;

// Size = 5 bytes (even with padding)

// AFTER: Use bit fields
typedef struct {
    uint8_t initialized : 1;  // 1 bit
    uint8_t calibrated  : 1;
    uint8_t error       : 1;
    uint8_t warning     : 1;
    uint8_t enabled     : 1;
    uint8_t reserved    : 3;  // Unused bits
} Flags_New_t;

// Size = 1 byte (5x smaller!)
void test_flags(void) {
    Flags_New_t flags = {0};
    flags.initialized = 1;  // Access like normal
    flags.calibrated = 1;
    
    if (flags.error) {
        // Handle error
    }
}

// Strategy 3: Move to flash (read-only data)
// ==========================================

// BEFORE: String in RAM (wastes precious RAM)
char error_message[] = "ERROR: Sensor failed!";  // ~20 bytes in RAM

// AFTER: String in flash (code memory, usually larger)
const char error_message_flash[] = "ERROR: Sensor failed!";  // In flash
// Access: printf("%s", error_message_flash);

// After linker optimization with --gc-sections:
// Unused strings are removed from binary

// Strategy 4: Circular buffers (fixed size)
// ==========================================

#define RX_BUFFER_SIZE 256

typedef struct {
    uint8_t buffer[RX_BUFFER_SIZE];  // Fixed size (on stack or static)
    uint16_t head;
    uint16_t tail;
    uint16_t count;
} CircularBuffer_t;

CircularBuffer_t rx_buffer = {0};

// vs. malloc() approach:
// uint8_t *rx_buffer = malloc(256);  // Dynamic = more complex + overhead

// Fixed buffers:
// ✓ Predictable memory usage
// ✓ No fragmentation
// ✓ No malloc overhead
// ✓ Smaller code size
// ✗ Fixed maximum

// Strategy 5: Function pointers instead of if-else chains
// =======================================================

// BEFORE: Multiple if-else branches
void handle_command_slow(uint8_t cmd, uint8_t *data) {
    if (cmd == 0x01) motor_control(data);
    else if (cmd == 0x02) led_control(data);
    else if (cmd == 0x03) sensor_read(data);
    else if (cmd == 0x04) config_system(data);
    else if (cmd == 0x05) calibrate(data);
    // More branches = larger code
}

// AFTER: Lookup table
typedef void (*CommandHandler)(uint8_t *data);

const CommandHandler cmd_handlers[] = {
    NULL,              // 0x00 (unused)
    motor_control,     // 0x01
    led_control,       // 0x02
    sensor_read,       // 0x03
    config_system,     // 0x04
    calibrate,         // 0x05
};

void handle_command_fast(uint8_t cmd, uint8_t *data) {
    if (cmd < sizeof(cmd_handlers)/sizeof(cmd_handlers[0])) {
        CommandHandler handler = cmd_handlers[cmd];
        if (handler) {
            handler(data);
        }
    }
}

// Smaller and faster!

// Strategy 6: Profiling memory usage
// ===================================

// Use linker map file to analyze:
// $ grep -E '\.text|\.data|\.bss' firmware.map

// Typical embedded system memory distribution:
// Flash (Code + Constants): 50-80% of available flash
// RAM (Variables + Stack): 50-90% of available RAM

// Tools to check memory:
void print_memory_info(void) {
    printf("=== Memory Usage ===\n");
    printf("Text segment (code): ~32KB\n");      // From map file
    printf("Data segment (initialized): ~2KB\n");
    printf("BSS segment (uninitialized): ~4KB\n");
    printf("Total RAM used: ~6KB\n");
    printf("Total flash used: ~36KB\n");
    
    // If STM32F4 (1MB flash, 192KB RAM):
    printf("Flash usage: 3.5%% (SAFE)\n");
    printf("RAM usage: 3.1%% (SAFE)\n");
}
```

---

### Q3: Power consumption optimization
**Answer:**
```c
#include <core_cm4.h>

// Power consumption is critical for battery-powered devices

// Strategy 1: Dynamic clock scaling
// =================================

void reduce_clock_speed(void) {
    // Lower system clock from 168 MHz to 84 MHz
    // Reduces power by ~50% (linear relationship)
    
    // Tradeoff: Operations take twice as long
    // Use when full speed not needed
    
    // Typical implementation: change PLL multiplier in RCC
}

// Strategy 2: Put unused peripherals to sleep
// ============================================

void disable_unused_peripherals(void) {
    // ADC not used? Disable it
    ADC1->CR2 &= ~(1 << 0);  // ADON = 0
    
    // SPI not in use? Stop clock
    RCC->APB1ENR &= ~(1 << 14);  // SPIxEN = 0
    
    // Timer not needed? Disable it
    RCC->APB1ENR &= ~(1 << 0);   // TIM2EN = 0
    
    // Unused GPIO? Set to analog mode (lowest power)
    GPIOA->MODER |= 0x3;  // Set PA0 to analog
}

// Strategy 3: Sleep modes (most critical)
// ========================================

// Sleep Mode Comparison:
// ┌──────────┬───────┬──────────┬──────────────┐
// │ Mode     │ Power │ Wakeup   │ Use When     │
// ├──────────┼───────┼──────────┼──────────────┤
// │ Run      │ 100mA │ N/A      │ Active work  │
// │ Sleep    │ 50mA  │ <1µs     │ Waiting      │
// │ Stop     │ 0.5mA │ 10µs     │ Deep sleep   │
// │ Standby  │ 10µA  │ 1 sec    │ Ultra-low    │
// └──────────┴───────┴──────────┴──────────────┘

void enter_sleep_for_button(void) {
    // Normal code takes 100mA
    // CPU sleep = 50mA
    // Still responsive to button (GPIO interrupt)
    
    while (no_work_to_do()) {
        __WFI();  // Wait For Interrupt (sleep mode)
    }
    // Wakes immediately on interrupt
}

void enter_deep_sleep(void) {
    // Use stop mode for longer inactivity
    volatile uint32_t *pwr_cr = (volatile uint32_t *)0x40007000;
    
    *pwr_cr &= ~1;     // LPDSR = 0 (speed mode)
    *pwr_cr |= (1 << 2);  // PDDS = 1 (power down)
    
    __WFI();  // Enter stop mode
    
    // Wake from: GPIO, RTC, UART (if configured)
    // Requires PLL restart (~10ms boot time)
}

// Strategy 4: Efficient interrupt handling
// =========================================

// BAD: Complex processing in ISR
void bad_uart_isr(void) {
    // UART interrupt handler
    uint8_t data = UART->DR;
    
    // Don't do this in ISR!
    process_command(data);  // Time-consuming
    update_display();       // More time-consuming
    log_data(data);         // Even more...
    
    // ISR blocks other interrupts (high-priority)
    // Can miss other interrupts!
}

// GOOD: ISR just queues event
volatile char rx_buffer[256];
volatile int rx_head = 0, rx_tail = 0;

void good_uart_isr(void) {
    uint8_t data = UART->DR;
    
    // Minimal ISR work
    rx_buffer[rx_head] = data;
    rx_head = (rx_head + 1) % 256;
    
    // Flag main loop to process
    // ISR returns quickly, allows other interrupts
}

void main_loop(void) {
    while (1) {
        // Check if data received
        if (rx_tail != rx_head) {
            char data = rx_buffer[rx_tail];
            rx_tail = (rx_tail + 1) % 256;
            
            // Now do time-consuming processing
            process_command(data);
            update_display();
            log_data(data);
        }
    }
}

// Strategy 5: Optimize ADC sampling
// =================================

void optimize_adc_power(void) {
    // ADC with slow clock = lower power
    // Sample when needed, not continuously
    
    // Bad: Sample every 100ms
    // Good: Sample every 5 seconds
    
    // Bad: 16-bit precision for temperature
    // Good: 8-bit precision (still accurate enough)
}

// Power consumption tracking:
void estimate_battery_life(void) {
    // Typical Smart temperature sensor:
    // ├─ Sleep: 0.5mA @ 3.3V * 23 hours = ~38mAh
    // ├─ Process: 20mA * 1 hour = 20mAh
    // └─ Total: ~58mAh per day
    //
    // With 1000mAh battery:
    // Battery life ≈ 1000 / 58 ≈ 17 days
}
```

Optimization transforms embedded systems from slow/power-hungry to efficient and responsive!
