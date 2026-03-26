# Hardware Basics for Embedded Systems - Q&A

## Electrical Fundamentals

### Q1: Voltage, Current, and Power basics
**Answer:**
- **Voltage (V)**: Electrical potential difference, drives current
  - Measured in volts (V)
  - Similar to pressure in a water pipe
  - Example: 3.3V, 5V, 12V supply

- **Current (I)**: Flow of electrical charge
  - Measured in amperes (A)
  - Similar to water flow rate
  - Example: 100 mA (0.1 A) for LED, 1 A for motor

- **Resistance (R)**: Opposition to current flow
  - Measured in ohms (Ω)
  - Ohm's Law: V = I × R

- **Power (P)**: Energy transfer rate
  - Measured in watts (W)
  - P = V × I
  - Important for heat dissipation

**Practical Example - LED Circuit:**
```c
// LED requires: 5V supply, 20mA current, safe operation
// Forward voltage of LED: ~2V
// Calculate resistor: R = (5V - 2V) / 0.02A = 150Ω
// Use standard 150Ω resistor

// Embedded perspective:
// GPIO pin can source/sink ~25mA (check datasheet!)
// Multiple LEDs on same pin increases current
```

---

### Q2: Logic levels for different MCU architectures
**Answer:**
```
Common Logic Levels:

TTL (Traditional)
├─ High (1): 2.0V to 5.5V
├─ Low (0): 0V to 0.8V
└─ Example: Arduino (5V)

CMOS
├─ High (1): VDD × 0.7 to VDD
├─ Low (0): 0V to VDD × 0.3
└─ Example: STM32 (3.3V)

ARM Cortex-M (Typical)
├─ High (1): 2.0V to 3.6V
├─ Low (0): 0V to 0.6V
└─ Example: STM32L4 (3.3V)
```

**Why it matters in embedded:**
```c
// DANGEROUS: Connecting 5V to STM32 is destructive
// STM32 max input: 3.6V
// If 5V applied: Damage to GPIO pin (reverse polarity, ESD)

// SAFE: Use level shifter or resistor divider
// 5V source → 10kΩ resistor → STM32 GPIO → 10kΩ resistor → GND
// Divides 5V to 2.5V (within safe range)

// Noise considerations:
// Add 10-100 nF capacitor next to GPIO for noise immunity
// Debounce button presses in firmware (20-50ms delay)
```

---

### Q3: Pull-up and pull-down resistors
**Answer:**
```c
// Pull-up: Resistor connects signal to VDD (high)
// When switch open: GPIO = 3.3V (HIGH)
// When switch closed: GPIO = 0V (LOW)

// Pull-down: Resistor connects signal to GND
// When switch open: GPIO = 0V (LOW)
// When switch closed: GPIO = 3.3V (HIGH)

// Configuration:
GPIO_MODE_INPUT_PULLUP       // GPIO at 3.3V when floating
GPIO_MODE_INPUT_PULLDOWN     // GPIO at 0V when floating
GPIO_MODE_INPUT              // GPIO undefined when floating (bad!)

// Internal pull-ups (MCU provides):
void init_button(void) {
    // STM32 example
    uint32_t *GPIO_MODE = (uint32_t *)0x40020000;
    *GPIO_MODE = GPIO_MODE_INPUT_PULLUP;  // Enable internal pull-up
}

// Button reading:
#define BTN_PRESSED()  (GPIO_READ(GPIOA, 0) == 0)  // Read input

// Debouncing with pull-up:
uint8_t button_read_debounced(void) {
    uint8_t sample1 = GPIO_READ(GPIOA, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    uint8_t sample2 = GPIO_READ(GPIOA, 0);
    
    if (sample1 == sample2) {
        return sample1;  // Stable reading
    }
    return button_read_debounced();  // Retry if noise
}
```

---

## Digital Electronics

### Q1: Digital logic gates (AND, OR, NOT, NAND, NOR, XOR)
**Answer:**
```
Truth tables and usage:

AND gate: Both inputs must be 1
Input A | Input B | Output
0       | 0       | 0
0       | 1       | 0
1       | 0       | 0
1       | 1       | 1
Use: Enable signal (gate between data and output)

OR gate: At least one input must be 1
Input A | Input B | Output
0       | 0       | 0
0       | 1       | 1
1       | 0       | 1
1       | 1       | 1
Use: Interrupt aggregation (multiple sources trigger)

NOT gate: Inverts input
Input | Output
0     | 1
1     | 0
Use: Signal inversion, reset pins

XOR gate: Inputs must differ
Input A | Input B | Output
0       | 0       | 0
0       | 1       | 1
1       | 0       | 1
1       | 1       | 0
Use: Parity, comparison
```

**In embedded firmware:**
```c
// Simulating gates in software
#define AND_GATE(a, b)  ((a) && (b))
#define OR_GATE(a, b)   ((a) || (b))
#define NOT_GATE(a)     (!(a))
#define XOR_GATE(a, b)  ((a) ^ (b))

// Practical: Enable ADC only when both conditions met
if (AND_GATE(sensor_ready, dma_complete)) {
    start_conversion();
}
```

