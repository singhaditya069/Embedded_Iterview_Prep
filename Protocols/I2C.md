# I2C (Inter-Integrated Circuit)

## Overview
I2C (also known as TWI - Two Wire Interface) is a synchronous, multi-master, multi-slave serial communication protocol. It uses only two wires for communication, making it ideal for connecting multiple devices on a single bus. Originally developed by Philips Semiconductor (now NXP).

## Block Diagram

```
┌─────────────────────────────────────────────────┐
│            I2C Bus (SDA & SCL lines)            │
└────────┬────────────────┬────────────────┬──────┘
         │                │                │
    ┌────▼────┐      ┌────▼────┐     ┌────▼────┐
    │ Master  │      │ Slave 1  │     │ Slave N │
    │  (MCU)  │      │(EEPROM)  │     │ (Sensor)│
    │         │      │          │     │         │
    │ SDA─────┼──────┼─────SDA──┼─────┼──────────┤
    │ SCL─────┼──────┼─────SCL──┼─────┼──────────┤
    │ GND─────┼──────┼─────GND──┼─────┼──────────┤
    └────┬────┘      └──────────┘     └─────────┘
         │
    ┌────▼─────────┐
    │ Pull-up      │
    │ Resistors    │
    │ (4.7K-10K)   │
    └──────────────┘
```

## Technical Specifications

| Parameter | Standard Mode | Fast Mode | Fast+ Mode |
|-----------|---------------|----|-----------|
| **Baud Rate** | 100 kbps | 400 kbps | 1000 kbps |
| **Max Slaves** | 127 | 127 | 127 |
| **Bus Voltage** | 5V (TTL) / 3.3V | 5V / 3.3V | 3.3V |
| **Output Rise Time** | 1000ns | 300ns | 100ns |
| **Capacitance (max)** | 400pF | 400pF | 400pF |
| **Clock Stretching** | Yes | Yes | Yes |
| **Cable Length** | ~1 meter | ~1 meter | ~0.5 meter |

## Pin Configuration

```
I2C Device (8-pin DIP example)
┌─────────────┐
│    1 ●    8 │
│ VCC        │ GND (pin 4)
│    2    7  │
│ SDA        │
│    3    6  │
│ SCL        │ (A2)
│    4 ● ● 5 │
│ GND    A1  │ (A0)
└─────────────┘
```

## Waveform & Frame Structure

```
START Condition:
    SDA: HIGH ──┐
              └─ LOW  (with SCL HIGH)
    SCL: ──────┐
             └─ (stays HIGH)
    
    
Byte Transfer (8 bits + ACK/NACK):
SCL: ┐┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌──┐┌──────────┐┌───
     └┘  └┘  └┘  └┘  └┘  └┘  └┘  └┘  └┘          └┘
     
SDA: ━━╱▔▔╲━━╱▔▔╲━━╱▔▔╲━━╱▔▔╲━━╱▔▔╲━━╱▔▔╲━━╱▔▔╲━━╱▔▔╲━ (Data bits)
     ┌──────────────────────────────────────────────┐┌─
     │ Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0 ACK │ STOP
     │(MSB)                                (LSB)    │
     └──────────────────────────────────────────────┘└─
     
STOP Condition:
    SDA: LOW  ────────┐
              ──────── HIGH (with SCL HIGH)
    SCL: HIGH ──────┐
             ──────┘ (returns LOW after SDA released)
```

## slave Address Format

```
7-bit Addressing:
┌────┬────┬────┬────┬────┬────┬────┬────┬────┐
│ A6 │ A5 │ A4 │ A3 │ A2 │ A1 │ A0 │ R/W│ ACK│
└────┴────┴────┴────┴────┴────┴────┴────┴────┘
 Device Address (7 bits)            │   │
                                    │   └─ Bit 0: Read(1) or Write(0)
                                    └───── Sent by Slave ACK/NACK

10-bit Addressing:
┌───────┬───────┬┬───────────┬────┬────┐
│1 1 0 │ A9 A8 ││ A7-A0     │ R/W│ACK │
└───────┴───────┘└───────────┴────┴────┘
 Reserved (always  Address bits    │   └─ Slave ACK/NACK
 110 binary)       (10 bits total)
```

