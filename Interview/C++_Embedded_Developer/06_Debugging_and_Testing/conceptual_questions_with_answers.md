# Debugging Embedded Systems - Comprehensive Q&A

## Debugging Tools and Techniques

### Q1: JTAG and SWD debuggers
**Answer:**
```
Debugging Interfaces Comparison
┌─────────────────┬─────────────────────┬────────────────┐
│ Feature         │ JTAG                │ SWD            │
├─────────────────┼─────────────────────┼────────────────┤
│ Pins needed     │ 4 (TCO, TDI, TCK)   │ 2 (SWCLK, SWDIO)
│ Bandwidth       │ 4 bits/clock        │ 1 bit/clock    │
│ Speed (MHz)     │ 1-50                │ 1-100          │
│ Cost            │ Moderate            │ Low (2-wire)   │
│ Standard        │ IEEE 1149.1         │ ARM proprietary
│ Cortex-M        │ Supported           │ Preferred      │
│ Single wire?    │ No                  │ Yes (SWO trace)
│                 │                     │                │
│ Typical use     │ Older/Complex chips │ Modern ARM MCU │
└─────────────────┴─────────────────────┴────────────────┘

JTAG vs SWD Advantages:
JTAG: Universal across all processors, chain multiple chips
SWD:  Fewer pins, faster, integrated in ARM Cortex-M
```

**Pin connections (SWD - most common for embedded):**
```
STM32F407 ←→ STLINK-V2 (Debugger)
┌────────────┐          ┌────────────┐
│ PA13 SWDIO ├──────────┤ SWDIO      │
│            │          │            │
│ PA14 SWCLK ├──────────┤ SWCLK      │
│            │          │            │
│ GND        ├──────────┤ GND        │
│            │          │            │
│ NRST       ├──────────┤ RST        │
└────────────┘          └────────────┘

Optional trace connection:
│ PB3  SWO   ├──────────┤ SWO (Serial Wire Output - trace)
```

---

### Q2: Setting up GDB and debugging with breakpoints
**Answer:**
```bash
# Start GDB with ARM target
arm-none-eabi-gdb application.elf

# Connect to openocd server (runs separately)
(gdb) target remote :3333

# Reset target
(gdb) monitor reset halt

# Load firmware
(gdb) load

# Set breakpoint at function
(gdb) break main

# Set breakpoint at line
(gdb) break main.c:42

# Set conditional breakpoint
(gdb) break main.c:50 if i > 10

# Watchpoint (break on variable change)
(gdb) watch sensor_value

# Continue execution
(gdb) continue

# Step over function
(gdb) next

# Step into function
(gdb) step

# Step to next instruction (assembly level)
(gdb) nexti

# Print variable value
(gdb) print temperature

# Print memory at address
(gdb) x/16bx 0x20000000    # 16 bytes in hex
(gdb) x/16dw 0x20000000    # 16 words as decimals
(gdb) x/i $pc              # Instruction at program counter

# Print register values
(gdb) info registers
(gdb) print $r0
(gdb) print $sp

# Execute until return from function
(gdb) finish

# View stack trace
(gdb) backtrace

# Run commands on breakpoint
(gdb) commands 1
> silent
> printf "Loop count: %d\\n", loop_count
> continue
> end

# Connect to debugger (OpenOCD example)
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg
```

---

### Q3: Printf debugging and logging
**Answer:**
```c
#include <stdio.h>
#include <stdint.h>

// Redirect printf to UART for embedded systems
#define DEBUG_UART UART0

void debug_init(void) {
    uart_init(&DEBUG_UART, &uart_config);
}

// Intercept printf calls (newlib/GCC)
int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        uart_send_byte(&DEBUG_UART, ptr[i]);
    }
    return len;
}

// Now printf works via UART!
int main(void) {
    debug_init();
    
    printf("System starting...\n");
    
    int sensor_val = read_adc();
    printf("ADC value: %d\n", sensor_val);
    
    return 0;
}

// Log levels for production code
#define LOG_LEVEL_DEBUG   0
#define LOG_LEVEL_INFO    1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR   3

#define CURRENT_LOG_LEVEL LOG_LEVEL_INFO

void log_message(uint8_t level, const char *format, ...) {
    if (level < CURRENT_LOG_LEVEL) return;  // Skip lower priority
    
    const char *level_str[] = {"[DBG]", "[INF]", "[WRN]", "[ERR]"};
    
    printf("%s ", level_str[level]);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\n");
}

// Usage
void process_sensor(uint16_t value) {
    log_message(LOG_LEVEL_DEBUG, "Raw sensor: %d", value);
    
    if (value > THRESHOLD) {
        log_message(LOG_LEVEL_ERROR, "Sensor out of range!");
    }
}

// Runtime log filtering (for production)
void set_log_level(uint8_t level) {
    // Would update CURRENT_LOG_LEVEL dynamically
    // Can read from EEPROM config
}

// Serial logging output example:
// [DBG] Raw sensor: 1024
// [DBG] Raw sensor: 1025
// [ERR] Sensor out of range!

// Benefits of printf debugging:
// ✓ Easy to add/remove print statements
// ✓ No debugger setup needed
// ✓ Works on any UART connection
// ✓ Can be left in production (filtered by log level)
// ✗ Overhead (slows system down slightly)
// ✗ Requires UART/serial connection
```

