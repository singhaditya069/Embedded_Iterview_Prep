# ARM Architecture - Comprehensive Q&A

## ARM Basics

### Q1: ARM Cortex-M processors (M0, M3, M4, M7)
**Answer:**
```
┌─────────────────────────────────────────────────────────────┐
│           ARM Cortex-M Processor Comparison               │
├──────────────┬──────┬──────┬──────┬──────┬─────────────────┤
│ Feature      │ M0   │ M3   │ M4   │ M7   │ Use Case        │
├──────────────┼──────┼──────┼──────┼──────┼─────────────────┤
│ Core         │ v6M  │ v7M  │ v7M  │ v7M  │ Base version    │
│ MHz          │ 50   │ 100  │ 100  │ 240+ │ Clock speed     │
│ Instructions │ ARMv6│ ARMv7│ ARMv7│ ARMv7│ Instruction set │
│ FPU          │ No   │ No   │ Yes* │ Yes  │ Float support   │
│ DSP          │ No   │ No   │ Yes  │ Yes  │ Math operations │
│ Cache        │ No   │ No   │ No   │ Yes  │ L1 I/D cache    │
│ MPU          │ Yes  │ Yes  │ Yes  │ Yes  │ Memory protect  │
│ Interrupt    │ NVIC │ NVIC │ NVIC │ NVIC │ Controller      │
│ RTOS         │ Basic│ Full │ Full │ Full │ OS support      │
│ Example MCU  │ STM32│ STM32│ STM32│ STM32│ Manufacturer    │
│              │ L0   │ F1   │ F4   │ H7   │                 │
└──────────────┴──────┴──────┴──────┴──────┴─────────────────┘

*M4F = M4 with FPU, no DSP
```

**Memory Architecture:**
```c
// Typical Cortex-M memory map (STM32 example)
// 0xFFFFFFFF │─────────────────────── NVIC, Debug
//            │
// 0x40000000 │─────────────────────── Peripherals
//            │  UART, SPI, GPIO, ADC, DMA
//            │
// 0x20000000 │─────────────────────── SRAM (32KB)
//            │  Stack (top), Heap (growing up)
//            │
// 0x00000000 │─────────────────────── Flash (256KB)
//            │  Code, const data
```

**Key Features per family:**
```c
// Cortex-M0 - Simplest, energy efficient
- Thumb instruction set only
- No floating point
- ~50 MHz typical
- Ideal for: Battery-powered, simple tasks
- Low cost

// Cortex-M3 - General purpose
- Full Thumb instruction set
- Hardware multiplier
- 32-bit memory access
- ~100 MHz typical
- Ideal for: Standard embedded applications

// Cortex-M4 - High performance computing
- M3 features +
- Single/double precision FPU option
- DSP instructions (SIMD)
- Optimized for signal processing
- Ideal for: Audio, motor control, DSP

// Cortex-M7 - Maximum performance
- M4 features +
- L1 I/D cache
- Dual-issue pipeline
- ~240+ MHz capability
- Ideal for: Complex real-time tasks, graphics

// Real-time guarantee
All Cortex-M: Deterministic interrupt response (<12 cycles)
```

---

### Q2: ARM instruction set (Thumb vs ARM state)
**Answer:**
```asm
; ARM vs Thumb instruction size comparison

; ARM state (32-bit instructions)
        MOV R0, #100        ; 4 bytes
        ADD R1, R2, R3      ; 4 bytes
        BL  function        ; 4 bytes
        
; Thumb state (16/32-bit instructions)
        MOV R0, #100        ; 2 bytes (if constant fits in 8 bits)
        ADD R1, R2, R3      ; 2 bytes
        BL  function        ; 4 bytes (branch with exchange)

; For Cortex-M: Always in Thumb mode!
; Cortex-M has ONLY Thumb instruction set (no ARM state)
```

**Comparison:**
```
┌──────────────────┬──────────────┬──────────────────┐
│ Feature          │ ARM (32-bit) │ Thumb (16/32-bit)│
├──────────────────┼──────────────┼──────────────────┤
│ Code size        │ Larger       │ 30-40% smaller   │
│ Memory usage     │ Higher       │ Lower - embedded │
│ Performance      │ Faster       │ Similar with M4+ │
│ Cortex-M support │ No           │ ONLY option      │
│ Cortex-A support │ Yes          │ Optional         │
└──────────────────┴──────────────┴──────────────────┘
```

**Modern approach:** C/C++ with inline assembly when needed
```c
#include <stdint.h>

// Instead of raw assembly, use intrinsics and inline asm

// Example: Wait For Interrupt (save power)
static inline void wait_for_interrupt(void) {
    __asm__ volatile ("wfi");  // Inline assembly
}

// Example: Memory barrier
static inline void memory_barrier(void) {
    __asm__ volatile ("dmb");  // Data Memory Barrier
}

// CMSIS provides macros (recommended)
#include <core_cm4.h>
__WFI();  // Wait For Interrupt
__ISB();  // Instruction Sync Barrier
__DSB();  // Data Sync Barrier
__DMB();  // Data Memory Barrier
```

