# Device Driver Development - Comprehensive Q&A

## Device Driver Architecture

### Q1: Peripheral driver structure and GPIO driver implementation
**Answer:**
```c
#include <stdint.h>
#include <stdbool.h>

// ---- GPIO Driver Implementation ----

// GPIO handle (abstract representation)
typedef struct {
    volatile uint32_t *gpio_port_base;  // GPIO port base address
    uint8_t pin_number;                   // 0-15
} GPIO_Handle_t;

// GPIO modes
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_ALTERNATE = 2,
    GPIO_MODE_ANALOG = 3,
} GPIO_Mode_t;

// GPIO output types
typedef enum {
    GPIO_OUTPUT_PUSH_PULL = 0,
    GPIO_OUTPUT_OPEN_DRAIN = 1,
} GPIO_OutputType_t;

// GPIO speed
typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MEDIUM = 1,
    GPIO_SPEED_HIGH = 2,
    GPIO_SPEED_VERY_HIGH = 3,
} GPIO_Speed_t;

// GPIO pull configuration
typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP = 1,
    GPIO_PULL_DOWN = 2,
} GPIO_Pull_t;

// STM32 GPIO register structure
typedef struct {
    volatile uint32_t MODER;    // Mode register
    volatile uint32_t OTYPER;   // Output type register
    volatile uint32_t OSPEEDR;  // Output speed register
    volatile uint32_t PUPDR;    // Pull-up/pull-down register
    volatile uint32_t IDR;      // Input data register
    volatile uint32_t ODR;      // Output data register
    volatile uint32_t BSRR;     // Bit set/reset register
    volatile uint32_t LCKR;     // Port configuration lock
    volatile uint32_t AFRL;     // Alternate function register (low)
    volatile uint32_t AFRH;     // Alternate function register (high)
} GPIO_TypeDef;

#define GPIOA_BASE 0x40020000
#define GPIOB_BASE 0x40020400
#define GPIOC_BASE 0x40020800

// GPIO initialization
void gpio_init(GPIO_Handle_t *handle, GPIO_Mode_t mode, GPIO_OutputType_t otype,
              GPIO_Speed_t speed, GPIO_Pull_t pull) {
    GPIO_TypeDef *port = (GPIO_TypeDef *)handle->gpio_port_base;
    uint8_t pin = handle->pin_number;
    
    // 1. Set mode (bits: 2*pin and 2*pin+1)
    uint32_t moder = port->MODER;
    moder &= ~(0x3 << (2 * pin));      // Clear
    moder |= (mode << (2 * pin));      // Set
    port->MODER = moder;
    
    // 2. Set output type (bit: pin)
    if (mode == GPIO_MODE_OUTPUT) {
        uint32_t otyper = port->OTYPER;
        otyper &= ~(1 << pin);         // Clear
        otyper |= (otype << pin);      // Set
        port->OTYPER = otyper;
    }
    
    // 3. Set speed (bits: 2*pin and 2*pin+1)
    uint32_t ospeedr = port->OSPEEDR;
    ospeedr &= ~(0x3 << (2 * pin));    // Clear
    ospeedr |= (speed << (2 * pin));   // Set
    port->OSPEEDR = ospeedr;
    
    // 4. Set pull-up/pull-down (bits: 2*pin and 2*pin+1)
    uint32_t pupdr = port->PUPDR;
    pupdr &= ~(0x3 << (2 * pin));      // Clear
    pupdr |= (pull << (2 * pin));      // Set
    port->PUPDR = pupdr;
}

// GPIO write (output high or low)
void gpio_write(GPIO_Handle_t *handle, uint8_t value) {
    GPIO_TypeDef *port = (GPIO_TypeDef *)handle->gpio_port_base;
    uint8_t pin = handle->pin_number;
    
    if (value) {
        // Set pin (atomic write to BSRR)
        port->BSRR = (1 << pin);
    } else {
        // Reset pin (atomic write to upper BSRR)
        port->BSRR = (1 << (pin + 16));
    }
    
    // BSRR is atomic - no need for disable interrupts
}

// GPIO read (input value)
uint8_t gpio_read(GPIO_Handle_t *handle) {
    GPIO_TypeDef *port = (GPIO_TypeDef *)handle->gpio_port_base;
    uint8_t pin = handle->pin_number;
    
    return (uint8_t)((port->IDR >> pin) & 1);
}

// GPIO toggle
void gpio_toggle(GPIO_Handle_t *handle) {
    GPIO_TypeDef *port = (GPIO_TypeDef *)handle->gpio_port_base;
    uint8_t pin = handle->pin_number;
    
    port->ODR ^= (1 << pin);  // XOR toggle
}

// ---- Higher-level abstractions ----

// LED driver (uses GPIO)
typedef struct {
    GPIO_Handle_t gpio_handle;
    bool active_high;  // true if LED on = high, false if active low
} LED_t;

void led_init(LED_t *led, volatile uint32_t *gpio_base, uint8_t pin,
             bool active_high) {
    led->gpio_handle.gpio_port_base = gpio_base;
    led->gpio_handle.pin_number = pin;
    led->active_high = active_high;
    
    // Configure as output
    GPIO_Mode_t mode = GPIO_MODE_OUTPUT;
    GPIO_OutputType_t otype = GPIO_OUTPUT_PUSH_PULL;
    GPIO_Speed_t speed = GPIO_SPEED_MEDIUM;
    GPIO_Pull_t pull = GPIO_PULL_NONE;
    
    gpio_init(&led->gpio_handle, mode, otype, speed, pull);
}

void led_on(LED_t *led) {
    uint8_t value = led->active_high ? 1 : 0;
    gpio_write(&led->gpio_handle, value);
}

void led_off(LED_t *led) {
    uint8_t value = led->active_high ? 0 : 1;
    gpio_write(&led->gpio_handle, value);
}

void led_toggle(LED_t *led) {
    gpio_toggle(&led->gpio_handle);
}

// Button driver (with debouncing)
typedef struct {
    GPIO_Handle_t gpio_handle;
    bool active_low;
    uint32_t last_press_time;
    uint32_t debounce_ms;
} Button_t;

void button_init(Button_t *btn, volatile uint32_t *gpio_base, uint8_t pin,
                bool active_low, uint32_t debounce_ms) {
    btn->gpio_handle.gpio_port_base = gpio_base;
    btn->gpio_handle.pin_number = pin;
    btn->active_low = active_low;
    btn->debounce_ms = debounce_ms;
    btn->last_press_time = 0;
    
    // Configure as input with pull-up/pull-down
    GPIO_Mode_t mode = GPIO_MODE_INPUT;
    GPIO_Speed_t speed = GPIO_SPEED_MEDIUM;
    GPIO_Pull_t pull = active_low ? GPIO_PULL_UP : GPIO_PULL_DOWN;
    
    gpio_init(&btn->gpio_handle, mode, GPIO_OUTPUT_PUSH_PULL, speed, pull);
}

bool button_is_pressed(Button_t *btn) {
    uint8_t pin_value = gpio_read(&btn->gpio_handle);
    
    if (btn->active_low) {
        return pin_value == 0;
    } else {
        return pin_value == 1;
    }
}

bool button_pressed_debounced(Button_t *btn) {
    static uint32_t current_time;
    current_time = get_system_ticks();
    
    if (button_is_pressed(btn)) {
        if ((current_time - btn->last_press_time) > btn->debounce_ms) {
            btn->last_press_time = current_time;
            return true;
        }
    }
    
    return false;
}

// ---- Usage Example ----

int main(void) {
    // Initialize LED on PA5
    LED_t led;
    led_init(&led, (volatile uint32_t *)GPIOA_BASE, 5, true);
    
    // Initialize button on PA0 (active low with debounce)
    Button_t button;
    button_init(&button, (volatile uint32_t *)GPIOA_BASE, 0, true, 200);
    
    while (1) {
        // Check button (with debouncing)
        if (button_pressed_debounced(&button)) {
            led_toggle(&led);  // Toggle LED on button press
        }
    }
    
    return 0;
}

// Driver design principles:
// 1. Abstraction - Hide hardware complexity
// 2. Modularity - Each driver self-contained
// 3. Reusability - Works across projects
// 4. Testability - Can mock GPIO layer
// 5. Efficiency - No unnecessary overhead
```

