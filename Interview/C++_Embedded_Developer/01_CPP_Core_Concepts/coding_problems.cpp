// C++ Embedded Developer - C++ Core Concepts Coding Problems

/**
 * Problem 1: Implement a RAII-based GPIO Wrapper for STM32
 * 
 * Requirements:
 * - Create a GPIO class that initializes a GPIO pin in the constructor
 * - Automatically deinitialize in the destructor
 * - Prevent copying but allow moving
 * - Provide methods to read/write digital values
 * 
 * Challenge: Ensure no resource leaks and optimal performance
 */

#include <cstdint>

// Simulated HAL functions
extern void GPIO_Init(uint32_t pin);
extern void GPIO_DeInit(uint32_t pin);
extern void GPIO_WritePin(uint32_t pin, uint8_t value);
extern uint8_t GPIO_ReadPin(uint32_t pin);

class GPIO {
private:
    uint32_t pin_;
    
public:
    // Constructor - acquire resource
    explicit GPIO(uint32_t pin) : pin_(pin) {
        GPIO_Init(pin);
    }
    
    // Destructor - release resource
    ~GPIO() {
        GPIO_DeInit(pin_);
    }
    
    // Delete copy constructor and assignment
    GPIO(const GPIO&) = delete;
    GPIO& operator=(const GPIO&) = delete;
    
    // Allow move semantics
    GPIO(GPIO&& other) noexcept : pin_(other.pin_) {
        other.pin_ = 0xFFFFFFFF; // Invalid pin
    }
    
    GPIO& operator=(GPIO&& other) noexcept {
        if (this != &other) {
            GPIO_DeInit(pin_);
            pin_ = other.pin_;
            other.pin_ = 0xFFFFFFFF;
        }
        return *this;
    }
    
    void write(uint8_t value) const {
        GPIO_WritePin(pin_, value);
    }
    
    uint8_t read() const {
        return GPIO_ReadPin(pin_);
    }
};

/**
 * Problem 2: Compile-time Registry using Variadic Templates
 * 
 * Design a registry that stores sensor types at compile-time
 * allowing O(1) access without runtime overhead.
 */

template<typename... Sensors>
class SensorRegistry {
public:
    static constexpr size_t count() {
        return sizeof...(Sensors);
    }
    
    // Access sensor at compile-time
    template<size_t Index>
    struct get {
        // Implementation using template specialization
    };
};

// Usage
class TemperatureSensor {};
class PressureSensor {};
class HumiditySensor {};

using MyRegistry = SensorRegistry<TemperatureSensor, PressureSensor, HumiditySensor>;

// At compile-time: MyRegistry::count() = 3

/**
 * Problem 3: Efficient Bit Field Manager using Template Specialization
 * 
 * Create a template class to manage bit fields efficiently.
 * Optimize for both 8-bit, 16-bit, and 32-bit values.
 */

template<typename T, size_t BitStart, size_t BitCount>
class BitField {
    static_assert(BitCount > 0 && BitCount <= sizeof(T) * 8);
    static_assert(BitStart + BitCount <= sizeof(T) * 8);
    
private:
    T value_;
    static constexpr T MASK = ((1ULL << BitCount) - 1) << BitStart;
    
public:
    BitField() : value_(0) {}
    explicit BitField(T val) : value_(val) {}
    
    T read() const {
        return (value_ & MASK) >> BitStart;
    }
    
    void write(T val) {
        value_ = (value_ & ~MASK) | ((val << BitStart) & MASK);
    }
    
    // Conversion operator for convenience
    operator T() const { return read(); }
    BitField& operator=(T val) { write(val); return *this; }
};

// Usage
//#pragma pack(1)
//struct StatusRegister {
//    BitField<uint32_t, 0, 8> mode;
//    BitField<uint32_t, 8, 4> status;
//    BitField<uint32_t, 12, 1> ready;
//};

