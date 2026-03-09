# Bare Metal Coding to STM32 Cube PIN Initialization

## Introduction
This guide provides a comprehensive step-by-step approach to STM32 development, starting from bare metal programming and progressing to STM32CubeIDE/STM32CubeMX integration. It covers pin initialization, peripheral configuration, and best practices for embedded development.

## Bare Metal Programming Fundamentals

### What is Bare Metal Programming?
Bare metal programming involves direct hardware manipulation without an operating system. In STM32, this means:
- Direct register access
- Manual peripheral initialization
- No HAL (Hardware Abstraction Layer) libraries
- Full control over hardware resources

### Advantages and Disadvantages
**Advantages:**
- Maximum performance
- Minimal code size
- Full hardware control
- No library dependencies

**Disadvantages:**
- Complex initialization code
- Error-prone register manipulation
- Time-consuming development
- Limited portability

## STM32 Memory-Mapped Registers

### Register Access
STM32 peripherals are controlled through memory-mapped registers. Key points:
- Registers are 32-bit wide
- Accessed using pointers to specific addresses
- Volatile keyword prevents compiler optimization
- Bit manipulation for configuration

### Common Register Types
- **Control Registers**: Configure peripheral behavior
- **Status Registers**: Indicate peripheral state
- **Data Registers**: Hold input/output data
- **Configuration Registers**: Set operating parameters

## Step-by-Step Bare Metal GPIO Initialization

### Step 1: Include Header Files
```c
#include <stdint.h>

// Define register addresses (example for STM32F4)
#define RCC_BASE        0x40023800
#define GPIOA_BASE      0x40020000

#define RCC_AHB1ENR     (*(volatile uint32_t*)(RCC_BASE + 0x30))
#define GPIOA_MODER     (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER    (*(volatile uint32_t*)(GPIOA_BASE + 0x04))
#define GPIOA_OSPEEDR   (*(volatile uint32_t*)(GPIOA_BASE + 0x08))
#define GPIOA_PUPDR     (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))
#define GPIOA_ODR       (*(volatile uint32_t*)(GPIOA_BASE + 0x14))
```

### Step 2: Enable Peripheral Clock
```c
void enable_gpioa_clock(void) {
    // Enable GPIOA clock (bit 0 in AHB1ENR)
    RCC_AHB1ENR |= (1 << 0);
}
```

### Step 3: Configure Pin Mode
```c
void configure_pin_mode(void) {
    // Clear mode bits for PA5 (bits 10-11)
    GPIOA_MODER &= ~(3 << 10);
    // Set PA5 as output (01)
    GPIOA_MODER |= (1 << 10);
}
```

### Step 4: Configure Output Type
```c
void configure_output_type(void) {
    // Clear bit 5 for push-pull output
    GPIOA_OTYPER &= ~(1 << 5);
}
```

### Step 5: Configure Output Speed
```c
void configure_output_speed(void) {
    // Clear speed bits for PA5 (bits 10-11)
    GPIOA_OSPEEDR &= ~(3 << 10);
    // Set high speed (10)
    GPIOA_OSPEEDR |= (2 << 10);
}
```

### Step 6: Configure Pull-up/Pull-down
```c
void configure_pull_resistor(void) {
    // Clear pull bits for PA5 (bits 10-11)
    GPIOA_PUPDR &= ~(3 << 10);
    // Set no pull (00)
    GPIOA_PUPDR |= (0 << 10);
}
```

### Step 7: Complete Initialization Function
```c
void gpio_init(void) {
    enable_gpioa_clock();
    configure_pin_mode();
    configure_output_type();
    configure_output_speed();
    configure_pull_resistor();
}
```

### Step 8: Using the Configured Pin
```c
void led_toggle(void) {
    // Toggle PA5
    GPIOA_ODR ^= (1 << 5);
}

int main(void) {
    gpio_init();

    while(1) {
        led_toggle();
        // Simple delay
        for(volatile int i = 0; i < 100000; i++);
    }
}
```

## Advanced Bare Metal Techniques

### Bit Manipulation Macros
```c
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (BIT))

// Usage
SET_BIT(GPIOA_MODER, GPIO_MODER_MODER5_0);
CLEAR_BIT(GPIOA_OTYPER, GPIO_OTYPER_OT5);
```

### Structure-Based Register Access
```c
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)

// Usage
GPIOA->MODER |= GPIO_MODER_MODER5_0;
GPIOA->ODR ^= GPIO_ODR_ODR5;
```

## Transition to STM32CubeMX

### What is STM32CubeMX?
STM32CubeMX is a graphical tool for:
- MCU and MPU selection
- Pin configuration
- Clock configuration
- Peripheral configuration
- Code generation

### Step-by-Step STM32CubeMX Setup

