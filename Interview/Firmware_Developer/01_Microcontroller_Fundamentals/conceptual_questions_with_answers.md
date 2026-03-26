# Microcontroller Fundamentals - Comprehensive Q&A

## Microcontroller Architecture

### Q1: Microcontroller (MCU) vs Microprocessor (CPU)
**Answer:**
```
┌────────────────────────────────────────────────────────┐
│         MCU vs CPU Comparison                          │
├──────────────────┬──────────────────┬─────────────────┤
│ Feature          │ Microcontroller  │ Microprocessor  │
├──────────────────┼──────────────────┼─────────────────┤
│ Integration      │ Integrated SOC   │ Separate chips  │
│ Memory           │ Internal 4KB-1MB │ External, larger│
│ Power usage      │ mW to µW         │ Watts           │
│ Clock speed      │ 1-240 MHz        │ GHz             │
│ I/O              │ GPIO, SPI, UART  │ Need chipsets   │
│ Cost             │ $0.50-$10        │ $5-$100+        │
│ Application      │ Embedded systems │ Computers       │
│ Real-time        │ Deterministic    │ Best-effort     │
│ Interrupts       │ 10-100+ vectors  │ Few vectors     │
└──────────────────┴──────────────────┴─────────────────┘

MCU Examples:
- STM32F407: 168 MHz, 1MB Flash, 192KB RAM, $5-10
- ATmega328: 16 MHz, 32KB Flash, 2KB RAM, $2-3
- ARM Cortex-M0: 50 MHz, 256KB Flash, 32KB RAM, $1-2

CPU Examples:
- ARM Cortex-A53: 1-2 GHz, Needs Linux/Android
- x86-64: Multi-GHz, Needs full OS
- RISC-V: Variable speed, experimental
```

**Typical MCU block diagram (STM32F4):**
```
┌─────────────────────────────────────────┐
│         Cortex-M4 CPU                   │
│  168 MHz, FPU, DSP instructions         │
└─────────────────────────────────────────┘
                   │
    ┌──────────────┼──────────────┐
    │              │              │
    ▼              ▼              ▼
┌─────────┐  ┌──────────┐  ┌──────────┐
│ Flash   │  │ RAM      │  │ DMA      │
│ 1MB     │  │ 192KB    │  │          │
└─────────┘  └──────────┘  └──────────┘
    │              │              │
    └──────────────┼──────────────┘
                   │
┌──────────────────────────────────────┐
│    Peripheral Interface Bus          │
├──────────────────────────────────────┤
│ UART(4) │ SPI(5) │ I2C(3) │ USB(1)  │
│ ADC(3)  │ DAC(2) │ GPIO   │ Timers  │
└──────────────────────────────────────┘
```

---

### Q2: MCU reset sources and boot process
**Answer:**
```c
#include <stdint.h>

// Reset source enumeration
typedef enum {
    RESET_POWER_ON,      // Power-on reset
    RESET_EXTERNAL,      // External NRST pin
    RESET_SOFTWARE,      // Software reset (watchdog, NVIC)
    RESET_BROWNOUT,      // Brown-out voltage
    RESET_IWDG,          // Independent watchdog
    RESET_WWDG,          // Window watchdog
    RESET_PIN_RESET,     // NRST pin held low
} ResetSource_t;

// STM32 RCC (Reset and Clock Control) register
typedef struct {
    volatile uint32_t CR;      // Clock control
    volatile uint32_t PLLCFGR; // PLL config
    volatile uint32_t CFGR;    // Clock configuration
    volatile uint32_t CIR;     // Clock interrupt
    volatile uint32_t AHB1RSTR;// AHB1 reset
    volatile uint32_t APB1RSTR;// APB1 reset
    volatile uint32_t CSR;     // Clock status (reset source!)
} RCC_Type;

#define RCC_BASE    0x40023800
#define RCC         ((RCC_Type *)RCC_BASE)

// Read reset source from CSR register
ResetSource_t get_reset_source(void) {
    uint32_t csr = RCC->CSR;
    
    // STM32F4 reset status bits
    #define CSR_LWDGRSF     (1 << 30)  // Low power watchdog
    #define CSR_WWDGRSF     (1 << 29)  // Window watchdog
    #define CSR_IWDGRSF     (1 << 27)  // Independent watchdog
    #define CSR_SFTRSF      (1 << 24)  // Software reset
    #define CSR_PORRST      (1 << 27)  // Power-on reset
    #define CSR_PINRSF      (1 << 26)  // Pin reset
    #define CSR_BORRSF      (1 << 25)  // Brown-out reset
    
    if (csr & CSR_PINRSF) return RESET_EXTERNAL;
    if (csr & CSR_SFTRSF) return RESET_SOFTWARE;
    if (csr & CSR_IWDGRSF) return RESET_IWDG;
    if (csr & CSR_WWDGRSF) return RESET_WWDG;
    if (csr & CSR_PORRST) return RESET_POWER_ON;
    
    return RESET_POWER_ON;
}

// Clear reset flags
void clear_reset_flags(void) {
    RCC->CSR |= (1 << 31);  // Set RMVF (remove reset flags)
}

// Boot sequence
void bootloader_main(void) {
    // 1. Get reset source
    ResetSource_t reset = get_reset_source();
    
    // 2. Initialize clock system
    system_clock_init();
    
    // 3. Initialize critical peripherals
    uart_init();              // For debugging
    
    // 4. Check reset reason
    if (reset == RESET_EXTERNAL || reset == RESET_SOFTWARE) {
        // Unexpected reset - possible error recovery
        log_reset_event(reset);
    }
    
    // 5. Initialize remaining peripherals
    gpio_init();
    adc_init();
    dma_init();
    
    // 6. Initialize RTOS (if used)
    rtos_init();
    
    // 7. Jump to application
    app_main();
}

// Watchdog reset (intentional reset for recovery)
void system_reset_with_fault_check(void) {
    // IWDG (Independent Watchdog) - cannot be disabled!
    // Guaranteed reset within specified time
    
    volatile uint32_t *iwdg_kr = (volatile uint32_t *)0x40003000;
    *iwdg_kr = 0x5555;  // Write key to unlock
    *iwdg_kr = 0xCCCC;  // Start watchdog
    
    // Watchdog will reset MCU if we don't reset it in time
    while (1);  // Wait for reset
}

// Intentional software reset
void software_reset(void) {
    volatile uint32_t *aircr = (volatile uint32_t *)0xE000ED0C;
    *aircr = 0x05FA0004;  // SYSRESETREQ bit
    
    while (1);  // Wait for reset
}
```

