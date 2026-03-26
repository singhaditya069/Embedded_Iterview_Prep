# Hardware Basics - Comprehensive Q&A

## Registers and Memory-Mapped I/O

### Q1: Understanding hardware registers and bit fields
**Answer:**
```c
#include <stdint.h>
#include <stddef.h>

// Registers are memory locations that control hardware
// Located at fixed addresses in the memory map

// STM32F4 GPIO example (Address: 0x40020000 + offset)

// METHOD 1: Direct volatile pointer (bad - error-prone)
volatile uint32_t *gpio_odr = (volatile uint32_t *)0x40020014;
*gpio_odr = 0x00000020;  // Problem: Easy to make mistakes

// METHOD 2: Struct definition (better)
typedef struct {
    volatile uint32_t MODER;      // 0x00: Mode register
    volatile uint32_t OTYPER;     // 0x04: Output type register
    volatile uint32_t OSPEEDR;    // 0x08: Output speed register
    volatile uint32_t PUPDR;      // 0x0C: Pull-up/pull-down
    volatile uint32_t IDR;        // 0x10: Input data register
    volatile uint32_t ODR;        // 0x14: Output data register
    volatile uint32_t BSRR;       // 0x18: Bit set/reset register
    volatile uint32_t LCKR;       // 0x1C: Port configuration lock
    volatile uint32_t AFRL;       // 0x20: Alternate function (low)
    volatile uint32_t AFRH;       // 0x24: Alternate function (high)
} GPIO_TypeDef;

// Create mapped structure
#define GPIOA ((GPIO_TypeDef *)0x40020000)
#define GPIOB ((GPIO_TypeDef *)0x40020400)
#define GPIOC ((GPIO_TypeDef *)0x40020800)

// Now access registers cleanly
GPIOA->ODR |= (1 << 5);   // Set PA5 high
GPIOA->ODR &= ~(1 << 5);  // Set PA5 low
uint8_t pin_state = (GPIOA->IDR >> 5) & 1;  // Read PA5

// METHOD 3: Bit field struct (most user-friendly)
typedef struct {
    uint32_t MODE0 : 2;      // Bits 0-1 (2 bits)
    uint32_t MODE1 : 2;      // Bits 2-3
    uint32_t MODE2 : 2;      // Bits 4-5
    // ... etc
    uint32_t RESERVED : 4;   // Padding
} GPIO_MODER_Bits;

typedef union {
    GPIO_MODER_Bits bits;
    uint32_t word;
} GPIO_MODER_t;

// Set mode for PA2 using bit field
GPIO_TypeDef_BitField gpioa;
gpioa.MODER.bits.MODE2 = 1;  // Set PA2 to output mode

// Register bitfield values for GPIO mode:
#define GPIO_MODE_INPUT     0  // 00
#define GPIO_MODE_OUTPUT    1  // 01
#define GPIO_MODE_ALTERNATE 2  // 10
#define GPIO_MODE_ANALOG    3  // 11
```

**Register types:**
```c
// Read-Only Register (RO)
// Can only read, writing has no effect
volatile uint32_t *status_reg;
uint32_t status = *status_reg;  // OK
*status_reg = 0;                // No effect (writing ignored)

// Write-Only Register (WO)
// Writing has effect, reading shows undefined value
volatile uint32_t *control_reg;
*control_reg = 0x01;            // OK
uint32_t val = *control_reg;    // Returns undefined!

// Read-Write Register (RW)
// Can read and write
volatile uint32_t *rw_reg;
*rw_reg = 0xFFFF;               // Write
uint32_t val = *rw_reg;         // Read

// W1C (Write 1 to Clear)
// Writing 1 clears the bit, writing 0 has no effect
volatile uint32_t *irq_status;
*irq_status = (1 << 5);         // Clear bit 5
// Note: *irq_status &= ~(1 << 5) is WRONG - would read first!

// W1S (Write 1 to Set)
volatile uint32_t *irq_enable;
*irq_enable = (1 << 3);         // Set bit 3 to enable

// Toggle bit (XOR)
volatile uint32_t *reg;
*reg ^= (1 << 2);               // Toggle bit 2
```

---