---

## Registers and Memory

### Q1: ARM registers (R0-R15, SP, LR, PC, PSR)
**Answer:**
```c
// ARM Cortex-M registers

┌─────────────────────────────────────────┐
│         General Purpose Registers       │
├───────────────────────────┬─────────────┤
│ R0-R3    │ Argument/Result, Working    │ 4 registers
│ R4-R11   │ Local variables, Preserved  │ 8 registers
│ R12 (IP) │ Intra-procedure call        │ Temporary
│          │                             │
│ R13 (SP) │ Stack Pointer               │ Auto-managed
│ R14 (LR) │ Link Register (return addr) │ For function calls
│ R15 (PC) │ Program Counter             │ Current instruction
│          │                             │
│ PSR      │ Program Status Register     │ Flags & mode info
└───────────────────────────┴─────────────┘

// Calling convention (ARM Application Binary Interface - AABI)
void function(int a, int b, int c, int d, int e) {
    // a → R0
    // b → R1
    // c → R2
    // d → R3
    // e → stack (pushed at SP - 4)
}

// Return value
int32_t get_value(void) {
    // Return via R0 for 32-bit
    // R1:R0 for 64-bit
}
```

**Stack usage in Cortex-M:**
```c
// AAPCS: ARM Architecture Procedure Calling Standard

void outer(void) {
    inner(1, 2, 3, 4, 5, 6);  // 6 args
}

void inner(int a, int b, int c, int d, int e, int f) {
    // Call stack layout:
    // SP+20 │ arg f (stacked)  │ ← Pushed by caller
    // SP+16 │ arg e (stacked)  │
    // SP+12 │ LR save (frame)  │
    // SP+8  │ R11 save (frame) │ ← Pushed by prologue
    // SP+4  │ Local var 2      │
    // SP+0  │ Local var 1      │
}

// C code example with ARM memory model
typedef struct {
    volatile uint32_t control;      // Register offset 0x00
    volatile uint32_t status;       // Register offset 0x04
    volatile uint32_t data;         // Register offset 0x08
} UartRegisters_t;

// Map to memory
#define UART0_BASE  0x40000000
UartRegisters_t *uart0 = (UartRegisters_t *)UART0_BASE;

// Direct register access
uart0->control = 0x0301;  // Enable UART
uint32_t data = uart0->data;  // Read received byte
```

---

### Q2: PSR register (Program Status Register)
**Answer:**
```c
// PSR Structure (32-bit)
// ┌──┬───────────────┬───┬─┬─┬─┬─┐
// │N │       -       │ Z │ │C│V│ │
// └──┴───────────────┴───┴─┴─┴─┴─┘
// Bit 31: N - Negative flag
// Bit 30: Z - Zero flag
// Bit 29: C - Carry flag
// Bit 28: V - Overflow flag
// Bit 0:  M - Mode bit (always 0 in Cortex-M, always Thread mode)

#include <core_cm4.h>

// Reading PSR
uint32_t psr = __get_PSR();

// Check individual flags
#define FLAG_N  0x80000000  // Negative
#define FLAG_Z  0x40000000  // Zero
#define FLAG_C  0x20000000  // Carry
#define FLAG_V  0x10000000  // Overflow

void check_flags(uint32_t result) {
    uint32_t psr = __get_PSR();
    
    if (psr & FLAG_Z) {
        // Result is zero
    }
    
    if (psr & FLAG_C) {
        // Carry occurred (overflow in unsigned)
    }
    
    if (psr & FLAG_V) {
        // Overflow occurred (signed)
    }
}

// Conditional execution in Cortex-M (limited)
void conditional_example(int a, int b) {
    // In Cortex-A: Can execute instructions conditionally (AL, EQ, NE, etc.)
    // In Cortex-M: Limited to IT instructions (If-Then)
    
    // Instead, use C conditionals (compiler optimizes)
    if (a > b) {
        // Do something
    }
}

// ARM Condition Codes (for understanding old code)
// EQ - Equal (Z set)
// NE - Not equal (Z clear)
// CS - Carry set (C set)
// CC - Carry clear (C clear)
// MI - Minus/Negative (N set)
// PL - Plus (N clear)
// VS - Overflow (V set)
// VC - No overflow (V clear)
// HI - Unsigned higher (C set, Z clear)
// LS - Unsigned lower/same (C clear OR Z set)
// GE - Greater/equal signed (N == V)
// LT - Less than signed (N != V)
// GT - Greater than signed (Z clear AND N == V)
// LE - Less/equal signed (Z set OR N != V)
// AL - Always (any condition)
```