---

## Microcontroller Basics

### Q1: MCU memory layout (Flash vs RAM vs EEPROM)
**Answer:**
```
Three types of memory on STM32 (typical MCU):

FLASH (Non-volatile, non-erasable during operation)
├─ Size: 128 KB to 512 KB
├─ Purpose: Program storage
├─ Speed: Slower writes (typical: 1-2 ms per page)
├─ Typical structure:
│  ├─ Bootloader (0x00000000 - 0x00002000)
│  └─ Application (0x00002000 - end)
└─ Use: Store code, constants, lookup tables

RAM (Volatile, fast, erasable, lost on power)
├─ Size: 8 KB to 64 KB
├─ Purpose: Variables, stacks, heaps
├─ Speed: Fastest (nanoseconds for read/write)
├─ Speed: ~100+ MB/sec bandwidth
└─ Use: Runtime data, RTOS task stacks

EEPROM (Non-volatile, slower, limited writes)
├─ Size: Small (1-4 KB)
├─ Purpose: Configuration storage
├─ Write cycles: Limited (100,000+ cycles typical)
├─ Erase time: 1-10 ms per byte
└─ Note: STM32 uses Flash emulation via firmware
```

**Memory layout diagram:**
```
0x00000000 ┌─────────────────┐
           │  Boot Code      │ (Bootloader or application entry)
           │  (4 KB)         │
0x00001000 ├─────────────────┤
           │  Application    │ (Main program)
           │  Code           │
           │  (200 KB)       │
0x00032000 ├─────────────────┤
           │  Constants      │ (Read-only data)
           │  (20 KB)        │
0x00037000 ├─────────────────┤
           │  Config Data    │ (Stored in Flash, not cleared on boot)
           │  (4 KB)         │
0x00038000 └─────────────────┘ (End of Flash)

0x20000000 ┌─────────────────┐
           │  Global Data    │ (Initialized to values from Flash)
           │  (4 KB)         │
0x20001000 ├─────────────────┤
           │  BSS (zeros)    │ (Uninitialized globals)
           │  (4 KB)         │
0x20002000 ├─────────────────┤
           │  Heap           │ (Dynamic malloc)
           │  (grows up)     │
           ├─────────────────┤
           │  Stack          │ (grows down)
           │  (RTOS tasks)   │
0x20008000 └─────────────────┘ (End of RAM)
```

---

### Q2: Clock and timing - oscillators and PLLs
**Answer:**
```c
// Clock sources:

1. Internal oscillator (limited accuracy)
   - Frequency: ~8 MHz typical
   - Accuracy: ±1% (poor for communication protocols)
   - Use: When external oscillator not available

2. External crystal oscillator (high accuracy)
   - Frequency: 8-32 MHz typical (STM32 uses 8 MHz on many boards)
   - Accuracy: ±50 ppm (good for UART/SPI)
   - Use: Primary clock source

3. PLL (Phase-Locked Loop) - multiplier
   - Input: 8 MHz
   - Multiplier: ×168 (for STM32F4)
   - Output: 1.34 GHz (but internal dividers reduce to usable frequency)
   - Use: Boost frequency for faster execution

// Typical STM32 configuration:
// 8 MHz crystal → PLL ×168 → Main clock dividers
#define HSE_VALUE 8000000   // External oscillator 8 MHz
#define PLL_M     8         // Divide to 1 MHz
#define PLL_N     336       // Multiply to 336 MHz
#define PLL_P     2         // Divide to 168 MHz (system clock)
#define PLL_Q     7         // USB clock 48 MHz
```

**Impact on RTOS:**
```c
// The clock frequency determines tick rate for RTOS scheduler
#define configCPU_CLOCK_HZ 168000000  // 168 MHz
#define configTICK_RATE_HZ 1000       // 1 ms per tick

// Example: 1 ms tick @ 168 MHz
// Timer interrupt every 168,000 CPU cycles
// Switch tasks at this frequency
```

---

### Q3: Reset and interrupt sources
**Answer:**
```
Reset Sources (hard reset):

Power-on Reset (POR)
├─ When: Power connected to MCU
├─ Action: Initialize all registers to default
└─ Code: Not executed (MCU not running yet)

Brown-out Reset (BOR)
├─ When: Supply voltage drops below threshold (~2.6V)
├─ Action: Reset MCU to protect data corruption
└─ Code: Not executed

Watchdog Reset
├─ When: Watchdog counter timeout (software fault)
├─ Action: Force complete reset
└─ Use: Recover from infinite loops

External Reset
├─ When: Pin pulled low (RESET pin)
├─ Action: Reset MCU like power-on
└─ Use: External debugger, manual reset button

Software Reset
├─ When: Code explicitly triggers (NVIC_SystemReset())
├─ Action: Reset like power-on
└─ Use: Safe restart, bootloader hand-off

Interrupt Sources (don't reset, just interrupt execution):

Exception Types (highest priority):
1. Reset (hard reset, highest priority)
2. NMI (Non-maskable interrupt) - watchdog, etc
3. Hard Fault - illegal instruction
4. Memory Management Fault - MPU violation
5. Bus Fault - bus error
6. Usage Fault - undefined instruction

Peripheral Interrupts (lower priority):
- GPIO (pin change)
- UART (data received)
- SPI (transfer complete)
- Timer (match event)
- ADC (conversion complete)
- DMA (transfer complete)
```