---

## Unit Testing in Embedded

### Q1: Mocking hardware for unit tests
**Answer:**
```c
#include <stdint.h>
#include <string.h>
#include <assert.h>

// ---- Actual Hardware Abstraction ----

// Abstract GPIO interface
typedef struct {
    void (*write)(uint8_t pin, uint8_t value);
    uint8_t (*read)(uint8_t pin);
} GPIO_Interface_t;

// Application code (uses interface, not direct hardware)
typedef struct {
    GPIO_Interface_t *gpio;
} LED_Driver_t;

LED_Driver_t *led_driver;

void led_init(GPIO_Interface_t *gpio_interface) {
    led_driver->gpio = gpio_interface;
}

void led_on(void) {
    led_driver->gpio->write(5, 1);  // Turn on PA5
}

void led_off(void) {
    led_driver->gpio->write(5, 0);  // Turn off PA5
}

bool led_is_on(void) {
    return led_driver->gpio->read(5) == 1;
}

// ---- Real Hardware Implementation ----

void real_gpio_write(uint8_t pin, uint8_t value) {
    GPIO_Handle_t handle;
    handle.pin_number = pin;
    gpio_write(&handle, value);  // Real hardware call
}

uint8_t real_gpio_read(uint8_t pin) {
    GPIO_Handle_t handle;
    handle.pin_number = pin;
    return gpio_read(&handle);  // Real hardware call
}

GPIO_Interface_t real_gpio = {
    .write = real_gpio_write,
    .read = real_gpio_read,
};

// ---- Mock Hardware for Testing ----

uint8_t mock_gpio_state[16];  // Simulated GPIO register

void mock_gpio_write(uint8_t pin, uint8_t value) {
    mock_gpio_state[pin] = value;
    printf("[MOCK] GPIO%d set to %d\n", pin, value);
}

uint8_t mock_gpio_read(uint8_t pin) {
    return mock_gpio_state[pin];
}

GPIO_Interface_t mock_gpio = {
    .write = mock_gpio_write,
    .read = mock_gpio_read,
};

// ---- Unit Tests ----

void test_led_on(void) {
    printf("Test: LED On\n");
    
    led_driver = malloc(sizeof(LED_Driver_t));
    led_init(&mock_gpio);
    
    // Reset mock state
    memset(mock_gpio_state, 0, sizeof(mock_gpio_state));
    
    // Test
    led_on();
    assert(led_is_on() == true);
    
    printf("✓ PASS: LED turns on\n");
}

void test_led_off(void) {
    printf("Test: LED Off\n");
    
    led_driver = malloc(sizeof(LED_Driver_t));
    led_init(&mock_gpio);
    memset(mock_gpio_state, 0xFF, sizeof(mock_gpio_state));  // Start on
    
    led_off();
    assert(led_is_on() == false);
    
    printf("✓ PASS: LED turns off\n");
}

void test_led_toggle(void) {
    printf("Test: LED Toggle\n");
    
    led_driver = malloc(sizeof(LED_Driver_t));
    led_init(&mock_gpio);
    memset(mock_gpio_state, 0, sizeof(mock_gpio_state));
    
    // Toggle on
    led_on();
    assert(led_is_on() == true);
    
    // Toggle off
    led_off();
    assert(led_is_on() == false);
    
    printf("✓ PASS: LED toggle works\n");
}

// Production code
int main(void) {
    // Use real hardware
    led_driver = malloc(sizeof(LED_Driver_t));
    led_init(&real_gpio);
    
    led_on();   // Control real LED
    
    return 0;
}

// Test runner (runs all tests)
int run_tests(void) {
    printf("Running LED Driver Tests\n");
    printf("=======================\n\n");
    
    test_led_on();
    printf("\n");
    
    test_led_off();
    printf("\n");
    
    test_led_toggle();
    printf("\n");
    
    printf("=======================\n");
    printf("All tests passed!\n");
    
    return 0;
}

// Build test executable:
// gcc -I. led_driver_test.c -o led_driver_test
// ./led_driver_test

// Benefits of mocking:
// ✓ Test without hardware
// ✓ Simulate failures/edge cases
// ✓ Fast test execution
// ✓ CI/CD compatible (runs on build server)
// ✓ Deterministic results
```

