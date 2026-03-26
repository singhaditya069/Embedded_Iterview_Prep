# Bit Manipulation and Bitwise Operations - Q&A

## Bitwise Operators

### Q1: Bitwise AND (&) operation
**Answer:**
```c
// AND truth table: 1 & 1 = 1, others = 0
uint8_t a = 0b10110101;  // 181 decimal
uint8_t b = 0b11001010;  // 202 decimal
uint8_t result = a & b;  // 0b10000000 = 128

// Use case: Bit masking - extract specific bits
uint8_t status = 0b11010110;
uint8_t byte3 = (status & 0b00001000) >> 3;  // Extract bit 3
```

---

### Q2: Bitwise OR (|) operation
**Answer:**
```c
// OR truth table: 0 | 0 = 0, others = 1
uint8_t a = 0b10110101;
uint8_t b = 0b01001010;
uint8_t result = a | b;   // 0b11111111 = 255

// Use case: Set specific bits
uint8_t reg = 0b00001000;
reg |= 0b00000100;         // Set bit 2: 0b00001100
// Common: reg |= (1 << 2); // Set bit at position 2

// Enable multiple interrupts:
reg |= (1 << UART_INT) | (1 << TIMER_INT);
```

---

### Q3: Bitwise XOR (^) operation
**Answer:**
```c
// XOR truth table: 1 ^ 1 = 0, 1 ^ 0 = 1, 0 ^ 0 = 0
uint8_t a = 0b10110101;
uint8_t b = 0b10110101;
uint8_t result = a ^ b;   // 0b00000000 = 0 (same bits XOR to 0)

uint8_t c = 0b10110101;
uint8_t d = 0b11001010;
uint8_t result = c ^ d;   // 0b01111111 = 127

// Use case 1: Toggle bits
uint8_t flags = 0b00001010;
flags ^= 0b00001010;       // Toggle bits: 0b00000000

// Use case 2: Check if bits are same
if ((a ^ b) == 0) printf("Bits are identical\n");

// Use case 3: Parity check
uint8_t byte = 0b11010110;
uint8_t parity = byte;
parity ^= parity >> 4;     // Reduce bits
parity ^= parity >> 2;
parity ^= parity >> 1;
printf("Parity bit: %d\n", parity & 1);
```

---

### Q4: Bitwise NOT (~) operation
**Answer:**
```c
// NOT inverts all bits
uint8_t a = 0b10110101;  // 181
uint8_t result = ~a;      // 0b01001010 = 74

// BUT for unsigned types:
uint8_t val = ~0;         // 0xFF (all bits set)
printf("%d\n", val);      // 255

// Usually used to create masks:
#define CREATE_MASK(bits) (~0 << (bits))  // Mask for clearing

// Or to clear bits:
uint8_t val = 0b11111111;
val &= ~(1 << 3);         // Clear bit 3: ~(0b00001000) = 0b11110111
```

---

### Q5: Shift operators (<< and >>)
**Answer:**
```c
// Left shift (<<): Multiply by powers of 2
uint8_t a = 0b00001010;  // 10
uint8_t b = a << 1;      // 0b00010100 = 20 (multiply by 2)
uint8_t c = a << 2;      // 0b00101000 = 40 (multiply by 4)

// Right shift (>>): Divide by powers of 2
uint8_t d = 0b00101000;  // 40
uint8_t e = d >> 1;      // 0b00010100 = 20 (divide by 2)
uint8_t f = d >> 2;      // 0b00001010 = 10 (divide by 4)

// Important: Signed right shift fills with sign bit
int8_t neg = -4;         // 0b11111100 in two's complement
int8_t result = neg >> 1; // Arithmetic shift: 0b11111110 = -2

unsigned uint_val = 0x80000000U;
unsigned shifted = uint_val >> 1;  // Logical shift: 0x40000000

// Use in embedded: Bit extraction
uint8_t byte = 0b10110101;
uint8_t bit2 = (byte >> 2) & 1;    // Extract bit 2
```

---

## Bit Manipulation Techniques