#### Step 1: Project Creation
1. Open STM32CubeMX
2. Click "New Project"
3. Select your STM32 microcontroller (e.g., STM32F407VG)
4. Choose board or MCU directly

#### Step 2: Pin Configuration
1. In Pinout view, click on desired pins
2. Select function from dropdown:
   - GPIO_Output for LED
   - GPIO_Input for button
   - GPIO_Analog for ADC
   - Peripheral functions (USART_TX, SPI_SCK, etc.)

#### Step 3: Peripheral Configuration
1. Go to "Configuration" tab
2. Click on peripheral (e.g., GPIO)
3. Configure parameters:
   - Pin: PA5
   - GPIO mode: Output Push Pull
   - GPIO Pull-up/Pull-down: No pull-up and no pull-down
   - Maximum output speed: High
   - User Label: LED

#### Step 4: Clock Configuration
1. Go to "Clock Configuration" tab
2. Set system clock source (HSI/HSE)
3. Configure PLL settings
4. Set AHB/APB prescalers

#### Step 5: Project Settings
1. Go to "Project Manager" tab
2. Set project name and location
3. Select Toolchain/IDE (STM32CubeIDE)
4. Choose firmware package
5. Set code generator options

#### Step 6: Code Generation
1. Click "Generate Code"
2. Open project in STM32CubeIDE
3. Add user code in designated sections

## STM32CubeIDE Integration

### Generated Code Structure
```
Project/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── stm32f4xx_hal_conf.h
│   │   └── stm32f4xx_it.h
│   └── Src/
│       ├── main.c
│       ├── stm32f4xx_hal_msp.c
│       ├── stm32f4xx_it.c
│       └── system_stm32f4xx.c
├── Drivers/
│   └── STM32F4xx_HAL_Driver/
└── STM32F407VG_FLASH.ld
```

### User Code Sections
STM32CubeMX generates code with user code sections:
```c
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

int main(void) {
    /* USER CODE BEGIN 1 */
    /* USER CODE END 1 */

    HAL_Init();
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    /* USER CODE END SysInit */

    MX_GPIO_Init();

    /* USER CODE BEGIN 2 */
    // Add user initialization code here
    /* USER CODE END 2 */

    while (1) {
        /* USER CODE BEGIN 3 */
        // Add main loop code here
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(500);
        /* USER CODE END 3 */
    }
}
```

### GPIO Initialization Function
```c
void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    /* Configure GPIO pin : PA5 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
```

## HAL vs LL vs Bare Metal

### HAL (Hardware Abstraction Layer)
- High-level API
- Easy to use
- Larger code size
- Slower execution
- Cross-family compatibility

### LL (Low Layer)
- Mid-level API
- Direct register access with helper functions
- Smaller code size than HAL
- Faster than HAL
- Less portable

### Bare Metal
- Direct register manipulation
- Maximum performance
- Minimum code size
- Most complex
- Least portable

## Advanced Pin Initialization Techniques

### Interrupt Configuration
```c
void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // LED output
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Button input with interrupt
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Enable interrupt
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
```

### Alternate Function Configuration
```c
void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    // USART2 TX
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // USART2 RX
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
```

## Debugging and Troubleshooting

### Common Issues
1. **Pin not working**: Check clock enable, mode configuration
2. **Wrong alternate function**: Verify AF number in datasheet
3. **Interrupt not firing**: Check NVIC configuration, priority
4. **Timing issues**: Verify clock configuration

### Debugging Tools
- **Logic Analyzer**: For signal timing analysis
- **Oscilloscope**: For voltage level verification
- **STM32CubeMonitor**: Real-time variable monitoring
- **SWD/JTAG Debugger**: Step-by-step code execution

## Best Practices

### Code Organization
- Separate initialization from application logic
- Use meaningful names for pins and peripherals
- Document register manipulations
- Group related configurations

### Performance Optimization
- Use LL drivers for time-critical code
- Minimize HAL function calls in interrupts
- Use DMA for high-throughput operations
- Optimize clock configurations

### Safety and Reliability
- Implement watchdog timers
- Add error checking for peripheral operations
- Use volatile for shared variables
- Implement proper reset handling

### Portability
- Use HAL for cross-STM32 compatibility
- Abstract hardware-specific code
- Document platform dependencies
- Consider future hardware changes

## Migration Path

### From Bare Metal to HAL
1. Identify register manipulations
2. Find equivalent HAL functions
3. Replace with HAL calls
4. Add error handling
5. Test functionality

### From HAL to LL
1. Replace HAL functions with LL equivalents
2. Remove HAL initialization overhead
3. Optimize for performance
4. Maintain compatibility

This comprehensive guide provides a solid foundation for STM32 development, from low-level hardware control to high-level abstraction layers, enabling developers to choose the appropriate approach for their specific requirements.