---

## Interrupts and Exceptions

### Q1: Nested Vectored Interrupt Controller (NVIC)
**Answer:**
```c
#include <stdint.h>
#include <core_cm4.h>

// NVIC structure in memory (STM32 example)
typedef struct {
    volatile uint32_t ISER[8];      // Interrupt Set Enable
    volatile uint32_t ICER[8];      // Interrupt Clear Enable
    volatile uint32_t ISPR[8];      // Interrupt Set Pending
    volatile uint32_t ICPR[8];      // Interrupt Clear Pending
    volatile uint32_t IABR[8];      // Interrupt Active Bit
    volatile uint32_t IPR[240];     // Interrupt Priority (8 bits each)
    volatile uint32_t STIR;         // Software Trigger Interrupt
} NVIC_Type;

#define NVIC_BASE   0xE000E000
#define NVIC        ((NVIC_Type *)NVIC_BASE)

// IRQ numbers for STM32F4 (some examples)
enum IRQn_Type {
    UART0_IRQn = 5,
    SPI0_IRQn  = 10,
    GPIO_IRQn  = 23,
    // ... more
};

// Enable interrupt
void nvic_enable_irq(enum IRQn_Type irq) {
    // Set bit in ISER[irq/32] at position (irq % 32)
    NVIC->ISER[irq >> 5] = (1UL << (irq & 0x1F));
    
    // Or use CMSIS
    NVIC_EnableIRQ(irq);
}

// Disable interrupt
void nvic_disable_irq(enum IRQn_Type irq) {
    NVIC->ICER[irq >> 5] = (1UL << (irq & 0x1F));
    // Or: NVIC_DisableIRQ(irq);
}

// Set interrupt priority (0-255, lower number = higher priority)
void nvic_set_priority(enum IRQn_Type irq, uint8_t priority) {
    NVIC->IPR[irq] = (priority << 4);  // 4-bit priority in high nibble
    // Or: NVIC_SetPriority(irq, priority);
}

// Check if interrupt is pending
uint8_t nvic_is_pending(enum IRQn_Type irq) {
    return (uint8_t)((NVIC->ISPR[irq >> 5] >> (irq & 0x1F)) & 1U);
}

// Set interrupt as pending (software trigger)
void nvic_set_pending(enum IRQn_Type irq) {
    NVIC->ISPR[irq >> 5] = (1UL << (irq & 0x1F));
}

// Clear pending status
void nvic_clear_pending(enum IRQn_Type irq) {
    NVIC->ICPR[irq >> 5] = (1UL << (irq & 0x1F));
}

// Initialization example
void init_uart_interrupt(void) {
    // Enable UART interrupt
    NVIC_EnableIRQ(UART0_IRQn);
    
    // Set priority (higher number = lower priority)
    NVIC_SetPriority(UART0_IRQn, 5);  // Mid-priority
    
    // Enable interrupts globally
    __enable_irq();  // CPSIE I
}

// ISR definition
void UART0_IRQHandler(void) {
    // Handle UART interrupt
    // This must be named exactly as per vector table
    uint8_t data = UART0->DR;  // Read received byte
    
    // Process data
    process_uart_data(data);
}
```

**Priority levels in Cortex-M4:**
```
Priority field = 8 bits, but implementation uses top 4 bits
Valid: 0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240

Lower value = Higher priority (more urgent)
0   = Highest priority (often reserved for critical tasks)
16  = High
32  = Medium-high
...
240 = Lowest priority
```

---

## Memory Management