---

## How I2C Works

### Communication Steps:

#### 1. **START Condition**
```
Normal state: Both SDA and SCL HIGH
Master pulls SDA LOW while SCL is HIGH
This is detected by all slaves as a START condition
```

#### 2. **Send Address Byte**
```
Master sends 7-bit address + R/W bit (8 bits total, MSB first)
7-bit addr format: [A6][A5][A4][A3][A2][A1][A0][R/W]
R/W bit: 1 = Read from slave, 0 = Write to slave
```

#### 3. **Slave ACKnowledge**
```
Addressed slave pulls SDA LOW during 9th clock pulse
This signals slave presence and readiness
Master releases SDA (pulled up by resistor)
```

#### 4. **Data Transfer**
```
8 bits sent MSB first
Master generates clock pulses on SCL
Data changes on SDA when SCL is LOW
Data read on SDA when SCL is HIGH
```

#### 5. **Acknowledge Signal**
```
After each byte:
Receiver pulls SDA LOW during 9th clock pulse
Sender releases SDA for this bit
Missing ACK = NACK (Negative Acknowledge) - communication stops
```

#### 6. **STOP Condition**
```
Master releases SCL (goes HIGH)
Then releases SDA (goes HIGH)
This signals end of transmission to all slaves
```

### Complete Read/Write Sequence:

```cpp
// I2C Master Write Operation
// Transmit data to slave address 0x50

START
Send Address Byte: 10100000 (0x50 with R/W=0)
↓ Slave ACK
Send Data Byte 1: 11001010
↓ Master Expects ACK
Send Data Byte 2: 11110000
↓ Master Expects ACK
...
STOP

// I2C Master Read Operation
// Read data from slave address 0x50

START
Send Address Byte: 10100001 (0x50 with R/W=1)
↓ Slave ACK
Receive Data Byte 1: 11001010
↓ Master sends ACK (or NACK if last byte)
Receive Data Byte 2: 11110000
↓ Master sends NACK (to stop communication)
STOP
```

---

## Clock Stretching

```
Normal Clock Pulse:
SCL: ┐┌──────┐┌──────┐
     └┘      └┘      └┘
     
With Clock Stretching (Slave needs more time):
SCL: ┐┌──────┐┌──────────────────────────────┐┌──────┐
     └┘      └┘                              └┘      └┘
           ↑ Slave pulls SCL LOW longer       ↑ Slave releases
           (Slave needs time to process)      (Ready to continue)
           
Master must wait for SCL to go HIGH
before proceeding to next bit
```

---

## Interview Questions & Answers

### BEGINNER LEVEL

**Q1: What does I2C stand for?**
A: I2C stands for Inter-Integrated Circuit (also called TWI - Two Wire Interface). It's a synchronous serial communication protocol developed by Philips Semiconductor for connecting multiple chips in a system.

**Q2: How many wires does I2C use and what are they?**
A: I2C uses 2 wires:
- **SDA** (Serial Data): Bidirectional data line with pull-up resistor
- **SCL** (Serial Clock): Clock line with pull-up resistor

Both lines use open-drain output that can be pulled LOW by any device but float HIGH through pull-up resistors.

**Q3: What are the advantages of I2C over UART?**
A:
- I2C can address multiple devices on one bus (UART = point-to-point only)
- Uses only 2 wires (UART uses 2, but I2C is bidirectional on one wire)
- Has built-in addressing mechanism
- Master-Slave architecture is flexible
- Supports multiple masters

**Q4: Why are pull-up resistors needed in I2C?**
A: I2C devices use open-drain outputs (they can only pull lines LOW). Pull-up resistors:
- Pull SDA and SCL HIGH when released
- Create defined HIGH state
- Allow multiple devices to share the bus safely
- Typical values: 4.7kΩ to 10kΩ (depends on bus capacitance)

**Q5: What is the default I2C address for common devices?**
A:
```
EEPROM (AT24C256): 0x50, 0x54
RTC (DS1307): 0x68
Temperature Sensor (LM75): 0x48-0x4F
Humidity Sensor (DHT): 0x40-0x41
OLED Display (SSD1306): 0x3C, 0x3D
Multiplexer (PCA9547): 0x70-0x77
```