**Boot process sequence:**
```
1. Power-on / Reset pin pulled low
   │
2. MCU CPU starts execution at 0x00000000 (Reset_Handler)
   │
3. LDR PC, [PC, #reset_vector_offset]  → Jump to Reset_Handler
   │
4. Reset_Handler (in bootloader/startup code)
   ├─ Set up stack pointer (SP initially at top of RAM)
   ├─ Copy initialized variables from Flash to RAM (\.data section)
   ├─ Initialize uninitialized variables to 0 (\.bss section)
   ├─ Call global constructors (C++ only)
   │
5. Call main() or bootloader_main()
   │
6. Enable interrupts and run application
```

---

## Clock and Power Management

### Q1: Clock sources and system clock configuration
**Answer:**
```c
// Clock sources in STM32F4

typedef enum {
    CLOCK_SOURCE_HSI,    // High Speed Internal (16 MHz)
    CLOCK_SOURCE_HSE,    // High Speed External (8-25 MHz crystal)
    CLOCK_SOURCE_PLL,    // Phase-Locked Loop (up to 240 MHz)
} ClockSource_t;

// PLL configuration for 168 MHz operation
// Input: 8 MHz external crystal
// Output: 168 MHz

#define PLL_M 8      // Divide input by 8 → 1 MHz
#define PLL_N 336    // Multiply by 336 → 336 MHz
#define PLL_P 2      // Divide by 2 → 168 MHz (SYSCLK)
#define PLL_Q 7      // Divide by 7 → 48 MHz (USB/SDIO)

typedef struct {
    volatile uint32_t CR;      // Clock control
    volatile uint32_t PLLCFGR; // PLL configuration
    volatile uint32_t CFGR;    // Clock configuration
    volatile uint32_t CIR;     // Clock interrupt control
} RCC_Type;

void clock_init_168mhz(void) {
    RCC_Type *rcc = (RCC_Type *)0x40023800;
    
    // 1. Enable HSE (external crystal oscillator)
    rcc->CR |= (1 << 16);  // HSEON = 1
    while (!(rcc->CR & (1 << 17)));  // Wait for HSERDY
    
    // 2. Set power regulator output voltage to maximum
    volatile uint32_t *pwr_cr = (volatile uint32_t *)0x40007000;
    *pwr_cr |= (3 << 14);  // VOS = Scale 1 (maximum performance)
    
    // 3. Configure PLL
    rcc->PLLCFGR = 0;
    rcc->PLLCFGR |= PLL_M;                  // M factor
    rcc->PLLCFGR |= (PLL_N << 6);           // N factor
    rcc->PLLCFGR |= ((PLL_P/2 - 1) << 16);  // P factor
    rcc->PLLCFGR |= (PLL_Q << 24);          // Q factor
    rcc->PLLCFGR |= (1 << 22);              // PLL source = HSE
    
    // 4. Enable PLL
    rcc->CR |= (1 << 24);  // PLLON = 1
    while (!(rcc->CR & (1 << 25)));  // Wait for PLLRDY
    
    // 5. Set clock dividers
    rcc->CFGR |= (4 << 13);  // APB1 clock = SYSCLK / 4 (42 MHz)
    rcc->CFGR |= (2 << 10);  // APB2 clock = SYSCLK / 2 (84 MHz)
    rcc->CFGR |= (0 << 4);   // AHB clock = SYSCLK (168 MHz)
    
    // 6. Set PLL as system clock
    rcc->CFGR |= 0x02;  // SW[1:0] = 10 (PLL selected as system clock)
    while ((rcc->CFGR & 0x0C) != 0x08);  // Wait for SWS = 10
    
    // 7. Update SystemCoreClock for HAL/CMSIS
    SystemCoreClock = 168000000;
}

// Clock tree output frequencies
// SYSCLK = 168 MHz
// AHB    = 168 MHz (max 168 MHz)
// APB1   = 42 MHz  (max 42 MHz)
// APB2   = 84 MHz  (max 84 MHz)
// USB    = 48 MHz  (required for USB)
```

