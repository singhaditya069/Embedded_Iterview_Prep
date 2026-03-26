# Object-Oriented Programming (C++) - Comprehensive Q&A

## Class and Object Basics

### Q1: Classes, objects, constructors, and destructors
**Answer:**
```cpp
#include <iostream>
#include <string>

class Motor {
private:
    int rpm;
    bool is_running;
    std::string name;
    
public:
    // Constructor - initializes object
    Motor(std::string motor_name, int initial_rpm = 0) 
        : rpm(initial_rpm), is_running(false), name(motor_name) {
        std::cout << name << " constructed\n";
    }
    
    // Copy constructor
    Motor(const Motor& other) 
        : rpm(other.rpm), is_running(other.is_running), name(other.name) {
        std::cout << "Copy constructor called for " << name << "\n";
    }
    
    // Destructor - cleanup
    ~Motor() {
        std::cout << name << " destructed\n";
        if (is_running) {
            stop();  // Cleanup state
        }
    }
    
    // Member function
    void start() {
        is_running = true;
        std::cout << name << " started at " << rpm << " RPM\n";
    }
    
    void stop() {
        is_running = false;
        std::cout << name << " stopped\n";
    }
    
    void set_rpm(int new_rpm) {
        rpm = new_rpm;
    }
    
    int get_rpm() const { return rpm; }
    bool running() const { return is_running; }
};

// Usage
int main() {
    Motor motor1("Motor1", 1000);  // Constructor called
    motor1.start();
    
    Motor motor2 = motor1;         // Copy constructor called
    motor2.set_rpm(2000);
    
    // Destructor called when objects go out of scope
    return 0;
}
```

**Key Points:**
- Constructor: Initialize object state
- Destructors: Cleanup resources (files, memory, interrupts)
- Default: Called if not explicitly defined

**Embedded Context:** Motor/device initialization and cleanup

---

### Q2: Default, parameterized, and copy constructors
**Answer:**
```cpp
class Timer {
private:
    uint32_t period_ms;
    bool auto_reload;
    
public:
    // Default constructor
    Timer() : period_ms(1000), auto_reload(false) {
        std::cout << "Default timer created (1000ms)\n";
    }
    
    // Parameterized constructor
    Timer(uint32_t period, bool reload) 
        : period_ms(period), auto_reload(reload) {
        std::cout << "Timer created: " << period << "ms, auto_reload=" 
                  << reload << "\n";
    }
    
    // Copy constructor
    Timer(const Timer& other) 
        : period_ms(other.period_ms), auto_reload(other.auto_reload) {
        std::cout << "Timer copied\n";
    }
    
    // Move constructor (C++11) - for efficiency
    Timer(Timer&& other) noexcept 
        : period_ms(other.period_ms), auto_reload(other.auto_reload) {
        // Steal resources from temporary
        std::cout << "Timer moved\n";
    }
};

// Usage example
int main() {
    Timer t1;                      // Default: 1000ms
    Timer t2(500, true);           // Parameterized: 500ms, auto-reload
    Timer t3 = t1;                 // Copy: t1 copied to t3
    Timer t4 = std::move(t2);      // Move: t2 resources moved to t4
    
    return 0;
}
```

**Best Practice:** Provide meaningful defaults for embedded devices

---

## Inheritance