**Q6: What does "ACK" and "NACK" mean?**
A:
- **ACK (Acknowledge)**: Slave pulls SDA LOW during 9th clock bit to confirm byte reception
- **NACK (Not Acknowledge)**: Slave leaves SDA HIGH, indicating unable to receive or end of read

**Q7: What are the I2C speed modes?**
A:
- **Standard**: 100 kbps (max)
- **Fast**: 400 kbps (max)
- **Fast+**: 1000 kbps (max)
- **HS (High Speed)**: 3.4 Mbps (requires special devices)

---

### INTERMEDIATE LEVEL

**Q8: Explain the START and STOP conditions.**
A:
```
START Condition:
• SDA goes from HIGH → LOW while SCL is HIGH
• Signals all devices on bus to pay attention
• Master always initiates

START: SCL = HIGH
       SDA: ───┐
            └─ (HIGH to LOW transition)

STOP Condition:
• SDA goes from LOW → HIGH while SCL is HIGH
• Signals end of transmission
• Releases the bus for other masters

STOP:  SCL = HIGH
       SDA: ─┐
          └── (LOW to HIGH transition)
```

**Q9: How is the 7-bit slave address used?**
A:
```
Address Byte Format (8 bits total):
┌─────────────────────┬────────────┐
│ 7-bit Address (A6-A0)│ R/W (1 bit)│
└─────────────────────┴────────────┘

Example: Address 0x50 = 1010000 (binary)

Write operation (R/W=0):
Address byte = 10100000 = 0xA0

Read operation (R/W=1):
Address byte = 10100001 = 0xA1

Slave 0x50 listens for either 0xA0 (write) or 0xA1 (read)
```

**Q10: What happens during an ACK/NACK signal?**
A:
```
Normal ACK (Slave ready):
SCL: ┐┌────┐┌────┐
     └┘    └┘    └┘ (Clock pulses)
SDA: ───────────┐   (SDA HIGH during data bits)
             └─┘    (Slave pulls SDA LOW on 9th bit)

NACK (Slave not ready or last byte):
SCL: ┐┌────┐┌────┐
     └┘    └┘    └┘
SDA: ───────────────  (SDA stays HIGH on 9th bit)

Master reads SDA during 9th clock pulse:
- If LOW = ACK (continue)
- If HIGH = NACK (stop)
```

**Q11: What is clock stretching and why is it important?**
A: Clock stretching allows slaves to slow down communication:
```
Scenario: Slave needs time to process data
1. Master pulls SCL LOW and releases it (expects HIGH)
2. Slave also pulls SCL LOW (holds it down)
3. Master waits... slave is "stretching" the clock
4. When slave is ready, it releases SCL
5. Communication continues

Code benefit: Handles devices of different speeds
uint8_t i2c_write(uint8_t addr, uint8_t *data, uint8_t len) {
    // Start condition
    // Address byte with ACK/NACK check
    for (int i = 0; i < len; i++) {
        i2c_send_byte(data[i]);
        // Device may stretch clock here if needed
        if (!i2c_wait_ack()) return 0;
    }
    // Stop condition
    return 1;
}
```

**Q12: How do you detect a busy I2C bus?**
A:
```cpp
// Check if I2C bus is busy (both lines LOW)
bool i2c_is_busy() {
    // Configure pins as inputs temporarily
    // If both SDA and SCL are LOW, bus is busy
    // Master should wait before START
    
    if (!(SDA_READ) || !(SCL_READ)) {
        return true;  // Bus busy
    }
    return false;  // Bus free
}

// Safe START sequence:
void i2c_start() {
    while (i2c_is_busy()) {
        // Wait for bus to be free
        _delay_ms(1);
    }
    // Now generate START condition
    SDA_LOW();  // SCL already HIGH
}
```