---

### Q2: Power management and sleep modes
**Answer:**
```c
#include <stdint.h>
#include <core_cm4.h>

// Power modes in Cortex-M4
typedef enum {
    POWER_MODE_RUN,         // Normal operation
    POWER_MODE_SLEEP,       // Light sleep (can wake on interrupt)
    POWER_MODE_STOP,        // Deep sleep (lower power)
    POWER_MODE_STANDBY,     // Ultra-low power (requires full reset)
} PowerMode_t;

// Enter sleep mode (light sleep)
void enter_sleep_mode(void) {
    // CPU stops executing but peripherals keep running
    // Wakes on any interrupt
    
    __WFI();  // Wait For Interrupt
    
    // After ISR, continues here
}

// Enter stop mode (deeper sleep)
void enter_stop_mode(void) {
    // Oscillators stopped, only low-power peripherals active
    // RAM preserved
    
    volatile uint32_t *pwr_cr = (volatile uint32_t *)0x40007000;
    
    // Clear WUF (wakeup flag)
    *pwr_cr |= (1 << 3);
    
    // Set PDDS bit to enter power-down mode
    *pwr_cr |= (1 << 2);  // PDDS = 1 (STOP mode)
    
    // Set LPDSR for regulator in low power mode
    *pwr_cr &= ~(1 << 0);  // LPDSR = 0
    
    // Wait for interrupt to wake
    __WFI();
    
    // Peripheral re-initialization may be needed
    system_clock_init();
}

// Enter standby mode (deepest sleep)
void enter_standby_mode(void) {
    // All oscillators stopped, only RTC/wakeup pin active
    // RAM NOT preserved (full reset on wakeup)
    
    volatile uint32_t *pwr_cr = (volatile uint32_t *)0x40007000;
    
    // Clear WUF flag
    *pwr_cr |= (1 << 3);
    
    // Enable wakeup pin
    *pwr_cr |= (1 << 8);   // EWUP (Enable Wakeup Pin)
    
    // Set PDDS for standby
    *pwr_cr |= (1 << 2);   // PDDS = 1
    
    // Wait for interrupt (wakeup pin)
    __WFI();
    
    // After wakeup, system resets (no code here reached)
}

// Power consumption estimates (STM32F4 @ 168 MHz)
// Run mode:      160 mA
// Sleep mode:    88 mA (peripherals still running)
// Stop mode:     0.8 mA (only SRAM powered)
// Standby mode:  0.01 mA (deepest sleep)
//
// Tradeoff: Power vs Wake-up time
// Run     → 0 ms wakeup  (always ready)
// Sleep   → < 1 µs wakeup (very fast)
// Stop    → ~10 µs wakeup (fast)
// Standby → ~1 sec wakeup (slow, full reset)

// Dynamic power management example
void adaptive_sleep_manager(uint32_t inactivity_ms) {
    static uint32_t last_activity = 0;
    uint32_t current_time = get_system_ticks();
    
    if ((current_time - last_activity) < 100) {
        // Recent activity - stay in run mode
        return;
    }
    
    if ((current_time - last_activity) < 5000) {
        // 100ms - 5s inactive - use sleep mode
        enter_sleep_mode();
    } else if ((current_time - last_activity) < 60000) {
        // 5s - 60s inactive - use stop mode
        enter_stop_mode();
    } else {
        // > 60s inactive - consider standby
        enter_standby_mode();
    }
    
    // Update activity timestamp on wakeup
    last_activity = get_system_ticks();
}

// Wakeup source configuration
void setup_wakeup_sources(void) {
    // 1. RTC alarm (for periodic wakeup)
    rtc_set_alarm_interval(60000);  // Wake every 60 seconds
    
    // 2. External interrupt (button)
    // Configure GPIO as external interrupt
    
    // 3. Serial communications (UART RX)
    // Enable UART wakeup interrupt
}
```

Microcontroller fundamentals are essential for embedded systems work!