### Q1: Single inheritance for embedded systems
**Answer:**
```cpp
// Base class - abstract interface
class PeripheralDevice {
protected:
    uint8_t device_id;
    volatile uint32_t *base_address;
    
public:
    PeripheralDevice(uint8_t id, volatile uint32_t *addr) 
        : device_id(id), base_address(addr) {}
    
    virtual void initialize() = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual ~PeripheralDevice() {}
    
    uint8_t get_id() const { return device_id; }
};

// Derived class - UART implementation
class UART : public PeripheralDevice {
private:
    uint32_t baud_rate;
    
public:
    UART(uint8_t id, volatile uint32_t *addr, uint32_t baud)
        : PeripheralDevice(id, addr), baud_rate(baud) {}
    
    void initialize() override {
        // Configure UART registers
        base_address[0] = (SystemClock / (16 * baud_rate)) - 1;
        std::cout << "UART" << (int)device_id << " initialized\n";
    }
    
    void enable() override {
        base_address[1] |= 0x01;  // Set enable bit
        std::cout << "UART" << (int)device_id << " enabled\n";
    }
    
    void disable() override {
        base_address[1] &= ~0x01; // Clear enable bit
        std::cout << "UART" << (int)device_id << " disabled\n";
    }
    
    void send_byte(uint8_t data) {
        base_address[2] = data;
    }
    
    uint8_t receive_byte() {
        return (uint8_t)base_address[3];
    }
};

// Another derived class - SPI
class SPI : public PeripheralDevice {
private:
    uint8_t clock_divider;
    
public:
    SPI(uint8_t id, volatile uint32_t *addr, uint8_t divider)
        : PeripheralDevice(id, addr), clock_divider(divider) {}
    
    void initialize() override {
        base_address[0] = clock_divider << 8;
        std::cout << "SPI" << (int)device_id << " initialized\n";
    }
    
    void enable() override {
        base_address[0] |= 0x01;
    }
    
    void disable() override {
        base_address[0] &= ~0x01;
    }
    
    uint8_t transfer_byte(uint8_t tx_data) {
        base_address[1] = tx_data;
        while (!(base_address[0] & 0x80));  // Wait for completion
        return (uint8_t)base_address[1];
    }
};

// Usage - polymorphism in action
int main() {
    // Initialize different peripherals through common interface
    UART uart0(0, (volatile uint32_t*)0x40000000, 115200);
    SPI spi0(0, (volatile uint32_t*)0x40010000, 4);
    
    PeripheralDevice *devices[] = {&uart0, &spi0};
    
    for (int i = 0; i < 2; i++) {
        devices[i]->initialize();
        devices[i]->enable();
    }
    
    return 0;
}
```

**Key Benefits:**
- Code reuse (common interface)
- Type safety with virtual functions
- Polymorphic device handling

---

## Polymorphism

### Q1: Function overriding and virtual functions
**Answer:**
```cpp
class Device {
public:
    // Virtual function - can be overridden
    virtual void operate() {
        std::cout << "Generic device operating\n";
    }
    
    // Pure virtual - must be overridden
    virtual uint8_t read_status() = 0;
    
    // Non-virtual - always calls Device version
    void report_status() {
        std::cout << "Status: OK\n";
        read_status();  // Virtual call happens here
    }
    
    virtual ~Device() {}
};

class Sensor : public Device {
private:
    uint16_t last_reading = 0;
    
public:
    void operate() override {
        std::cout << "Sensor operating - reading value\n";
        // Sensor-specific operation
    }
    
    uint8_t read_status() override {
        std::cout << "Sensor reading: " << last_reading << "\n";
        return (last_reading > 100) ? 1 : 0;
    }
};

class Actuator : public Device {
private:
    uint8_t control_signal = 0;
    
public:
    void operate() override {
        std::cout << "Actuator operating - changing position\n";
    }
    
    uint8_t read_status() override {
        std::cout << "Actuator position: " << (int)control_signal << "%\n";
        return control_signal;
    }
};

// Polymorphic function
void system_check(Device *device) {
    device->operate();      // Virtual call - right version called
    uint8_t status = device->read_status();
    device->report_status();
}

int main() {
    Sensor sensor;
    Actuator actuator;
    
    system_check(&sensor);    // Calls Sensor::operate()
    system_check(&actuator);  // Calls Actuator::operate()
    
    return 0;
}
```

**Virtual Function Mechanism:**
```
class Device {          Compiler generates:
    virtual void foo()  
    virtual void bar()  typedef struct {
};                          void (*foo)(Device*);
                            void (*bar)(Device*);
                        } VTable;
                        
Device *obj points to:
┌─────────────────┐
│ vptr ────────┐  │
├─────────────┤  │
│ member_var1 │  │
├─────────────┤  │
│ member_var2 │  │
└─────────────┘  │
                 │
                 ▼
            ┌─────────────────┐
            │ Device::foo*    │
            ├─────────────────┤
            │ Device::bar*    │
            └─────────────────┘
```

