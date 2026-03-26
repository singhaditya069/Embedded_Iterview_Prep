# Quick Reference - Last Minute Review Cheat Sheet

## ⚡ Essential Facts (2 Hours Before Interview)

### Pointers (Fundamental)
```c
int x = 5;
int *p = &x;           // Pointer to x
int **pp = &p;         // Pointer to pointer
int *arr[10];          // Array of pointers
int (*parr)[10];       // Pointer to array
int (*func_ptr)(int);  // Function pointer
*p = 10;               // Dereference
(*func_ptr)(5);        // Call function via pointer
```
**Remember**: `*` on left of `=` means "store at address", on right means "load from address"

---

### Memory Layout
```
┌─────────────────────┐ High address (0xFFFFFFFF)
│                     │
│    RUNTIME STACK ↑  │ (grows upward)
│ (local variables)   │
│                     │
├─────────────────────┤
│                     │
│    HEAP ↓           │ (grows downward)
│ (malloc/new)        │
├─────────────────────┤
│  .bss (uninitialized)│ (500+ KB)
├─────────────────────┤
│  .data (initialized)│ (100+ KB)
├─────────────────────┤
│  .text (code)       │ (1-2 MB)
│  .rodata (constants)│
└─────────────────────┘ Low address (0x00000000)
```

**Stack vs Heap**:
- Stack: Fast, limited size, automatic cleanup
- Heap: Slow, large size, manual cleanup

---

### Bit Manipulation Cheat Sheet
```c
x & y           // AND: both bits 1
x | y           // OR: at least one bit 1
x ^ y           // XOR: bits different
~x              // NOT: flip all bits
x << n          // Left shift (multiply by 2^n)
x >> n          // Right shift (divide by 2^n)
x & (1 << n)    // Check if bit n is set
x |= (1 << n)   // Set bit n
x &= ~(1 << n)  // Clear bit n
x ^= (1 << n)   // Toggle bit n
```

---

### Microcontroller Basics (ARM Cortex-M4)
```c
// Register access
#define GPIOA_BASE   0x40020000
#define GPIO_MODER   *(volatile uint32_t *)(GPIOA_BASE + 0x00)
#define GPIO_ODR     *(volatile uint32_t *)(GPIOA_BASE + 0x14)

// GPIO output (PA3)
GPIO_MODER |= (0x1 << 6);    // Set bits 7:6 to 01 (output)
GPIO_ODR |= (1 << 3);        // Set PA3 high
GPIO_ODR &= ~(1 << 3);       // Set PA3 low

// Interrupt (NVIC)
NVIC_EnableIRQ(GPIO_IRQn);      // Enable
NVIC_SetPriority(GPIO_IRQn, 1); // Priority 1

void GPIO_ISR(void) {
    // Handler code
    NVIC_ClearPendingIRQ(GPIO_IRQn);  // Clear interrupt
}
```

---

### Common Data Structures

#### Circular Buffer (Most Important!)
```c
#define SIZE 10
uint8_t buffer[SIZE];
int head = 0, tail = 0;

void enqueue(uint8_t data) {
    buffer[head] = data;
    head = (head + 1) % SIZE;
    if (head == tail) tail = (tail + 1) % SIZE;  // Overflow
}

uint8_t dequeue(void) {
    if (head == tail) return -1;  // Empty
    uint8_t data = buffer[tail];
    tail = (tail + 1) % SIZE;
    return data;
}

int is_empty(void) { return head == tail; }
```

#### Linked List Node
```c
struct Node {
    int data;
    struct Node *next;
};

struct Node *head = NULL;

void insert_at_head(int data) {
    struct Node *new = malloc(sizeof(struct Node));
    new->data = data;
    new->next = head;
    head = new;
}

void delete_node(struct Node *prev, struct Node *node) {
    prev->next = node->next;
    free(node);
}
```

---

### UART Communication (STM32)
```c
// Configuration
#define UART1_BASE 0x40011000
#define UART_SR    *(volatile uint32_t *)(UART1_BASE + 0x00)
#define UART_DR    *(volatile uint32_t *)(UART1_BASE + 0x04)
#define UART_BRR   *(volatile uint32_t *)(UART1_BASE + 0x08)

// Setup baud rate (APB frequency / baud)
UART_BRR = 72000000 / 115200;  // For 72MHz clock

// Transmit
while (!(UART_SR & (1 << 7)));  // Wait TXE (transmit empty)
UART_DR = 'A';

// Receive
while (!(UART_SR & (1 << 5)));  // Wait RXNE (receive not empty)
char c = UART_DR;

// Interrupt
void UART1_IRQ_Handler(void) {
    if (UART_SR & (1 << 5)) {  // RXNE
        char data = UART_DR;
        // Process data
    }
}
```

---

### I2C Protocol Sequence
```
START condition: SDA goes LOW while SCL is HIGH

WRITE byte (0xA4):
  SCL  ────┐        ┐        ┐      ┌────
       ────┘────┐───┘────┐───┘──────┘
  SDA  ────┐    └─────┐      ┐
       ────┘─────────┘──────┘────────
       START  1  0  1  0  0  1  0  0  ACK
       
STOP condition: SDA goes HIGH while SCL is HIGH

ACK: Slave pulls SDA low during 9th clock pulse
NACK: Slave releases SDA (master sees HIGH)
```