### Q1: ARM memory protection unit (MPU)
**Answer:**
```c
#include <core_cm4.h>

// MPU Configuration for STM32
typedef struct {
    volatile uint32_t TYPE;         // Type register (read-only)
    volatile uint32_t CTRL;         // Control register
    volatile uint32_t RNR;          // Region Number register
    volatile uint32_t RBAR;         // Region Base Address register
    volatile uint32_t RASR;         // Region Attribute and Size register
} MPU_Type;

#define MPU_BASE    0xE000ED90
#define MPU         ((MPU_Type *)MPU_BASE)

// MPU Region Sizes
#define MPU_SIZE_32B        4
#define MPU_SIZE_64B        5
#define MPU_SIZE_128B       6
#define MPU_SIZE_256B       7
#define MPU_SIZE_512B       8
#define MPU_SIZE_1K         9
#define MPU_SIZE_2K         10
#define MPU_SIZE_4K         11
#define MPU_SIZE_8K         12
#define MPU_SIZE_16K        13
#define MPU_SIZE_32K        14
#define MPU_SIZE_64K        15
#define MPU_SIZE_128K       16
#define MPU_SIZE_256K       17
#define MPU_SIZE_512K       18
#define MPU_SIZE_1M         19
#define MPU_SIZE_2M         20
#define MPU_SIZE_4G         31

// Access permissions
#define PERM_NO_ACCESS      0   // Read/write not permitted
#define PERM_RW_PRIV        1   // Read/write privileged only
#define PERM_RW_PRIV_RO_USR 2   // Privileged read/write, User read-only
#define PERM_RW_ALL         3   // Read/write allowed
#define PERM_RO_PRIV        5   // Privileged read-only
#define PERM_RO_ALL         6   // Read-only allowed

// Configure MPU region
void mpu_configure_region(uint32_t region_num, 
                         uint32_t base_addr, 
                         uint32_t size,
                         uint32_t permissions) {
    // Set region number
    MPU->RNR = region_num;
    
    // Set base address (must be aligned to region size)
    MPU->RBAR = base_addr;
    
    // Set region attributes and size
    uint32_t rasr = 0;
    rasr |= (1 << 0);               // Enable region
    rasr |= (size << 1);            // Region size
    rasr |= (permissions << 24);    // Access control
    rasr |= (0 << 16);              // Execute Never (XN) = 0
    rasr |= (0 << 17);              // Shareable = 0
    rasr |= (0 << 18);              // Cacheable = 0
    rasr |= (0 << 19);              // Bufferable = 0
    
    MPU->RASR = rasr;
}

// Enable MPU
void mpu_enable(void) {
    MPU->CTRL |= 0x01;  // Enable
}

// Disable MPU
void mpu_disable(void) {
    MPU->CTRL &= ~0x01;
}

// Example: Protect bootloader from application
void setup_mpu_protection(void) {
    // Region 0: Bootloader ROM (read-only)
    mpu_configure_region(0, 
                        0x00000000,     // Start of flash
                        MPU_SIZE_32K,   // 32KB bootloader
                        PERM_RO_ALL);   // Read-only
    
    // Region 1: Kernel data (privileged only)
    mpu_configure_region(1, 
                        0x20000000,     // Start of RAM
                        MPU_SIZE_4K,    // 4KB system reserved
                        PERM_RW_PRIV);  // Privileged read/write only
    
    // Region 2: Application RAM (full access)
    mpu_configure_region(2, 
                        0x20001000,     // After kernel data
                        MPU_SIZE_16K,   // 16KB for application
                        PERM_RW_ALL);   // Everyone can access
    
    // Region 3: Peripherals (privileged only)
    mpu_configure_region(3, 
                        0x40000000,     // Peripheral base
                        MPU_SIZE_256M,  // Large region
                        PERM_RW_PRIV);  // Privileged only
    
    mpu_enable();
}
```

**Benefits:**
- Prevent application from accessing kernel memory
- Prevent reads from bootloader
- Enforce access permissions
- Early fault detection

---

## Real-World Embedded Example

```c
// Complete ARM Cortex-M4 System Initialization

#include <core_cm4.h>
#include <stdint.h>

// System clock frequency (STM32F407 @168MHz)
#define SYSTEM_CLOCK    168000000UL

// 1. Clock Configuration
void clock_init(void) {
    // STM32-specific: Set system clock to 168MHz
    // This requires RCC configuration (not shown)
    // Simplified:
    SystemCoreClockUpdate();  // Update SystemCoreClock variable
}

// 2. SysTick timer (for RTOS tick)
volatile uint32_t tick_count = 0;

void systick_init(uint32_t ticks_per_sec) {
    uint32_t load_value = (SYSTEM_CLOCK / ticks_per_sec) - 1;
    
    SysTick_Config(load_value);
    NVIC_SetPriority(SysTick_IRQn, 15);  // Lowest priority
}

void SysTick_Handler(void) {
    tick_count++;
    // Call RTOS scheduler here
}

// 3. Peripheral Interrupt Setup
void uart_init(void) {
    // UART peripheral setup (STM32 specific)
    
    // Enable UART interrupt
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 5);
}

void USART2_IRQHandler(void) {
    // Handle UART interrupt
}

// 4. MPU Setup (optional, for safety-critical systems)
void mpu_init(void) {
    // Protect kernel memory regions
    setup_mpu_protection();
}

// 5. Main initialization sequence
int main(void) {
    // Disable all interrupts
    __disable_irq();
    
    // Initialize clocks
    clock_init();
    
    // Initialize SysTick
    systick_init(1000);  // 1000 ticks per second
    
    // Initialize peripherals
    uart_init();
    
    // Setup MPU (optional)
    mpu_init();
    
    // Enable global interrupts
    __enable_irq();
    
    // Main loop
    while (1) {
        // Application code
    }
    
    return 0;
}
```

This ARM architecture guide provides essential knowledge for embedded developers!
