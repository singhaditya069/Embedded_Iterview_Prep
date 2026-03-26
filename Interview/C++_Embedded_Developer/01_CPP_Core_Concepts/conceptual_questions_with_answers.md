# C++ Core Concepts for Embedded - Comprehensive Q&A

## Templates and Generics

### Q1: Templates for embedded systems
**Answer:**
```cpp
#include <stdint.h>

// Generic circular buffer using templates
template <typename T, uint16_t SIZE>
class CircularBuffer {
private:
    T buffer[SIZE];
    uint16_t head = 0;
    uint16_t tail = 0;
    uint16_t count = 0;
    
public:
    // Push item
    bool push(const T& item) {
        if (count >= SIZE) return false;  // Buffer full
        
        buffer[tail] = item;
        tail = (tail + 1) % SIZE;
        count++;
        return true;
    }
    
    // Pop item
    bool pop(T& item) {
        if (count == 0) return false;  // Buffer empty
        
        item = buffer[head];
        head = (head + 1) % SIZE;
        count--;
        return true;
    }
    
    uint16_t available() const { return count; }
    uint16_t space() const { return SIZE - count; }
};

// Usage in embedded
int main() {
    // Circular buffer for UART Rx (100 bytes max)
    CircularBuffer<uint8_t, 100> uart_rx_buffer;
    
    // Circular buffer for sensor readings (50 readings)
    struct Sensor {
        uint16_t temperature;
        uint16_t humidity;
    };
    CircularBuffer<Sensor, 50> sensor_readings;
    
    // In UART ISR
    uint8_t data = UART0->RX;
    uart_rx_buffer.push(data);
    
    // In main loop
    uint8_t byte;
    while (uart_rx_buffer.pop(byte)) {
        process_uart_byte(byte);
    }
    
    return 0;
}

// Benefits over non-template version:
// 1. Type-safe (compiler checks element type)
// 2. Zero-overhead (compile-time generation)
// 3. Template parameters become compile constants
// 4. No runtime polymorphism penalty
// 5. Optimized by compiler for each instantiation
```

**Template specialization for optimization:**
```cpp
// Generic swap
template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Specialized swap for 32-bit values (faster with XOR)
template <>
void swap<uint32_t>(uint32_t& a, uint32_t& b) {
    a ^= b;
    b ^= a;
    a ^= b;  // XOR swap - no temporary needed
}

// Specialized swap for pointers (assembly optimized)
template <>
void swap<void*>(void*& a, void*& b) {
    __asm__ volatile (
        "xchg %0, %1"
        : "+r"(a), "+r"(b)
    );
}
```

---

## Smart Pointers and Memory Management

### Q1: Smart pointers for embedded (unique_ptr, shared_ptr)
**Answer:**
```cpp
#include <memory>
#include <cstdint>

// RAII pattern (Resource Acquisition Is Initialization)
class UARTDevice {
private:
    volatile uint32_t *uart_base;
    
public:
    UARTDevice(volatile uint32_t *base) : uart_base(base) {
        // Initialize UART
        uart_base[0] |= 0x01;  // Enable
        printf("UART initialized\n");
    }
    
    ~UARTDevice() {
        // Cleanup UART
        uart_base[0] &= ~0x01;  // Disable
        printf("UART cleaned up\n");
    }
    
    void send_byte(uint8_t data) {
        uart_base[1] = data;
    }
    
    uint8_t read_byte() {
        return (uint8_t)uart_base[2];
    }
};

// unique_ptr - exclusive ownership (no copy)
class MotorController {
private:
    std::unique_ptr<UARTDevice> comm;  // Owns communication device
    
public:
    MotorController(volatile uint32_t *uart_base) 
        : comm(std::make_unique<UARTDevice>(uart_base)) {}
    
    // Move constructor (transfer ownership)
    MotorController(MotorController&& other) noexcept 
        : comm(std::move(other.comm)) {}
    
    void send_command(uint8_t cmd) {
        if (comm) {
            comm->send_byte(cmd);
        }
    }
    
    // ~MotorController() - UART automatically destroyed
};

// Usage
int main() {
    {
        // unique_ptr - automatic cleanup on scope exit
        std::unique_ptr<MotorController> motor(
            new MotorController((volatile uint32_t*)0x40000000)
        );
        
        motor->send_command(0x01);  // Move motor forward
        
    }  // motor destroyed here, UART cleaned up
    
    return 0;
}

// Benefits over raw pointers:
// 1. Automatic deletion (no memory leaks)
// 2. Exception-safe (destroyed even on throw)
// 3. Move semantics (efficient ownership transfer)
// 4. Clear ownership semantics
// 5. Zero-overhead (no extra indirection)
```