### Q2: ADC (Analog-to-Digital Converter) configuration
**Answer:**
```c
#include <stdint.h>

// ADC in STM32F4
typedef struct {
    volatile uint32_t SR;       // Status register
    volatile uint32_t CR1;      // Control register 1
    volatile uint32_t CR2;      // Control register 2
    volatile uint32_t SMPR1;    // Sample time register 1
    volatile uint32_t SMPR2;    // Sample time register 2
    volatile uint32_t JOFR1;    // Injected offset register 1
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;      // Watchdog higher threshold register
    volatile uint32_t LTR;      // Watchdog lower threshold register
    volatile uint32_t SQR1;     // Regular sequence register 1
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;     // Injected sequence register
    volatile uint32_t JDR1;     // Injected data register 1
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;       // Regular data register
} ADC_TypeDef;

#define ADC1 ((ADC_TypeDef *)0x40012000)

// ADC Sampling modes
#define SAMPLE_1_5_CYCLES    0    // 1.5 ADC cycles (fast, less accurate)
#define SAMPLE_7_5_CYCLES    1    // 7.5 cycles
#define SAMPLE_13_5_CYCLES   2    // 13.5 cycles
#define SAMPLE_28_5_CYCLES   3    // 28.5 cycles
#define SAMPLE_55_5_CYCLES   4    // 55.5 cycles (slow, more accurate)
#define SAMPLE_71_5_CYCLES   5
#define SAMPLE_239_5_CYCLES  6

// Conversion modes
#define SINGLE_CONVERSION    0    // Convert once
#define CONTINUOUS_MODE      1    // Keep converting

// ADC resolution
#define RESOLUTION_12BIT     0    // 0-4095 (12 bits)
#define RESOLUTION_10BIT     1    // 0-1023
#define RESOLUTION_8BIT      2    // 0-255
#define RESOLUTION_6BIT      3    // 0-63

// Initialize ADC for temperature sensor
void adc_init_temperature(void) {
    // Enable ADC1 clock (RCC setup required first)
    
    // Set resolution to 12-bit
    ADC1->CR1 |= (0 << 24);  // RES[1:0] = 00 (12-bit)
    
    // Set sampling time for channel 16 (temperature sensor)
    // Use longer sampling for temperature stability
    ADC1->SMPR1 |= (SAMPLE_239_5_CYCLES << 18);  // Ch16 sampling time
    
    // Enable temperature sensor
    ADC1->CR2 |= (1 << 23);  // TSVREFE = 1
    
    // Set scan mode (scan multiple channels)
    ADC1->CR1 |= (1 << 8);   // SCAN = 1
    
    // Set continuous mode
    ADC1->CR2 |= (1 << 1);   // CONT = 1
    
    // Enable ADC
    ADC1->CR2 |= (1 << 0);   // ADON = 1
    
    // Wait for startup
    for (volatile int i = 0; i < 1000; i++);
    
    // Start first conversion
    ADC1->CR2 |= (1 << 30);  // SWSTART = 1
}

// Read ADC value with polling
uint16_t adc_read_channel(uint8_t channel) {
    // Configure sequence to read single channel
    ADC1->SQR3 &= ~0x1F;     // Clear first channel
    ADC1->SQR3 |= channel;   // Set channel in sequence
    
    // Wait for conversion to complete
    while (!(ADC1->SR & (1 << 1)));  // Wait for EOC (end of conversion)
    
    // Clear EOC flag
    ADC1->SR &= ~(1 << 1);
    
    // Return converted value
    return (uint16_t)(ADC1->DR & 0xFFF);  // 12-bit mask
}

// Read temperature from internal sensor
int16_t adc_read_temperature(void) {
    uint16_t adc_value = adc_read_channel(16);  // Channel 16 = temp sensor
    
    // Temperature = ((V - V25) / TS_Slope) + 25
    // Where V25 = 0.76V typical at 25°C
    // TS_Slope = 2.5mV/°C
    
    // For STM32F4 @ 3.3V reference:
    // ADC_Value = (Temperature - 25) * (3300uV / (2.5mV * 4095 codes))
    // Temperature = 25 + ((ADC_Value - 1480) * 410) / 1000
    
    int16_t temp_c = 25 + ((adc_value - 1480) * 410) / 1000;
    return temp_c;
}

// DMA-based continuous ADC reading
void adc_init_with_dma(void) {
    // Setup DMA to transfer ADC values automatically
    // When conversion completes, DMA copies to memory
    
    // Configure DMA to read ADC->DR repeatedly
    // This is more efficient than polling
}

// ADC accuracy tips:
// ✓ Settle time: Wait after powering on ADC
// ✓ Sampling time: Longer = better accuracy (more settling)
// ✓ References: Use stable 3.3V reference
// ✓ Filtering: Average multiple readings
// ✓ GPIO config: Keep GPIO low impedance
// ✓ Layout: Keep analog traces away from digital
// ✓ Decoupling: Add capacitors near ADC Vref
```

---

## Electrical Fundamentals for Embedded

### Q1: Logic levels and voltage translation
**Answer:**
```
Logic Level Standards:

TTL (Transistor-Transistor Logic) - 5V System
├─ Low:  0V to 0.8V     (represents 0/false)
├─ High: 2.0V to 5V     (represents 1/true)
└─ Undefined: 0.8V to 2.0V (invalid)

LVCMOS (Low Voltage) - 3.3V System
├─ Low:  0V to 0.5V     (represents 0)
├─ High: 2.8V to 3.3V   (represents 1)
└─ Undefined: 0.5V to 2.8V (invalid)

CMOS (Rail-to-rail) - 3.3V/5V (depends on supply)
├─ Low:  ~0.3V
├─ High: ~2.7V to 5V
└─ Better noise immunity

STM32 (Cortex-M4) - 3.3V Logic
├─ Input: 0V to 3.3V (5V tolerant on most pins)
├─ Output: 0V to 3.3V
└─ Critical: Mixing 5V to 3.3V input can damage!
```