---

### Q2: UART driver implementation
**Answer:**
```c
#include <stdint.h>
#include <string.h>

// UART configuration
typedef struct {
    uint32_t baud_rate;      // 1200 to 115200
    uint8_t data_bits;        // 8 or 9
    uint8_t stop_bits;        // 1 or 2
    enum {
        UART_PARITY_NONE = 0,
        UART_PARITY_EVEN = 2,
        UART_PARITY_ODD = 1,
    } parity;
    enum {
        UART_FLOW_CONTROL_NONE = 0,
        UART_FLOW_CONTROL_RTS_CTS = 3,
        UART_FLOW_CONTROL_XON_XOFF = 1,
    } flow_control;
} UART_Config_t;

// UART driver handle
typedef struct {
    volatile uint32_t *uart_base;
    UART_Config_t config;
    uint8_t tx_buffer[256];
    uint8_t rx_buffer[256];
    uint16_t tx_head, tx_tail;
    uint16_t rx_head, rx_tail;
} UART_Handle_t;

// STM32 UART register structure
typedef struct {
    volatile uint32_t SR;    // Status register
    volatile uint32_t DR;    // Data register
    volatile uint32_t BRR;   // Baud rate register
    volatile uint32_t CR1;   // Control register 1
    volatile uint32_t CR2;   // Control register 2
    volatile uint32_t CR3;   // Control register 3
    volatile uint32_t GTPR;  // Guard time and prescaler
} UART_TypeDef;

// Initialize UART
void uart_init(UART_Handle_t *huart, volatile uint32_t *uart_base,
              UART_Config_t *config) {
    huart->uart_base = uart_base;
    memcpy(&huart->config, config, sizeof(UART_Config_t));
    huart->tx_head = huart->tx_tail = 0;
    huart->rx_head = huart->rx_tail = 0;
    
    UART_TypeDef *uart = (UART_TypeDef *)uart_base;
    
    // 1. Disable UART
    uart->CR1 &= ~0x01;  // Clear UE (UART enable)
    
    // 2. Set baud rate
    // BRR = SystemClock / (16 * Baud)
    // For STM32F4 @84MHz APB1: BRR = 84000000 / (16 * 115200) ≈ 45.57
    uint32_t brr = (84000000 / (16 * config->baud_rate));
    uart->BRR = brr;
    
    // 3. Configure control registers
    uart->CR1 |= 0x300C;    // TE (transmit enable) + RE (receive enable) + UE
    uart->CR1 |= 0x0040;    // RXNEIE (RX interrupt)
    uart->CR2 |= 0x0000;    // 1 stop bit
    
    // 4. Enable UART
    uart->CR1 |= 0x01;  // Set UE
    
    // 5. Enable UART interrupt in NVIC
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 5);
}

// Send single byte
void uart_send_byte(UART_Handle_t *huart, uint8_t byte) {
    UART_TypeDef *uart = (UART_TypeDef *)huart->uart_base;
    
    // Wait for TX ready (busy polling - blocking)
    while (!(uart->SR & 0x0080));  // TXE (transmit empty)
    
    uart->DR = byte;  // Send byte
}

// Send string
void uart_send_string(UART_Handle_t *huart, const char *str) {
    while (*str) {
        uart_send_byte(huart, (uint8_t)*str);
        str++;
    }
}

// Receive byte (non-blocking with timeout)
bool uart_receive_byte(UART_Handle_t *huart, uint8_t *byte,
                       uint32_t timeout_ms) {
    uint32_t start = get_ticks();
    
    while (1) {
        UART_TypeDef *uart = (UART_TypeDef *)huart->uart_base;
        
        if (uart->SR & 0x0020) {  // RXNE (receive not empty)
            *byte = (uint8_t)uart->DR;
            return true;
        }
        
        if ((get_ticks() - start) > timeout_ms) {
            return false;  // Timeout
        }
    }
}

// Interrupt handler (called on RX data)
void USART2_IRQHandler(void) {
    UART_TypeDef *uart = (UART_TypeDef *)USART2_BASE;
    
    if (uart->SR & 0x0020) {  // RXNE
        uint8_t data = (uint8_t)uart->DR;
        
        // Add to circular RX buffer
        uint16_t next_tail = (huart.rx_tail + 1) % 256;
        if (next_tail != huart.rx_head) {  // Buffer not full
            huart.rx_buffer[huart.rx_tail] = data;
            huart.rx_tail = next_tail;
        }
    }
}

// Get received data (non-blocking)
bool uart_get_byte(UART_Handle_t *huart, uint8_t *byte) {
    if (huart->rx_head == huart->rx_tail) {
        return false;  // No data available
    }
    
    *byte = huart->rx_buffer[huart->rx_head];
    huart->rx_head = (huart->rx_head + 1) % 256;
    
    return true;
}

// Printf-like debugging output
void uart_printf(UART_Handle_t *huart, const char *format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    uart_send_string(huart, buffer);
}

// Usage
int main(void) {
    // Configure UART
    UART_Config_t config = {
        .baud_rate = 115200,
        .data_bits = 8,
        .stop_bits = 1,
        .parity = UART_PARITY_NONE,
        .flow_control = UART_FLOW_CONTROL_NONE,
    };
    
    UART_Handle_t uart;
    uart_init(&uart, (volatile uint32_t *)0x40004400, &config);
    
    uart_send_string(&uart, "Hello World!\n");
    
    uint8_t byte;
    if (uart_get_byte(&uart, &byte)) {
        uart_printf(&uart, "Received: 0x%02X\n", byte);
    }
    
    return 0;
}

// UART driver features:
// ✓ Interrupt-driven Rx (non-blocking)
// ✓ Circular buffers for efficiency
// ✓ Configurable baud rate
// ✓ Error handling (timeout, buffer full)
// ✓ Printf-like debugging support
```

Device driver development is fundamental to embedded systems!