/**
 * Problem 4: CRTP (Curiously Recurring Template Pattern) for Static Polymorphism
 * 
 * Implement a device driver interface without virtual functions (no vtable overhead).
 */

template<typename Derived>
class PeripheralDriver {
public:
    void init() {
        static_cast<Derived*>(this)->init_impl();
    }
    
    void deinit() {
        static_cast<Derived*>(this)->deinit_impl();
    }
    
    void start() {
        static_cast<Derived*>(this)->start_impl();
    }
};

class UART : public PeripheralDriver<UART> {
public:
    void init_impl() {
        // UART initialization
    }
    
    void deinit_impl() {
        // UART deinitialization
    }
    
    void start_impl() {
        // Start UART communication
    }
};

class SPI : public PeripheralDriver<SPI> {
public:
    void init_impl() {
        // SPI initialization
    }
    
    void deinit_impl() {
        // SPI deinitialization
    }
    
    void start_impl() {
        // Start SPI communication
    }
};

/**
 * Problem 5: Zero-overhead Abstraction - constexpr Function Evaluator
 * 
 * Create a compile-time configuration system with zero runtime overhead.
 */

template<int BUFFER_SIZE, int MAX_CHANNELS>
struct SystemConfig {
    static constexpr size_t BUFFER_SIZE_VALUE = BUFFER_SIZE;
    static constexpr size_t MAX_CHANNELS_VALUE = MAX_CHANNELS;
    static constexpr size_t TOTAL_MEMORY = BUFFER_SIZE * MAX_CHANNELS;
    
    // Validate at compile-time
    static_assert(BUFFER_SIZE > 0, "Buffer size must be positive");
    static_assert(MAX_CHANNELS > 0, "Channel count must be positive");
    static_assert(TOTAL_MEMORY <= 65536, "Total memory exceeds limit");
};

// Create configurations at compile-time
using HighSpeedConfig = SystemConfig<512, 16>;
using LowPowerConfig = SystemConfig<128, 4>;

/**
 * Problem 6: Move Semantics for DMA Buffer Management
 * 
 * Design a efficient buffer class that uses move semantics
 * to avoid copying large data blocks.
 */

class DMABuffer {
private:
    uint8_t* data_;
    size_t size_;
    
public:
    DMABuffer() : data_(nullptr), size_(0) {}
    
    explicit DMABuffer(size_t size) : data_(new uint8_t[size]), size_(size) {}
    
    // Copy operations - deleted (expensive for large buffers)
    DMABuffer(const DMABuffer&) = delete;
    DMABuffer& operator=(const DMABuffer&) = delete;
    
    // Move operations - efficient
    DMABuffer(DMABuffer&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    DMABuffer& operator=(DMABuffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    ~DMABuffer() {
        delete[] data_;
    }
    
    uint8_t* get() { return data_; }
    const uint8_t* get() const { return data_; }
    size_t size() const { return size_; }
};

/**
 * Problem 7: Observer Pattern with Minimal Overhead
 * 
 * Implement an event system used in interrupt handlers.
 */

template<typename EventType>
class IEventListener {
public:
    virtual ~IEventListener() = default;
    virtual void on_event(const EventType& event) = 0;
};

template<typename EventType>
class EventManager {
private:
    // Use intrusive list or fixed array for embedded systems
    static constexpr size_t MAX_LISTENERS = 10;
    IEventListener<EventType>* listeners_[MAX_LISTENERS];
    size_t listener_count_;
    
public:
    EventManager() : listener_count_(0) {}
    
    void subscribe(IEventListener<EventType>* listener) {
        if (listener_count_ < MAX_LISTENERS) {
            listeners_[listener_count_++] = listener;
        }
    }
    
    void emit(const EventType& event) {
        for (size_t i = 0; i < listener_count_; ++i) {
            listeners_[i]->on_event(event);
        }
    }
};

// Example event
struct ButtonClickEvent {
    uint32_t timestamp;
    uint8_t pin;
};