### Q1: Setting and clearing bits
**Answer:**
```c
// Macros for bit operations
#define BIT_SET(reg, bit)    ((reg) |= (1U << (bit)))
#define BIT_CLEAR(reg, bit)  ((reg) &= ~(1U << (bit)))
#define BIT_TOGGLE(reg, bit) ((reg) ^= (1U << (bit)))
#define BIT_CHECK(reg, bit)  (((reg) >> (bit)) & 1U)

// Usage:
uint32_t gpio_status = 0;

BIT_SET(gpio_status, 5);     // Set bit 5
BIT_CLEAR(gpio_status, 3);   // Clear bit 3
BIT_TOGGLE(gpio_status, 2);  // Toggle bit 2

if (BIT_CHECK(gpio_status, 5)) {
    printf("Bit 5 is set\n");
}

// For register access:
volatile uint32_t *GPIO_BASE = (volatile uint32_t *)0x40020000;
BIT_SET(*GPIO_BASE, 10);     // Set bit 10 of register at 0x40020000
```

---

### Q2: Checking multiple bits
**Answer:**
```c
// Check if specific bits are set
#define MASK_CHECK(val, mask) (((val) & (mask)) == (mask))

uint8_t flags = 0b11010110;

// Check if bits 4, 5, 6 are all set
#define BITS_4_5_6 (0b01110000)
if (MASK_CHECK(flags, BITS_4_5_6)) {
    printf("All bits 4, 5, 6 are set\n");
}

// Check if at least one of bits 1, 2, 3 is set
#define BITS_1_2_3 (0b00001110)
if (flags & BITS_1_2_3) {
    printf("At least one of bits 1, 2, 3 is set\n");
}

// Check if any bit matches
if ((uart_status & (TX_READY | RX_READY))) {
    printf("UART is ready for transfer\n");
}
```

---

### Q3: Non-repeating element using XOR
**Answer:**
```c
// Find element appearing once when others appear twice
// Property: a ^ a = 0, a ^ 0 = a
int find_non_repeating(int arr[], int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= arr[i];  // Pairs cancel out, single remains
    }
    return result;
}

// Example:
int nums[] = {1, 1, 2, 2, 3, 4, 4};
int single = find_non_repeating(nums, 7);  // Returns 3

// Explanation:
// 0 ^ 1 ^ 1 ^ 2 ^ 2 ^ 3 ^ 4 ^ 4
// = (1 ^ 1) ^ (2 ^ 2) ^ (4 ^ 4) ^ 3
// = 0 ^ 0 ^ 0 ^ 3
// = 3
```

---

### Q4: Power of 2 checking
**Answer:**
```c
// Check if number is power of 2
// Powers of 2: 1, 2, 4, 8, 16, 32... (only one bit set)
#define IS_POWER_OF_2(n) ((n) > 0 && (((n) & ((n) - 1)) == 0))

// Explanation:
// 8 = 0b1000, 8-1 = 7 = 0b0111
// 8 & 7 = 0b1000 & 0b0111 = 0 ✓ (power of 2)
// 6 = 0b0110, 6-1 = 5 = 0b0101
// 6 & 5 = 0b0110 & 0b0101 = 0b0100 ≠ 0 ✗ (not power of 2)

if (IS_POWER_OF_2(buffer_size)) {
    printf("Buffer size is power of 2\n");  // Good for circular buffers
}

// Count set bits (Brian Kernighan's algorithm)
int count_set_bits(uint32_t n) {
    int count = 0;
    while (n) {
        n &= (n - 1);  // Remove rightmost set bit
        count++;
    }
    return count;
}
```

---

### Q5: Bit field structures
**Answer:**
```c
// Extract fields from packed data
struct Flags {
    uint8_t enable : 1;        // 1 bit
    uint8_t mode : 2;          // 2 bits (0-3)
    uint8_t priority : 2;      // 2 bits (0-3)
    uint8_t reserved : 3;      // Padding
};

struct Flags flags;
flags.enable = 1;              // Set only bit 0
flags.mode = 2;                // Set bits 1-2 to b'10'
flags.priority = 3;            // Set bits 3-4 to b'11'

// Or manual bit extraction:
uint8_t packed = 0b01100101;
uint8_t enable = (packed >> 0) & 1;     // Bit 0
uint8_t mode = (packed >> 1) & 0b11;    // Bits 1-2
uint8_t priority = (packed >> 3) & 0b11; // Bits 3-4

// Set bits manually:
uint8_t result = 0;
result |= (1 << 0);            // enable = 1
result |= (2 << 1);            // mode = 2
result |= (3 << 3);            // priority = 3
```

---

## Advanced Bitwise Operations