---

### State Machine Template
```c
// 1. Define states and events
typedef enum { STATE_OFF, STATE_ON } State_t;
typedef enum { EVENT_START, EVENT_STOP } Event_t;

// 2. State handlers
void on_enter_off(void) { /* cleanup */ }
void on_enter_on(void) { /* activate */ }

// 3. Transition table
const State_t transitions[STATE_COUNT][EVENT_COUNT] = {
    [STATE_OFF][EVENT_START] = STATE_ON,
    [STATE_ON][EVENT_STOP] = STATE_OFF,
};

// 4. Handle event
void process_event(Event_t event) {
    State_t new_state = transitions[current_state][event];
    if (new_state != current_state) {
        current_state = new_state;
        if (current_state == STATE_ON) on_enter_on();
        else on_enter_off();
    }
}
```

---

### Performance Optimization Quick Wins
```c
// 1. Loop unrolling
for (i = 0; i < 100; i += 4) {
    process(i); process(i+1); process(i+2); process(i+3);
}

// 2. Inline functions
static inline int add(int a, int b) { return a + b; }

// 3. Use bit operations instead of division
x >> 2  // Instead of x / 4
x << 1  // Instead of x * 2

// 4. Cache-friendly access patterns
for (row = 0; row < ROWS; row++)      // ✓ Good
    for (col = 0; col < COLS; col++)
        process(matrix[row][col]);

// 5. Lookup tables for expensive calculations
uint8_t sine_table[256];  // Pre-calculated sines
value = sine_table[index];  // O(1) vs sin() calculation
```

---

### RTOS Essentials
```c
// Task creation
TaskHandle_t h = NULL;
xTaskCreate(task_func, "name", stack_size, param, priority, &h);

// Semaphore
SemaphoreHandle_t sem = xSemaphoreCreateBinary();
xSemaphoreTake(sem, portMAX_DELAY);      // Wait
xSemaphoreGive(sem);                     // Signal

// Mutex
xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
xSemaphoreGiveRecursive(mutex);

// Message queue
QueueHandle_t q = xQueueCreate(10, sizeof(int));
xQueueSend(q, &data, portMAX_DELAY);
xQueueReceive(q, &data, portMAX_DELAY);

// Delay
vTaskDelay(10);    // 10 ticks (not milliseconds!)
```

---

### Debugging Checklist
```
If code doesn't work:

1. ☐ Check return values from functions (malloc, UART init, etc.)
2. ☐ Initialize all variables (stack junk!)
3. ☐ Check for off-by-one errors
4. ☐ Verify pointer dereferencing
5. ☐ Check array bounds
6. ☐ Look for integer overflow
7. ☐ Check if ISRs are enabled
8. ☐ Verify register addresses
9. ☐ Check clock configuration
10. ☐ Look for race conditions (RTOS)
11. ☐ Use breakpoints and step through
12. ☐ Dump memory to verify state
```

---

### Common Interview Mistakes
```
❌ NOT checking malloc return value
    int *p = malloc(100);  // What if it fails?
✓ Check:
    if (!p) { handle_error(); }

❌ Using uninitialized variable
    int x;  // Contains garbage
✓ Initialize:
    int x = 0;

❌ Off-by-one in loop
    for (i = 0; i <= 10; i++) arr[i];  // arr[10] out of bounds
✓ Use:
    for (i = 0; i < 10; i++) arr[i];

❌ Not disabling interrupts for critical section
    shared_var++;  // Race condition!
✓ Use:
    __disable_irq();
    shared_var++;
    __enable_irq();

❌ Forgetting to clear interrupt flag
    void ISR(void) { /* code */ }  // ISR called again immediately!
✓ Clear:
    void ISR(void) { /* code */ NVIC_ClearPendingIRQ(IRQn); }
```

---

### Quick Problem-Solving Flowchart
```
Problem → 1. Understand requirement
          ↓
         2. Identify data structures needed
          ↓
         3. Write pseudocode/algorithm
          ↓
         4. Handle edge cases (NULL, overflow, etc.)
          ↓
         5. Code with defensive practices
          ↓
         6. Test with examples
          ↓
         7. Optimize if needed
          ↓
         Solution!
```

---

### 10 Must-Know Concepts
```
1. Pointers and dereferencing
2. Stack vs Heap memory
3. Circular buffers
4. State machines
5. Interrupt handling
6. GPIO and register access
7. UART communication
8. I2C/SPI basics
9. Task scheduling (RTOS)
10. Bit manipulation
```

---

### Interview Last-Minute Tips
```
✓ DON'T panic if stuck
✓ Think out loud
✓ Ask clarifying questions
✓ Start with simple solution
✓ Handle edge cases
✓ Check return values
✓ Use meaningful names
✓ Add comments
✓ Test your code mentally
✓ Optimize after it works
✓ Explain your approach
✓ Ask if interviewer has questions
```

---

### Emergency Cheat Sheet (If Stuck)
```
Can't remember syntax? Remember the PATTERN:

Data structure: What am I storing? How do I access it?
Algorithm: Brute force first, then optimize
Edge cases: NULL, overflow, underflow, empty
Memory: Stack (fast, limited) or Heap (slow, large)?
Timing: Real-time or best-effort?
```

---

**Print this page and review 1 hour before interview!**

Good luck! 🚀