**Level shifting problem:**
```c
// Problem: Connecting 5V device to 3.3V microcontroller

// WRONG - Direct connection:
// 5V sensor output → 3.3V STM32 input
// Risk: Exceeds max input voltage, damages GPIO

// SOLUTION 1: Voltage Divider (simple, passive)
// 5V ──┬──── 1kΩ resistor ─────┬──── 3.3V GPIO pin
//      │                       │
//     5V sensor              680Ω resistor
//      │                       │
//      └───────────────────────┴──── GND
//
// Vout = Vin * R2 / (R1 + R2)
//      = 5V * 680 / (1000 + 680) = 1.79V (within 3.3V range!)

// SOLUTION 2: Logic Level Converter (active, reliable)
// 5V side ──┬──[TXS0108E]──┬── 3.3V side
//           │ Level Conv  │
// GND ──────┴─────────────┴──── GND
//
// Integrated circuit handles voltage translation
// Bidirectional capable

// SOLUTION 3: Open Drain (if available on both sides)
// 5V ──┬──[10kΩ pull-up]──┬──── 3.3V GPIO
//      │                  │
//   5V device output      STM32 GPIO (open drain)
//      │                  │
//      └──────────────────┴──── GND

// SOLUTION 4: Resistor on 5V side (simple for digital output)
// 5V output ──[10kΩ resistor]──┬──── 3.3V input
//                               │
//                             GND (optional pulldown)

#define VOLTAGE_DIVIDER_R1 1000  // 1kΩ
#define VOLTAGE_DIVIDER_R2 680   // 680Ω

uint16_t apply_voltage_divider(uint16_t adc_reading) {
    // If ADC reads the divided 5V signal:
    // ADC value is proportional to 5V input
    // Convert back to 5V equivalent
    
    // V5 = Vadc * (R1 + R2) / R2
    //    = Vadc * (1000 + 680) / 680
    //    = Vadc * 1.47
    
    return (adc_reading * 1470) / 1000;  // Scale back to 5V range
}
```

---

## PCB Design Basics for Firmware Engineers

### Q1: Decoupling capacitors and noise management
**Answer:**
```
Decoupling capacitor placement:

Power Supply
│
├─ [100µF Electrolytic] ─── Bulk capacitor
│                          (supply stabilization)
│
├─ [10µF Ceramic] ───────── Medium capacitor
│                          (switching transients)
│  
├─ [0.1µF Ceramic] ───────┐
├─ [0.1µF Ceramic] ───────┼─ MCU Vcc pins
├─ [0.1µF Ceramic] ───────┤ (one per pin pair)
├─ [0.1µF Ceramic] ───────┤
│                         │
└─ [GND plane] ──────────┘

Guidelines:
✓ Place 0.1µF closest to each Vcc pin
✓ Use low-inductance ceramic capacitors (X7R grade)
✓ Keep traces short (< 1cm from IC)
✓ Connect directly to ground plane
✓ Use multiple vias to ground
✓ Place bulk capacitor within 2cm of IC

Capacitor values by frequency handled:
- 100µF:     <10 kHz (bulk supply)
- 10µF:      10 kHz - 100 kHz
- 1µF:       100 kHz - 1 MHz
- 0.1µF:     1 MHz - 10 MHz (most important!)
- 0.01µF:    >10 MHz (high-speed signals)

Bad PCB = intermittent failures, resets, glitches!
Good PCB = stable, predictable operation
```

**Firmware impact:**
```c
// With poor decoupling:
// - Random resets (brownout detects VDD sag)
// - SPI/I2C communication errors
// - ADC noise and wrong readings
// - Peripheral malfunctions
// - Hard to debug (only happens sometimes)

// Typical symptom code (indicates hardware issue):
void init_system(void) {
    // If this randomly loops, suspect power supply!
    for (int i = 0; i < 100; i++) {
        if (read_wdt_status()) {
            // Watchdog firing = power glitch happened
            reset_watchdog();
            printf("WDT reset!\n");
        }
    }
}

// Better: Monitor supply voltage
void monitor_power_supply(void) {
    uint16_t vdd = adc_read_supply_voltage();
    
    if (vdd < 3.0) {
        printf("WARNING: Low supply voltage %d mV\n", vdd);
        // Trigger graceful shutdown or alert
        led_blink_error();
    }
}
```

Integration of hardware and firmware ensures reliable systems!