---

## Common Debugging Challenges

### Q1: Hard faults and exception handling
**Answer:**
```c
#include <stdint.h>
#include <core_cm4.h>

// Hard Fault information struct (captured on exception)
typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
    uint32_t mfsr;   // Memory Fault Status Register
    uint32_t bfsr;   // Bus Fault Status Register
    uint32_t ufsr;   // Usage Fault Status Register
} HardFault_Info_t;

HardFault_Info_t fault_info;

// Hard fault exception handler
void HardFault_Handler_C(uint32_t *stack_ptr) {
    // Extract fault info from stack
    fault_info.r0 = stack_ptr[0];
    fault_info.r1 = stack_ptr[1];
    fault_info.r2 = stack_ptr[2];
    fault_info.r3 = stack_ptr[3];
    fault_info.r12 = stack_ptr[4];
    fault_info.lr = stack_ptr[5];
    fault_info.pc = stack_ptr[6];      // Program counter of faulting instruction!
    fault_info.psr = stack_ptr[7];
    
    // Read fault registers
    volatile uint32_t *scb = (volatile uint32_t *)0xE000ED28;
    fault_info.mfsr = scb[0];  // MemManage Status
    fault_info.bfsr = scb[1];  // Bus Fault Status
    fault_info.ufsr = scb[2];  // Usage Fault Status
    
    // Log information
    printf("============ HARD FAULT ============\n");
    printf("PC:  0x%08X\n", fault_info.pc);
    printf("LR:  0x%08X\n", fault_info.lr);
    printf("PSR: 0x%08X\n", fault_info.psr);
    printf("R0:  0x%08X\n", fault_info.r0);
    printf("R1:  0x%08X\n", fault_info.r1);
    printf("R2:  0x%08X\n", fault_info.r2);
    printf("R3:  0x%08X\n", fault_info.r3);
    printf("====================================\n");
    
    // Analyze fault type
    uint32_t ufsr = fault_info.ufsr >> 16;
    if (ufsr & 0x01 == 0x01) {
        printf("ERROR: Undefined instruction at 0x%08X\n", fault_info.pc);
    }
    if (ufsr & 0x02) {
        printf("ERROR: ESPR + exception return alignment\n");
    }
    if (ufsr & 0x04) {
        printf("ERROR: Out-of-order exception\n");
    }
    
    // Watchdog reset or hang for debugging
    uint32_t flag = fault_info.pc;
    printf("Entering infinite loop (watch with debugger)\n");
    while (1) {
        // Debugger can inspect fault_info here
        (void)flag;
    }
}

// Assembly wrapper to call C handler with stack pointer
asm void HardFault_Handler(void) {
    IMPORT HardFault_Handler_C
    MOV R0, SP              ; Pass stack pointer as argument
    B HardFault_Handler_C
}

// Common hard fault causes:
// 1. Null pointer dereference
//    ptr = NULL;
//    *ptr = 0x12345678;  // Hard fault!
//
// 2. Stack overflow
//    local_array[100000];  // Huge array on stack
//
// 3. Executing from non-code area
//    code_ptr = 0x20000000;  // RAM address
//    ((void (*)(void))code_ptr)();  // Execute RAM?
//
// 4. Misaligned memory access
//    volatile uint32_t *ptr = (volatile uint32_t *)0x20000001;
//    *ptr = 0x12345678;  // Not aligned to 4 bytes!
//
// 5. Accessing invalid memory region
//    volatile uint32_t *ptr = (volatile uint32_t *)0xDEADBEEF;
//    *ptr = 0;  // Outside valid memory map

// Prevention techniques:
// ✓ Use defensive programming (null checks)
// ✓ Enable Memory Protection Unit (MPU)
// ✓ Stack overflow detection (set min free at end)
// ✓ Use compiler checks (-Wl,--gc-sections -Wl,-Map=output.map)
// ✓ Validate pointers before use
// ✓ Use firmware watchdog with proper resets
```

Debugging embedded systems requires systematic tools and techniques!