**For embedded with fixed allocations:**
```cpp
// Pre-allocated buffer pool
class SensorDataBuffer {
private:
    static const uint16_t POOL_SIZE = 100;
    static const uint16_t BUFFER_SIZE = 256;
    
    uint8_t buffer_pool[POOL_SIZE][BUFFER_SIZE];
    bool in_use[POOL_SIZE];
    
public:
    SensorDataBuffer() {
        memset(in_use, 0, sizeof(in_use));
    }
    
    uint8_t* allocate_buffer() {
        for (uint16_t i = 0; i < POOL_SIZE; i++) {
            if (!in_use[i]) {
                in_use[i] = true;
                return buffer_pool[i];
            }
        }
        return nullptr;  // No buffers available
    }
    
    void free_buffer(uint8_t *buffer) {
        for (uint16_t i = 0; i < POOL_SIZE; i++) {
            if (buffer_pool[i] == buffer) {
                in_use[i] = false;
                return;
            }
        }
    }
};

// RAII wrapper for buffer allocation
class ManagedBuffer {
private:
    SensorDataBuffer *pool;
    uint8_t *buffer;
    
public:
    ManagedBuffer(SensorDataBuffer *p) : pool(p) {
        buffer = pool->allocate_buffer();
        if (!buffer) {
            throw std::runtime_error("No buffers available");
        }
    }
    
    ~ManagedBuffer() {
        if (buffer) pool->free_buffer(buffer);
    }
    
    uint8_t* get() const { return buffer; }
    
    // Prevent copying
    ManagedBuffer(const ManagedBuffer&) = delete;
    ManagedBuffer& operator=(const ManagedBuffer&) = delete;
};

// Usage
int main() {
    SensorDataBuffer buffer_pool;
    
    {
        ManagedBuffer buf(&buffer_pool);
        memset(buf.get(), 0, 256);
        
    }  // Buffer automatically returned to pool
    
    return 0;
}
```

---

## STL Containers for Embedded

### Q1: Using containers with compile-time sizing
**Answer:**
```cpp
#include <array>
#include <vector>
#include <cstring>

// Use std::array for fixed-size (no heap allocation)
class TemperatureSensor {
private:
    static const uint16_t READINGS_PER_SECOND = 10;
    static const uint16_t SAMPLE_SECONDS = 60;
    
    // Fixed-size - no heap allocation
    std::array<int16_t, READINGS_PER_SECOND * SAMPLE_SECONDS> readings;
    uint16_t current_index = 0;
    
public:
    void add_reading(int16_t temp) {
        if (current_index < readings.size()) {
            readings[current_index++] = temp;
        }
    }
    
    // Calculate average temperature
    int16_t calculate_average() {
        if (current_index == 0) return 0;
        
        int32_t sum = 0;
        for (const auto& reading : readings) {
            sum += reading;
        }
        return sum / current_index;
    }
    
    // Find max temperature
    int16_t find_max() {
        if (current_index == 0) return INT16_MIN;
        
        int16_t max_val = readings[0];
        for (uint16_t i = 1; i < current_index; i++) {
            if (readings[i] > max_val) {
                max_val = readings[i];
            }
        }
        return max_val;
    }
};

// Compile-time size calculation
int main() {
    TemperatureSensor sensor;
    
    // Stack allocation - 600 * 2 bytes = 1200 bytes (on stack!)
    // Guaranteed no malloc/new calls
    
    // Add readings
    for (int i = 0; i < 600; i++) {
        sensor.add_reading(25 + (i % 10));
    }
    
    printf("Average: %d C\n", sensor.calculate_average());
    printf("Max: %d C\n", sensor.find_max());
    
    return 0;
}

// When to use containers:
// std::array<T, N>  - Fixed size, no allocation (use for embedded!)
// std::vector<T>    - Dynamic size, heap allocation (avoid for bare metal)
// std::deque<T>     - Double-ended queue (complex, avoid)
// std::list<T>      - Linked list (use only for variable size needed)

// Memory footprint:
// std::array<int, 100>  = 400 bytes (stack)
// std::vector<int>(100) = 400 bytes data + 24 bytes overhead (heap)
```

---

## Modern C++ Features (C++11/14/17)