### Q1: Even/Odd check without modulo
**Answer:**
```c
// Even: last bit is 0, Odd: last bit is 1
#define IS_EVEN(n) (((n) & 1) == 0)
#define IS_ODD(n)  (((n) & 1) == 1)

int num = 42;
if (IS_EVEN(num)) printf("Even\n");

// Modulo using bitwise
#define MOD_POWER_2(n, p) ((n) & ((1 << (p)) - 1))
// n % 8 = n % 2^3 = n & 0b111 = n & 7 = n & ((1 << 3) - 1)
int remainder = MOD_POWER_2(23, 3);  // 23 % 8 = 7
```

---

### Q2: Swapping without temp variable
**Answer:**
```c
// Swap using XOR
int a = 5, b = 10;
a ^= b;      // a = a ^ b;
b ^= a;      // b = b ^ (a ^ b) = a
a ^= b;      // a = (a ^ b) ^ a = b

printf("a=%d, b=%d\n", a, b);  // a=10, b=5

// Bitwise swap also works:
a = a ^ b;
b = a ^ b;   // This extracts original a
a = a ^ b;   // This extracts original b
```

---

### Q3: Count set bits (population count)
**Answer:**
```c
// Count 1-bits in a number
int count_ones(uint32_t n) {
    int count = 0;
    while (n) {
        count += n & 1;    // Check last bit
        n >>= 1;           // Shift right
    }
    return count;
}

// Brian Kernighan's algorithm (faster)
int count_ones_fast(uint32_t n) {
    int count = 0;
    while (n) {
        n &= (n - 1);      // Remove rightmost 1-bit
        count++;           // Reduced iterations
    }
    return count;
}

// For 32-bit parallel counting
int count_ones_parallel(uint32_t n) {
    n = n - ((n >> 1) & 0x55555555U);
    n = (n & 0x33333333U) + ((n >> 2) & 0x33333333U);
    return ((n + (n >> 4) & 0x0F0F0F0FU) * 0x01010101U) >> 24;
}
```

---

### Q4: Reverse bits
**Answer:**
```c
// Reverse bits in a byte
uint8_t reverse_bits(uint8_t b) {
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        result <<= 1;              // Make room
        result |= (b & 1);         // Add rightmost bit
        b >>= 1;                   // Shift off processed bit
    }
    return result;
}

// Example:
uint8_t val = 0b10110101;
uint8_t rev = reverse_bits(val);  // 0b10101101

// For 32-bit (lookup table method for speed)
uint32_t reverse_32(uint32_t v) {
    v = ((v >> 1) & 0x55555555U) | ((v & 0x55555555U) << 1);
    v = ((v >> 2) & 0x33333333U) | ((v & 0x33333333U) << 2);
    v = ((v >> 4) & 0x0F0F0F0FU) | ((v & 0x0F0F0F0FU) << 4);
    v = ((v >> 8) & 0x00FF00FFU) | ((v & 0x00FF00FFU) << 8);
    v = (v >> 16) | (v << 16);
    return v;
}
```

---

## Real-world Embedded Applications

### Scenario: STM32 GPIO Register Control

```c
// Typical ARM Cortex-M GPIO register layout
typedef struct {
    uint32_t MODER;        // Mode register (2 bits per pin)
    uint32_t OTYPER;       // Output type
    uint32_t OSPEEDR;      // Output speed
    uint32_t PUPDR;        // Pull-up/Pull-down
    uint32_t IDR;          // Input data
    uint32_t ODR;          // Output data
    uint32_t BSRR;         // Bit set/reset
} GPIO_TypeDef;

// Mode values
#define GPIO_MODE_INPUT   0b00
#define GPIO_MODE_OUTPUT  0b01
#define GPIO_MODE_AF      0b10
#define GPIO_MODE_ANALOG  0b11

// Configure pin as output
void configure_gpio_output(GPIO_TypeDef *port, int pin) {
    // Clear mode bits for this pin
    port->MODER &= ~(0b11 << (pin * 2));
    // Set to output mode
    port->MODER |= (GPIO_MODE_OUTPUT << (pin * 2));
}

// Set pin value
#define GPIO_SET(port, pin)   ((port)->BSRR = (1U << (pin)))
#define GPIO_CLEAR(port, pin) ((port)->BSRR = (1U << ((pin) + 16)))
#define GPIO_READ(port, pin)  (((port)->IDR >> (pin)) & 1U)

// Usage:
GPIO_TypeDef *GPIOA = (GPIO_TypeDef *)0x40020000;
configure_gpio_output(GPIOA, 5);  // PA5 = output
GPIO_SET(GPIOA, 5);               // Set PA5 high
GPIO_CLEAR(GPIOA, 5);             // Set PA5 low
```

This covers all essential bit manipulation for embedded systems!