**Q13: What types of I2C errors can occur?**
A:
```
1. NACK (No Acknowledge):
   - Slave not present
   - Slave not ready
   - End of data during read
   
2. Bus Hang (SCL stuck LOW):
   - Slave didn't release SCL
   - Device failure
   - Recovery: Send clock pulses to reset
   
3. Arbitration Failure (multi-master):
   - Two masters drive bus simultaneously
   - First to detect loss backs off
   - Other master continues
   
4. Collision:
   - Data conflict during transmission
   - Master detects when output ≠ expected state
   
5. CRC Error (if used):
   - Data corruption
   - Bus noise/EMI
```

---

### ADVANCED LEVEL

**Q14: Design an I2C system to read temperature from 4 sensors at addresses 0x48, 0x49, 0x4A, 0x4B.**
A:
```cpp
#define TEMP_SENSOR1 0x48
#define TEMP_SENSOR2 0x49
#define TEMP_SENSOR3 0x4A
#define TEMP_SENSOR4 0x4B

#define TEMP_REG 0x00  // Temperature register
#define CONFIG_REG 0x01 // Configuration register

typedef struct {
    uint8_t address;
    float temperature;
    uint8_t status;
} TemperatureSensor_t;

// Hardware I2C initialization (AVR example)
void i2c_init(unsigned long clock) {
    TWSR = 0x00;  // No prescaler
    TWBR = ((F_CPU / clock) - 16) / 2;
    TWCR = (1 << TWEN);  // Enable TWI
}

// I2C start condition
void i2c_start() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

// I2C stop condition
void i2c_stop() {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

// I2C write byte
void i2c_write_byte(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

// I2C read byte with ACK
uint8_t i2c_read_ack() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

// I2C read byte with NACK (last byte)
uint8_t i2c_read_nack() {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

// Read temperature from sensor
float read_temperature(uint8_t sensor_addr) {
    uint8_t temp_msb, temp_lsb;
    float temperature;
    
    // Start condition
    i2c_start();
    
    // Send address with READ bit
    i2c_write_byte((sensor_addr << 1) | 0x01);
    
    // Read MSB with ACK
    temp_msb = i2c_read_ack();
    
    // Read LSB with NACK (last byte)
    temp_lsb = i2c_read_nack();
    
    // Stop condition
    i2c_stop();
    
    // Convert to temperature
    // Sensor format: MMMMMMMM LLL00000 (°C)
    temperature = temp_msb + ((temp_lsb >> 5) * 0.125f);
    
    return temperature;
}

// Main program
int main() {
    i2c_init(100000);  // 100 kHz
    
    TemperatureSensor_t sensors[4] = {
        {TEMP_SENSOR1, 0},
        {TEMP_SENSOR2, 0},
        {TEMP_SENSOR3, 0},
        {TEMP_SENSOR4, 0}
    };
    
    while (1) {
        // Read all sensors
        for (int i = 0; i < 4; i++) {
            sensors[i].temperature = read_temperature(sensors[i].address);
        }
        _delay_ms(1000);  // 1 second interval
    }
    return 0;
}
```