**Performance:** Virtual call = one extra pointer dereference (~2-3 cycles)

---

## Encapsulation

### Q1: Access modifiers and data hiding
**Answer:**
```cpp
class MotorController {
private:
    // Private - only accessible within class
    uint32_t max_speed;
    uint16_t current_speed;
    
    // Private helper function
    void validate_speed(uint16_t speed) {
        if (speed > max_speed) {
            current_speed = max_speed;  // Clamp to max
        } else {
            current_speed = speed;
        }
    }
    
protected:
    // Protected - accessible to derived classes
    void update_hardware_registers() {
        volatile uint32_t *control_reg = (volatile uint32_t*)0x40000000;
        *control_reg = current_speed;
    }
    
public:
    // Public interface
    MotorController(uint32_t max) : max_speed(max), current_speed(0) {}
    
    // Setter with validation
    void set_speed(uint16_t speed) {
        validate_speed(speed);
        update_hardware_registers();
    }
    
    // Getter - safe read-only access
    uint16_t get_speed() const {
        return current_speed;
    }
    
    uint32_t get_max_speed() const {
        return max_speed;
    }
};

// Usage
int main() {
    MotorController motor(5000);
    
    motor.set_speed(3000);  // OK - validated
    // motor.max_speed = 10000;  // ERROR - private!
    // motor.current_speed = 9999;  // ERROR - private!
    
    return 0;
}
```

**Benefits:**
- Data validation before setting
- Implementation hiding (can change internals)
- Prevents accidental corruption

---

## Abstraction

### Q1: Abstract classes and interfaces
**Answer:**
```cpp
// Abstract base class - defines interface
class SensorInterface {
public:
    // Pure virtual functions - must be implemented
    virtual void initialize() = 0;
    virtual uint16_t read_value() = 0;
    virtual void calibrate() = 0;
    
    // Virtual with default implementation
    virtual void enable() {
        std::cout << "Sensor enabled\n";
    }
    
    virtual ~SensorInterface() = default;
};

// Concrete implementation
class TemperatureSensor : public SensorInterface {
private:
    uint16_t calibration_offset = 0;
    volatile uint32_t *adc_register;
    
public:
    TemperatureSensor(volatile uint32_t *adc_addr) 
        : adc_register(adc_addr) {}
    
    void initialize() override {
        // ADC setup code
        std::cout << "Temperature sensor initialized\n";
    }
    
    uint16_t read_value() override {
        // Read ADC and apply calibration
        uint16_t raw = *adc_register;
        return raw + calibration_offset;
    }
    
    void calibrate() override {
        // 25°C reference calibration
        calibration_offset = 2048 - (*adc_register);
        std::cout << "Calibrated with offset: " << calibration_offset << "\n";
    }
};

class PressureSensor : public SensorInterface {
private:
    uint16_t baseline_pressure = 0;
    volatile uint32_t *i2c_data;
    
public:
    PressureSensor(volatile uint32_t *i2c_addr) 
        : i2c_data(i2c_addr) {}
    
    void initialize() override {
        // I2C initialization
        std::cout << "Pressure sensor initialized\n";
    }
    
    uint16_t read_value() override {
        // Read from I2C
        uint16_t pressure = *i2c_data;
        return pressure - baseline_pressure;
    }
    
    void calibrate() override {
        baseline_pressure = *i2c_data;
        std::cout << "Baseline pressure: " << baseline_pressure << "\n";
    }
};

// Generic sensor handler - works with any sensor
class SensorManager {
private:
    SensorInterface *sensors[10];
    int sensor_count = 0;
    
public:
    void register_sensor(SensorInterface *sensor) {
        if (sensor_count < 10) {
            sensors[sensor_count++] = sensor;
        }
    }
    
    void initialize_all() {
        for (int i = 0; i < sensor_count; i++) {
            sensors[i]->initialize();
        }
    }
    
    void calibrate_all() {
        for (int i = 0; i < sensor_count; i++) {
            sensors[i]->calibrate();
        }
    }
    
    void read_all() {
        for (int i = 0; i < sensor_count; i++) {
            uint16_t value = sensors[i]->read_value();
            std::cout << "Sensor " << i << " value: " << value << "\n";
        }
    }
};

// Usage
int main() {
    TemperatureSensor temp((volatile uint32_t*)0x40010000);
    PressureSensor pressure((volatile uint32_t*)0x40020000);
    
    SensorManager manager;
    manager.register_sensor(&temp);
    manager.register_sensor(&pressure);
    
    manager.initialize_all();
    manager.calibrate_all();
    manager.read_all();
    
    return 0;
}
```