### Q1: auto, range-based for, lambda functions
**Answer:**
```cpp
#include <array>
#include <cstdint>

// auto - type deduction (reduces verbosity)
class SensorArray {
private:
    std::array<uint16_t, 16> sensors;
    
public:
    // Without auto - verbose
    void process_data_old() {
        std::array<uint16_t, 16>::iterator it = sensors.begin();
        while (it != sensors.end()) {
            *it = (*it >> 2);  // Right shift by 2
            ++it;
        }
    }
    
    // With auto - concise
    void process_data_new() {
        for (auto& sensor : sensors) {
            sensor >>= 2;  // Right shift by 2
        }
    }
    
    // Range-based for (iterator hidden by auto)
    uint32_t calculate_sum() {
        uint32_t sum = 0;
        for (auto sensor : sensors) {
            sum += sensor;
        }
        return sum;
    }
    
    // Lambda function - inline function object
    void apply_threshold(uint16_t threshold) {
        std::for_each(sensors.begin(), sensors.end(),
            [threshold](uint16_t& val) {  // Capture threshold
                if (val < threshold) {
                    val = 0;  // Threshold filter
                }
            }
        );
    }
};

// Lambda for callbacks (replaces function pointers)
class ButtonHandler {
private:
    void (*button_callback)(void);  // Old way - function pointer
    
public:
    // New way - store lambda
    void set_callback_lambda() {
        static volatile bool button_pressed = false;
        
        // Create lambda
        auto on_button_press = [&]() {
            button_pressed = true;
            printf("Button pressed!\n");
        };
        
        // Set up interrupt (pseudo-code)
        register_interrupt_handler(
            [](void) {  // Lambda as ISR
                // Process button
            }
        );
    }
};

// constexpr - compile-time constants
class TimerConfig {
public:
    // Compile-time calculation
    static constexpr uint32_t CLOCK_FREQ = 168000000;
    static constexpr uint32_t BAUD_RATE = 115200;
    static constexpr uint32_t BAUD_DIVISOR = CLOCK_FREQ / (16 * BAUD_RATE);
    
    // Compile-time function
    static constexpr uint16_t calculate_delay_ticks(uint32_t ms) {
        return (ms * CLOCK_FREQ) / 1000000;
    }
};

// All calculations done at compile-time - no runtime overhead
int main() {
    static_assert(TimerConfig::BAUD_DIVISOR == 91,
                 "UART baud rate setup incorrect");
    
    constexpr auto delay = TimerConfig::calculate_delay_ticks(10);  // 10ms
    
    return 0;
}
```

---

## Real-World Embedded C++ Example

```cpp
// Sensor management system with templates, RAII, and modern C++

#include <memory>
#include <array>
#include <cstdint>

// Abstract sensor interface
class ISensor {
public:
    virtual uint16_t read_value() = 0;
    virtual void initialize() = 0;
    virtual ~ISensor() = default;
};

// Temperature sensor implementation
class TemperatureSensor : public ISensor {
private:
    volatile uint32_t *adc_base;
    
public:
    TemperatureSensor(volatile uint32_t *base) : adc_base(base) {}
    
    void initialize() override {
        adc_base[0] |= 0x01;  // Enable ADC
    }
    
    uint16_t read_value() override {
        return (uint16_t)adc_base[1];  // Read ADC value
    }
};

// Pressure sensor
class PressureSensor : public ISensor {
private:
    volatile uint32_t *i2c_base;
    
public:
    PressureSensor(volatile uint32_t *base) : i2c_base(base) {}
    
    void initialize() override {
        i2c_base[0] |= 0x01;  // Enable I2C
    }
    
    uint16_t read_value() override {
        return (uint16_t)i2c_base[1];  // Read I2C register
    }
};

// Sensor manager - generic sensor handler
class SensorManager {
private:
    std::array<std::unique_ptr<ISensor>, 8> sensors;
    uint8_t sensor_count = 0;
    
public:
    template <typename SensorType, typename... Args>
    void register_sensor(Args&&... args) {
        if (sensor_count < sensors.size()) {
            auto sensor = std::make_unique<SensorType>(
                std::forward<Args>(args)...
            );
            sensor->initialize();
            sensors[sensor_count++] = std::move(sensor);
        }
    }
    
    void read_all() {
        for (uint8_t i = 0; i < sensor_count; i++) {
            if (sensors[i]) {
                uint16_t value = sensors[i]->read_value();
                printf("Sensor %d: %d\n", i, value);
            }
        }
    }
};

// Usage
int main() {
    SensorManager manager;
    
    // Register sensors with automatic initialization
    manager.register_sensor<TemperatureSensor>(
        (volatile uint32_t*)0x40010000
    );
    manager.register_sensor<PressureSensor>(
        (volatile uint32_t*)0x40020000
    );
    
    // Read all sensors
    manager.read_all();
    
    // Sensors automatically cleaned up on scope exit
    return 0;
}

// Key C++ advantages in embedded:
// 1. Type safety (compiler catches errors)
// 2. Memory safety (RAII patterns)
// 3. Zero-overhead abstractions (templates, inline)
// 4. Modern features (auto, lambda, constexpr)
// 5. Better code organization (classes, inheritance)
// 6. No runtime penalty vs C

// Embedded-specific considerations:
// ✓ Use constexpr for compile-time constants
// ✓ Use std::array for fixed-size buffers
// ✓ Use unique_ptr for single ownership
// ✓ Use templates for generic code
// ✗ Avoid std::vector (dynamic allocation)
// ✗ Avoid exceptions (use error codes instead)
// ✗ Avoid RTTI (type checking at runtime)
```

This C++ guide enables modern, safe embedded development!