**Q15: Implement I2C EEPROM read/write operations.**
A:
```cpp
#define EEPROM_ADDR 0x50  // AT24C256
#define PAGE_SIZE 64      // Bytes per page
#define MAX_ADDR 0x7FFF   // 32KB = 32768 bytes

// Write single byte to EEPROM
uint8_t eeprom_write_byte(uint16_t addr, uint8_t data) {
    i2c_start();
    
    // Send address byte
    if (i2c_write((EEPROM_ADDR << 1) | 0) == 0) {
        i2c_stop();
        return 0;  // NACK
    }
    
    // Send high byte of address
    if (i2c_write((addr >> 8) & 0xFF) == 0) {
        i2c_stop();
        return 0;
    }
    
    // Send low byte of address
    if (i2c_write(addr & 0xFF) == 0) {
        i2c_stop();
        return 0;
    }
    
    // Send data byte
    if (i2c_write(data) == 0) {
        i2c_stop();
        return 0;
    }
    
    i2c_stop();
    _delay_ms(5);  // Write cycle time ~5ms
    return 1;
}

// Read single byte from EEPROM
uint8_t eeprom_read_byte(uint16_t addr) {
    uint8_t data;
    
    // Send address (write operation)
    i2c_start();
    i2c_write((EEPROM_ADDR << 1) | 0);
    i2c_write((addr >> 8) & 0xFF);
    i2c_write(addr & 0xFF);
    
    // Repeated START
    i2c_start();
    i2c_write((EEPROM_ADDR << 1) | 1);  // Read bit set
    
    // Read data
    data = i2c_read_nack();
    i2c_stop();
    
    return data;
}

// Write page (efficient)
uint8_t eeprom_write_page(uint16_t addr, uint8_t *data, uint8_t len) {
    if (len > PAGE_SIZE) return 0;
    
    i2c_start();
    i2c_write((EEPROM_ADDR << 1) | 0);
    i2c_write((addr >> 8) & 0xFF);
    i2c_write(addr & 0xFF);
    
    for (int i = 0; i < len; i++) {
        i2c_write(data[i]);
    }
    
    i2c_stop();
    _delay_ms(5);
    return 1;
}

// Read sequential bytes
uint8_t eeprom_read_bytes(uint16_t addr, uint8_t *data, uint8_t len) {
    // Set address
    i2c_start();
    i2c_write((EEPROM_ADDR << 1) | 0);
    i2c_write((addr >> 8) & 0xFF);
    i2c_write(addr & 0xFF);
    
    // Repeated START for read
    i2c_start();
    i2c_write((EEPROM_ADDR << 1) | 1);
    
    // Read bytes
    for (int i = 0; i < len; i++) {
        if (i < len - 1) {
            data[i] = i2c_read_ack();
        } else {
            data[i] = i2c_read_nack();  // NACK on last byte
        }
    }
    
    i2c_stop();
    return 1;
}
```

**Q16: Design I2C arbitration for multi-master systems.**
A:
```cpp
// Multi-master I2C with arbitration

#define BUS_ERROR_TIMEOUT 100  // ms

typedef enum {
    I2C_IDLE,
    I2C_MASTER,
    I2C_READY,
    I2C_LOST_ARBITRATION
} i2c_state_t;

volatile i2c_state_t i2c_state = I2C_IDLE;

// Check if bus is free
uint8_t i2c_bus_free() {
    return (SDA_READ && SCL_READ);
}

// Attempt to become master
uint8_t i2c_request_bus() {
    uint32_t timeout = 0;
    
    // Wait for bus to be free
    while (!i2c_bus_free()) {
        _delay_us(1);
        if (++timeout > BUS_ERROR_TIMEOUT * 1000) {
            return 0;  // Bus stuck
        }
    }
    
    // Send START condition
    SDA_LOW();
    SCL_LOW();
    i2c_state = I2C_MASTER;
    return 1;
}

// Check for arbitration loss during transmission
uint8_t i2c_detect_arbitration_loss() {
    // Master transmits LOW, monitors SDA for expected LOW
    // If SDA is HIGH when master expects LOW = arbitration loss
    
    if (!SDA_CURRENT_VALUE && SDA_READ) {
        // Master pulled LOW, but SDA is HIGH
        // Another master pulled SDA HIGH
        i2c_state = I2C_LOST_ARBITRATION;
        return 1;  // Lost arbitration
    }
    return 0;  // Still winning arbitration
}

// Safe write with arbitration check
uint8_t i2c_safe_write(uint8_t data) {
    for (int bit = 7; bit >= 0; bit--) {
        // Set data bit
        if (data & (1 << bit)) {
            SDA_RELEASE();  // HIGH
        } else {
            SDA_LOW();      // LOW
        }
        
        // Generate clock
        SCL_RELEASE();  // HIGH
        _delay_us(5);
        
        // Check for arbitration loss
        if (i2c_detect_arbitration_loss()) {
            SCL_LOW();
            return 0;  // Lost arbitration
        }
        
        SCL_LOW();
        _delay_us(5);
    }
    
    // Wait for ACK
    SDA_RELEASE();
    SCL_RELEASE();
    _delay_us(5);
    
    uint8_t ack = !SDA_READ;  // 0 = ACK, 1 = NACK
    SCL_LOW();
    
    return ack ? 1 : 0;
}
```

**Q17: Explain how repeated START works.**
A:
Repeated START is used to change direction (read after write) without releasing the bus:

```
Normal START-STOP-START:
START ──────────────── STOP  START ──────────
(Master releases bus, other masters can take over)

Repeated START:
START ──────────── RESTART ──────────── STOP
(Bus stays held, no other master can intervene)

Timing:
SCL: ┐┌─ ┐┌─   ┐┌─ ┐┌─ ┐┌─
     └┘─ └┘─   └┘─ └┘─ └┘─
       Normal   Repeated START
SDA: ─┐      ┌─       ┐     ┌─
    └─┘      └─┐    └───────┘
       ↑             ↓
      STOP        Not released
                  
Use case: Write address byte, then read from device
```

**Q18: Calculate maximum bus capacitance and choose pull-up resistor.**
A:
```
Formula:
R_pullup = 0.473 × τ / C_Load

Where:
τ = rise time (typically 1000ns for standard, 300ns for fast, 100ns for fast+)
C_Load = total bus capacitance

Example for Standard Mode (100kHz):
Maximum capacitance = 400pF (spec limit)
Required rise time = 1000ns

R = 0.473 × 1000ns / 400pF = 1182Ω ≈ 1kΩ

But practical choices:
- Standard mode: 4.7kΩ to 10kΩ
- Fast mode: 2.2kΩ to 4.7kΩ
- Fast+ mode: 1kΩ or adjustable

Practical calculation:
1. Measure actual bus capacitance
2. Calculate minimum resistance
3. Choose nearest resistor value upward (slower rise = safer)
4. Measure rise time on scope
5. Adjust if needed
```

---

## Advantages & Disadvantages

### Advantages ✓
- Only 2 wires (plus GND)
- Multiple devices on single bus
- Multiple masters supported
- Simple, proven technology
- Widely used and documented
- Lower current consumption
- Built-in addressing
- Clock stretching for slow devices

### Disadvantages ✗
- Slower than SPI (100-400 kbps typical)
- Limited distance (~1 meter typical)
- Pull-up resistors affect speed
- Open-drain outputs require pull-ups
- Half-duplex (not full-duplex)
- More complex protocol than UART
- Clock stretching complexity
- Noise sensitivity

---

## Applications

| Application | Typical Use |
|-------------|------------|
| Real-Time Clock (RTC) | DS1307, DS3231 at 0x68 |
| EEPROM | AT24C256 at 0x50 |
| Temperature Sensor | LM75 at 0x48, TMP102 |
| Humidity Sensor | DHT12 at 0x40 |
| Pressure Sensor | BMP280 at 0x76/0x77 |
| Motion Sensor (IMU) | MPU6050 at 0x68 |
| OLED Display | SSD1306 at 0x3C |
| GPIO Expander | PCF8574 at 0x20 |
| Digital Potentiometer | AD5171 at 0x2C |
| LED Driver | TLC5971 at 0x3E |
| Touch Sensor | CAP1188 at 0x29 |
| Multiplexer | PCA9547 at 0x70 |

---

## Comparison with Other Protocols

| Feature | I2C | SPI | UART |
|---------|-----|-----|------|
| Wires | 2 | 4+ | 2 |
| Speed | 100-400 kbps | 10+ Mbps | 115 kbps |
| Multi-slave | Yes | Yes | No |
| Multi-master | Yes | No | No |
| Full-duplex | No | Yes | Yes |
| Distance | ~1m | ~1m | ~50m |
| Clock need | Yes (SCL) | Yes (SCK) | No |
| Addressing | Yes (7/10 bit) | No (CS lines) | No |

---

## Troubleshooting Guide

| Issue | Cause | Solution |
|-------|-------|----------|
| NACK on address | Wrong address | Verify address and device presence |
| Bus hangs | SCL stuck LOW | Send clock pulses to reset |
| Slow communication | Wrong pull-up R | Calculate for capacitance |
| Data corruption | Noise/EMI | Add shielding/shorter cable |
| No communication | Clock line failed | Check SCL pull-up resistor |
| Intermittent errors | Loose connection | Check wiring, try different cable |
| Not detected | Device powered off | Verify power supply |

---

## References
- I2C Specification Revision 6 (NXP)
- AVR TWI (I2C) User Guide
- STM32 I2C Reference Manual
- Various Microcontroller Datasheets