**Advantages:**
1. **Decoupling:** SensorManager doesn't know sensor types
2. **Extensibility:** Add new sensors without modifying manager
3. **Testability:** Mock sensors for testing
4. **Polymorphism:** Single interface, many implementations

---

## Real-World Embedded Example

```cpp
// Device Driver Architecture

// Abstract interface
class I2CDevice {
public:
    virtual uint8_t get_address() = 0;
    virtual void initialize() = 0;
    virtual void read_registers(uint8_t *buffer, uint8_t count) = 0;
    virtual void write_registers(const uint8_t *buffer, uint8_t count) = 0;
    virtual ~I2CDevice() = default;
};

// EEPROM implementation
class EEPROM_24C256 : public I2CDevice {
private:
    uint8_t i2c_address;
    volatile uint32_t *i2c_base;
    
public:
    EEPROM_24C256(uint8_t addr, volatile uint32_t *base) 
        : i2c_address(addr), i2c_base(base) {}
    
    uint8_t get_address() override { return i2c_address; }
    
    void initialize() override {
        // Verify EEPROM responds
        uint8_t dummy = 0;
        read_registers(&dummy, 1);
    }
    
    void read_registers(uint8_t *buffer, uint8_t count) override {
        // I2C read sequence
        i2c_base[0] = I2C_START;
        i2c_base[1] = i2c_address << 1;
        for (uint8_t i = 0; i < count; i++) {
            buffer[i] = i2c_base[2];
        }
        i2c_base[0] = I2C_STOP;
    }
    
    void write_registers(const uint8_t *buffer, uint8_t count) override {
        i2c_base[0] = I2C_START;
        i2c_base[1] = (i2c_address << 1) | 1;
        for (uint8_t i = 0; i < count; i++) {
            i2c_base[2] = buffer[i];
        }
        i2c_base[0] = I2C_STOP;
    }
};

// Accelerometer implementation  
class ADXL345 : public I2CDevice {
private:
    uint8_t i2c_address;
    volatile uint32_t *i2c_base;
    float scale_factor;
    
public:
    ADXL345(uint8_t addr, volatile uint32_t *base) 
        : i2c_address(addr), i2c_base(base), scale_factor(0.004f) {}
    
    uint8_t get_address() override { return i2c_address; }
    
    void initialize() override {
        uint8_t config[] = {0x31, 0x0B};  // Set range to ±16g
        write_registers(config, 2);
    }
    
    void read_registers(uint8_t *buffer, uint8_t count) override {
        // I2C read implementation
    }
    
    void write_registers(const uint8_t *buffer, uint8_t count) override {
        // I2C write implementation
    }
};

// Generic I2C manager
class I2CManager {
private:
    I2CDevice *devices[10];
    int device_count = 0;
    
public:
    void add_device(I2CDevice *device) {
        if (device_count < 10) {
            devices[device_count++] = device;
            device->initialize();
        }
    }
    
    void initialize_all() {
        for (int i = 0; i < device_count; i++) {
            devices[i]->initialize();
        }
    }
};
```

This OOP architecture enables:
- **Modularity:** Each device self-contained
- **Reusability:** Generic I2C manager works with any I2C device
- **Testability:** Mock I2CDevice for testing
- **Extensibility:** Add new devices without touching manager code

Perfect for embedded systems projects!