**Implementing interrupts:**
```c
// Vector table setup (first thing on MCU startup)
// Address 0x00000000: Stack pointer initial value
// Address 0x00000004: Reset handler address
// Address 0x00000008: NMI handler address
// ...

// Interrupt handler example (UART receive)
void USART1_IRQHandler(void) {
    // Automatically called when UART data received
    uint8_t data = USART1->DR;  // Read received byte
    
    // Must be fast! (microseconds)
    // Set flags for task or directly process
    xQueueSendFromISR(uart_queue, &data, NULL);
}

// Enable interrupt:
NVIC_EnableIRQ(USART1_IRQn);
NVIC_SetPriority(USART1_IRQn, 5);  // Priority 5 (0=highest)
```

---

## Analog Circuits

### Q1: Operational amplifiers (op-amps) basics
**Answer:**
```
Op-amp basics:
- Differential input: V+ and V-
- Single output
- Very high gain (100,000+ typical)
- Applications: Amplify difference between inputs

Configurations:

1. Comparator (simplest)
   V+ → Input 1
   V- → Reference (threshold)
   Output: HIGH if V+ > V-
   Use: Trigger when sensor exceeds threshold

2. Inverting amplifier
   Input → Resistor Rin → Input
   Feedback → Resistor Rf → Input
   Gain = -Rf / Rin
   
3. Non-inverting amplifier
   Input → V+
   Feedback resistor divider
   Gain = 1 + Rf / Rin

4. Buffer (unity gain)
   V+ = Input
   V- = Output (feedback)
   Gain = 1
   Use: Impedance matching, buffering
```

**In embedded MCU context:**
```c
// Op-amp output → ADC input
// Amplify weak sensor signal before digitizing

#define ADC_VREF    3300   // 3.3V reference in mV
#define ADC_BITS    12     // 12-bit ADC (0-4095)
#define GAIN        10     // Op-amp gain

void read_amplified_sensor(void) {
    uint16_t raw = adc_read();
    uint16_t mv = (raw * ADC_VREF) / ((1 << ADC_BITS) - 1);
    uint16_t actual_signal = mv / GAIN;  // Remove op-amp gain
}
```

---

## PCB Design Concepts

### Q1: Decoupling and bypass capacitors
**Answer:**
```c
// Decoupling capacitor (bypass) purpose:
// Absorb sudden current spikes from digital logic
// Keep voltage stable at IC power pins

// Typical values:
#define CERAMIC_CAP_100NF  0.0000001  // 100nF = 0.1µF
#define CERAMIC_CAP_10UF   0.000010   // 10µF

// Placement rules:
// 1. One 100nF cap per IC power pin (as close as possible)
// 2. One larger cap (10µF) per power domain
// 3. Short traces to ground plane

// Example circuit:
//
// VDD (3.3V)
//  │
//  ├─[100nF cap]─┐
//  │             │
//  IC (MCU)    GND
//  │
//  ├─[10µF cap]──┐
//  │             │
// GND plane    GND

// Without decoupling capacitor:
// - Transient noise spikes
// - Digital noise couples into analog circuits
// - ADC conversions have errors
// - Processor resets at worst

// With decoupling:
// - Stable voltage
// - Low noise
// - Reliable operation
```

---

## Embedded Real-World Scenario

### Scenario: Sensor Conditioning Circuit

```c
// Typical sensor chain:

Sensor → Amplifier → Filter → ADC → MCU

// Implementation:

// 1. Sensor: Thermistor (temperature)
//    Output: 0-5V (needs level shifting)

// 2. Op-amp stage (gain = 10)
//    Amplify 100mV range to 1V range
//    Output: 0-3.3V (MCU compatible)

// 3. Low-pass filter
//    RC filter or op-amp integrator
//    Removes high-frequency noise

// 4. ADC sampling
uint16_t temperature_count;

void init_adc_channel(void) {
    // Configure ADC for 12-bit, continuous sampling
    adc_config.resolution = 12;
    adc_config.sampletime = 100;  // 100 ADC clocks per sample
}

void adc_complete_callback(void) {
    temperature_count = adc_read();  // 0-4095 for 3.3V
}

uint16_t get_temperature_celsius(void) {
    float mv = (temperature_count * 3300) / 4095;
    float celsius = (mv - 500) / 10;  // 500mV = 0°C, 10mV/°C
    return (uint16_t)celsius;
}
```

This hardware basics guide provides essential knowledge for embedded systems development!
