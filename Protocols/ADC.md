# ADC (Analog-to-Digital Converter)

## Overview
An ADC is a device that converts continuous analog signals (voltage, current) into discrete digital values that microcontrollers can process. ADCs are essential for reading sensor data like temperature, pressure, light intensity, and analog measurements.

## Block Diagram

```
┌─────────────────────────────────────────────┐
│            ADC (Analog-to-Digital)          │
├─────────────────────────────────────────────┤
│                                             │
│  Analog Input  Sample & Hold  Quantizer    │
│    Vin ──────────┬──────────────┬─────────┐│
│    (0-5V or      │              │         ││
│     0-3.3V)      │              │         ││
│                  │              │ Encoder │
│                  ▼              ▼         ▼
│              ┌──────┐      ┌────────┐    │
│  Reference   │S&H   │──────│ Comp.  │────┼─► Digital Output
│  Voltage     └──────┘      └────────┘   │   (Binary/Digital)
│    Vref         │            │          │
│    ──────────────┴────────────┴──────────┘
│                                             │
│              Control Logic                  │
│              Clock Generator                │
└─────────────────────────────────────────────┘
```

## Technical Specifications

| Parameter | Standard | Value |
|-----------|----------|-------|
| **Resolution** | Common | 8, 10, 12, 16-bit |
| **Conversion Time** | Typical | 10-100 µs |
| **Sampling Rate** | Typical | 10 kS/s - 1 MS/s |
| **Input Range** | Standard | 0-5V (TTL), 0-3.3V (CMOS) |
| **Reference Voltage** | Internal/External | 5V, 3.3V, or adjustable |
| **Accuracy** | Typical | ±0.5 - ±2 LSB |
| **Number of Channels** | Typical | 6-16 channels (multiplexed) |
| **Power Consumption** | Typical | 5-50 mW |

## ADC Resolution & Bit Depth

```
Resolution determines digital output range:

8-bit ADC (256 levels):
0000 0000 = 0V (minimum)
1111 1111 = 5V (maximum)
Step size = 5V / 256 = 19.53 mV per LSB

10-bit ADC (1024 levels):
00 0000 0000 = 0V
11 1111 1111 = 5V
Step size = 5V / 1024 = 4.88 mV per LSB

12-bit ADC (4096 levels):
0000 0000 0000 = 0V
1111 1111 1111 = 5V
Step size = 5V / 4096 = 1.22 mV per LSB

16-bit ADC (65536 levels):
0000 0000 0000 0000 = 0V
1111 1111 1111 1111 = 5V
Step size = 5V / 65536 = 76.3 µV per LSB

Formula:
Vin = (Digital_Output × Vref) / (2^Resolution - 1)
Digital_Output = (Vin × (2^Resolution - 1)) / Vref
```

## ADC Conversion Process (Successive Approximation)

### Basic SAR Algorithm

```
Step 1: Initialization
        Start conversion signal
        Set MSB (most significant bit) = 1
        
Step 2: Compare
        DAC generates voltage based on current bit pattern
        Comparator checks: Is Vin > VDAC?
        
Step 3: Adjust Bit
        If Vin > VDAC: Keep bit = 1
        If Vin < VDAC: Set bit = 0
        
Step 4: Move to Next Bit
        Repeat for remaining bits (LSB = last bit)
        
Step 5: Complete
        All bits determined, result ready
```

### Detailed Example Walkthrough (10-bit SAR, Vin = 2.5V, Vref = 5V)

**Given Parameters:**
- Input Voltage (Vin) = 2.5V
- Reference Voltage (Vref) = 5V
- Resolution: 10 bits (1024 levels)
- LSB (Least Significant Bit) = 5V / 1024 = 4.88 mV per step

**The Core Concept: Binary Search**

The SAR ADC uses a binary search algorithm - like a guessing game where you narrow down possibilities:
```
"Is the value above the midpoint?"
├─ YES → Search upper half
└─ NO  → Search lower half
Repeat until you find the answer!
```

---

### **ITERATION 1: Testing Bit 9 (MSB - Most Significant Bit)**

```
Question:      "Is Vin > Vref/2?"
               "Is 2.5V > 2.5V?"

Internal DAC:  Sets to 2.5V (exactly half of 5V reference)
               DAC_Value = 512 (binary: 1000000000)
               
Comparator:    Checks: Vin vs VDAC
               Result: 2.5V > 2.5V? NO ✗
               
Decision:      Bit 9 = 0 (NOT in upper half)

Current Result: 0_________ (9 bits remain to test)

Implication:   The actual value is in LOWER half (0 to 2.5V)
               Search window: 5V → 2.5V (halved!)
```

---

### **ITERATION 2: Testing Bit 8**

```
Current Result: 0_________

Question:      "Is Vin > Vref/4?"
               "Is 2.5V > 1.25V?"

Internal DAC:  Sets to 1.25V (25% of 5V reference)
               Previous bit: 0 from iteration 1
               This bit weight: 256
               DAC_Value = 0×512 + 1×256 = 256
               
Comparator:    2.5V > 1.25V? YES ✓

Decision:      Bit 8 = 1 (YES, value IS in this range)

Current Result: 01________ 

Voltage So Far: 0×512 + 1×256 = 1.25V (25% of reference)

Range Narrowed: 1.25V < Vin ≤ 2.5V (search space halved again!)
               Window width: 1.25V (was 2.5V, now 50% smaller)
```

---

### **ITERATION 3: Testing Bit 7**

```
Current Result: 01________

Question:      "Is Vin > 1.875V?"
               (1.25V + 0.625V = 1.875V)

Internal DAC:  Adds Bit 7 weight (128)
               DAC_Value = 256 + 128 = 384
               Voltage = (384/1024) × 5V = 1.875V
               
Comparator:    2.5V > 1.875V? YES ✓

Decision:      Bit 7 = 1

Current Result: 011_______

New Search Window: 1.875V < Vin ≤ 2.5V
               Window width: 0.625V (continuing to halve)
```

---

### **ITERATION 4: Testing Bit 6**

```
Current Result: 011_______

Internal DAC:  DAC_Value = 256 + 128 + 64 = 448
               Voltage = (448/1024) × 5V = 2.1875V
               
Comparator:    2.5V > 2.1875V? YES ✓

Decision:      Bit 6 = 1

Current Result: 0111______

Search Window: 2.1875V < Vin ≤ 2.5V (0.3125V range)
```

---

### **ITERATION 5: Testing Bit 5**

```
Current Result: 0111______

Internal DAC:  DAC_Value = 256 + 128 + 64 + 32 = 480
               Voltage = (480/1024) × 5V = 2.34375V
               
Comparator:    2.5V > 2.34375V? YES ✓

Decision:      Bit 5 = 1

Current Result: 01111_____

Search Window: 2.34375V < Vin ≤ 2.5V (0.15625V range)
```

---

### **ITERATION 6: Testing Bit 4**

```
Current Result: 01111_____

Internal DAC:  DAC_Value = 256 + 128 + 64 + 32 + 16 = 496
               Voltage = (496/1024) × 5V = 2.421875V
               
Comparator:    2.5V > 2.421875V? YES ✓

Decision:      Bit 4 = 1

Current Result: 011111____
```

---

### **ITERATION 7: Testing Bit 3**

```
Current Result: 011111_____

Internal DAC:  DAC_Value = 256 + 128 + 64 + 32 + 16 + 8 = 504
               Voltage = (504/1024) × 5V = 2.46484375V
               
Comparator:    2.5V > 2.46484375V? YES ✓

Decision:      Bit 3 = 1

Current Result: 0111111____

New Range:     2.46484375V < Vin ≤ 2.5V (0.0352V window)
```

---

### **ITERATION 8: Testing Bit 2**

```
Current Result: 0111111____

Internal DAC:  DAC_Value = 256 + 128 + 64 + 32 + 16 + 8 + 4 = 508
               Voltage = (508/1024) × 5V = 2.48046875V
               
Comparator:    2.5V > 2.48046875V? YES ✓

Decision:      Bit 2 = 1

Current Result: 01111111___

New Range:     2.48046875V < Vin ≤ 2.5V (0.0195V window)
```

---

### **ITERATION 9: Testing Bit 1**

```
Current Result: 01111111___

Internal DAC:  DAC_Value = 256 + 128 + 64 + 32 + 16 + 8 + 4 + 2 = 510
               Voltage = (510/1024) × 5V = 2.48535156V
               
Comparator:    2.5V > 2.48535156V? YES ✓

Decision:      Bit 1 = 1

Current Result: 011111111__

New Range:     2.48535156V < Vin ≤ 2.5V (0.0146V window, smaller than 1 LSB!)
```

---

### **ITERATION 10: Testing Bit 0 (LSB)**

```
Current Result: 011111111__

Internal DAC:  DAC_Value = 256 + 128 + 64 + 32 + 16 + 8 + 4 + 2 + 1 = 511
               Voltage = (511/1024) × 5V = 2.49511719V
               
Comparator:    2.5V > 2.49511719V? YES ✓

Decision:      Bit 0 = 1

Final Result:  0111111111 = 511 (decimal)
               Binary: 0111111111
```

---

### **Visual Convergence Timeline (Corrected)**

```
Iteration    DAC Value    DAC Voltage    Vin (2.5V)    Vin > VDAC?    Bit Value
    1         512         2.5000V        2.5V          NO (=)           0
    2         256         1.2500V        2.5V          YES              1
    3         384         1.8750V        2.5V          YES              1
    4         448         2.1875V        2.5V          YES              1
    5         480         2.3438V        2.5V          YES              1
    6         496         2.4219V        2.5V          YES              1
    7         504         2.4648V        2.5V          YES              1
    8         508         2.4805V        2.5V          YES              1
    9         510         2.4854V        2.5V          YES              1
    10        511         2.4951V        2.5V          YES              1

Final Result:  0111111111 (binary) = 511 (decimal) = 0x1FF (hex)

Search Window Shrinking:
After Iteration 1: 2.5V (entire range below 2.5V)
After Iteration 2: 1.25V (50% smaller)
After Iteration 3: 0.625V (50% smaller)
After Iteration 4: 0.3125V (50% smaller)
After Iteration 5: 0.15625V (50% smaller)
After Iteration 6: 0.07813V (50% smaller)
After Iteration 7: 0.03906V (50% smaller)
After Iteration 8: 0.01953V (50% smaller)
After Iteration 9: 0.00977V (50% smaller)
After Iteration 10: 0.00488V (smaller than 1 LSB = 4.88mV!)

Convergence Complete! ✓
```

---

### **Final Digital Result - Complete Breakdown**

```
Final Binary Representation:  0 1 1 1 1 1 1 1 1 1
                              B9 B8 B7 B6 B5 B4 B3 B2 B1 B0

Weight Breakdown (10-bit, each bit represents a power of 2):
Bit 9 (weight 512):    0 × 512 = 0
Bit 8 (weight 256):    1 × 256 = 256
Bit 7 (weight 128):    1 × 128 = 128
Bit 6 (weight 64):     1 × 64  = 64
Bit 5 (weight 32):     1 × 32  = 32
Bit 4 (weight 16):     1 × 16  = 16
Bit 3 (weight 8):      1 × 8   = 8
Bit 2 (weight 4):      1 × 4   = 4
Bit 1 (weight 2):      1 × 2   = 2
Bit 0 (weight 1):      1 × 1   = 1
                      ─────────────
                      TOTAL = 511

Final Decimal Value:   511 (0x1FF hex)
```

---

### **Critical Understanding: Why 511 Instead of 512?**

```
IMPORTANT CONCEPT - The Boundary Condition:

Input Voltage:        Vin = 2.5V
Reference Voltage:    Vref = 5V
Expected Ratio:       2.5 / 5 = 0.5 (exactly 50%)

Theoretical Digital Value:  0.5 × 1024 = 512

BUT - The SAR Algorithm Uses ">" (STRICTLY GREATER THAN):

At Iteration 1, the algorithm asks:
"Is Vin > Vref/2?" 
"Is 2.5V > 2.5V?" 
Answer: NO (because 2.5 is NOT greater than 2.5, it EQUALS 2.5)

This is mathematically correct but results in:
- Bit 9 = 0 (we exclude the MSB)
- The algorithm searches in the LOWER half
- Final convergence: 511 instead of 512

Reconstructed Voltage from Result:
V = (511 / 1024) × 5V = 2.49511719V

Error Analysis:
Expected: 2.5V
Got:      2.49511719V
Error:    2.5V - 2.49511719V ≈ 4.88 mV = 1 LSB

This represents a QUANTIZATION ERROR of exactly 1 LSB!
```

---

### **What Would Happen With >= (Greater-Than-Or-Equal)?**

```
If the algorithm used ">=" instead of ">":

At Iteration 1:
"Is Vin >= Vref/2?"
"Is 2.5V >= 2.5V?"
Answer: YES (equal values satisfy >=)

Then:
- Bit 9 = 1 (include the MSB)
- Continue testing remaining bits...
- All remaining bits would be 0 (since input exactly at threshold)
- Final Result: 1000000000 = 512 (EXACT!)
- Reconstructed: (512/1024) × 5V = 2.5V ✓

Why Most ADCs Use ">" Instead of ">=":
The > comparison is simpler to implement in hardware (no need for equality check)
The quantization error of 1 LSB is acceptable and expected
This is NORMAL behavior in real ADCs!
```

---

### **Actual Result Summary**

```
Input:                    Vin = 2.5V (exactly 50% of reference)
Output:                   511 (binary: 0111111111)
Reconstructed Voltage:    2.49512V
Quantization Error:       -4.88 mV (one LSB)
Relative Error:           0.195% (acceptable!)

Why This is Normal:
Every analog-to-digital conversion has unavoidable quantization noise
The error is bounded by ±0.5 LSB = ±2.44 mV
This example shows 1 LSB error, which is well within specification
Higher resolution ADCs (12-bit, 16-bit) have smaller LSBs → smaller errors

Final Accuracy:           511/512 = 99.8% (excellent!)
```

---

### **Timing Analysis**

```
Per Iteration Breakdown:
1. DAC settling time:        ~1-2 µs
2. Comparator response:      ~0.5-1 µs
3. Logic/Register update:    ~0.5-1 µs
                            ────────────
   Total per bit:            ~2-4 µs

10-Bit Conversion:
Total iterations:           10 bits
Time per iteration:         2-4 µs
Total conversion time:      20-40 µs (minimum)
Overhead/settling:          60-80 µs (typical)
                           ────────────────
   At 100 kHz clock:        ~100 µs ✓ (matches example)

Clock Requirement:
1 MHz clock → ~10 µs per bit → 100 µs for 10 bits ✓

Faster Implementation:
If using 10 MHz clock → ~1 µs per bit → ~10 µs conversion ✓
```

---

### **Why SAR Works So Well**

```
Binary Search Efficiency:
- Linear search would need: N iterations for N levels (1024 iterations!)
- Binary search needs: log₂(N) iterations (only 10 iterations!)

Exponential Convergence:
After N iterations, error bounds:
Error ≤ Vref / 2^N

For 10-bit: Error ≤ 5V / 1024 ≈ 4.88mV (1 LSB) ✓

Self-Correcting:
If a bit guess is wrong, next iteration corrects it
No negative feedback or oscillation
Simple and robust!

Hardware Requirement:
- 1 DAC (simple, small)
- 1 Comparator (simple, fast)
- 1 SAR Register (logic, tiny)
Total: Minimal hardware = Low cost ✓
```

---

### **Complete Conversion Timeline (Corrected)**

```
Detailed Time-by-Time Progression:

t=0 µs:       Conversion starts
              Testing Bit 9 (MSB)
              Internal DAC sets to 512 (voltage = 2.5V)

t=10 µs:      Bit 9 result recorded as 0 
              Testing Bit 8
              DAC updates to 256 (voltage = 1.25V)

t=20 µs:      Bit 8 result recorded as 1
              Testing Bit 7
              DAC updates to 384 (voltage = 1.875V)
              
t=30 µs:      Bit 7 result recorded as 1
              Testing Bit 6
              DAC updates to 448 (voltage = 2.1875V)

t=40 µs:      Bit 6 result recorded as 1
              Testing Bit 5
              DAC updates to 480 (voltage = 2.3438V)

t=50 µs:      Bit 5 result recorded as 1
              Testing Bit 4
              DAC updates to 496 (voltage = 2.4219V)

t=60 µs:      Bit 4 result recorded as 1
              Testing Bit 3
              DAC updates to 504 (voltage = 2.4648V)

t=70 µs:      Bit 3 result recorded as 1
              Testing Bit 2
              DAC updates to 508 (voltage = 2.4805V)

t=80 µs:      Bit 2 result recorded as 1
              Testing Bit 1
              DAC updates to 510 (voltage = 2.4854V)

t=90 µs:      Bit 1 result recorded as 1
              Testing Bit 0 (LSB)
              DAC updates to 511 (voltage = 2.4951V)

t=100 µs:     Bit 0 result recorded as 1
              ▼▼▼ CONVERSION COMPLETE ▼▼▼

Final Binary Result:  0111111111
Final Decimal Value:  511
Final Hex Value:      0x1FF
Reconstructed V:      2.4951V
Quantization Error:   -4.88 mV (one LSB)

Data ready for application to read
ADC Interrupt signal triggered (if enabled)
```

## ADC Timing Diagram

```
Start Signal:    ┐
                 └─ Starts conversion process

Internal Clock:  ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
                 └─┘ └─┘ └─┘ └─┘ └─┘

Conversion:      |←─ Successive Approximation ─→|

End of Conv.:                               ┐
                                            └─ Conversion complete

Data Output:     ───────────────────────────Data Ready───────────
(VALID after this)

Interrupt:       ───────────────────────────┐
(if enabled)                               └─ ADC Interrupt

Ready Time:      ~100-500 µs for 12-bit SAR ADC
```

---

## How ADC Works

### Sequential Conversion Steps:

1. **Sample & Hold**
   ```
   Analog input sampled
   Capacitor holds value for conversion
   Prevents noise during conversion process
   ```

2. **Successive Approximation (Most Common)**
   ```
   Binary search algorithm
   Test each bit from MSB to LSB
   10 bits = 10 iterations
   12 bits = 12 iterations
   ```

3. **Comparison**
   ```
   Internal DAC generates test voltage
   Comparator checks: Vin vs VDAC
   Output binary digit (0 or 1)
   ```

4. **Conversion Complete**
   ```
   Result available in output register
   Interrupt signal (if enabled)
   Ready for next sample
   ```

### Multiplexed Channel Conversion:

```cpp
// Multiple analog channels
Channel 0 (temp): P0
Channel 1 (light): P1
Channel 2 (sound): P2
...
Channel 7 (motion): P7

ADC Multiplexer: 3-bit select (N0, N1, N2)
000 = Channel 0
001 = Channel 1
010 = Channel 2
...
111 = Channel 7

Conversion Sequence:
1. Select Channel 0
2. Wait for conversion (Tconv)
3. Read result
4. Select Channel 1
5. Wait for conversion
6. Read result
... (repeat for all channels)

Continuous conversion mode:
Automatically cycles through channels
Faster sampling of all channels
```

---

## Interview Questions & Answers

### BEGINNER LEVEL

**Q1: What is an Analog-to-Digital Converter?**
A: An ADC is a device that converts continuous analog signals (voltage ranging from 0-5V or 0-3.3V) into discrete digital values (binary numbers) that microcontrollers can process. Examples: LM358 op-amp based, internal MCU ADC.

**Q2: Why do we need ADC in embedded systems?**
A: Microcontrollers work with digital signals (0 or 1), but many real-world sensors produce analog signals (continuous voltage). ADCs bridge this gap, allowing microcontrollers to read temperature sensors, light sensors, pressure sensors, etc.

**Q3: What is resolution in ADC?**
A: Resolution is the number of bits used to represent the analog value. Higher resolution = finer granularity:
- 8-bit: 256 levels
- 10-bit: 1024 levels
- 12-bit: 4096 levels
- 16-bit: 65536 levels

**Q4: What does LSB (Least Significant Bit) mean in ADC context?**
A: LSB is the smallest voltage change that the ADC can detect:
- For 10-bit with 5V reference: LSB = 5V / 1024 = 4.88 mV
- For 12-bit with 5V reference: LSB = 5V / 4096 = 1.22 mV
- Smaller LSB = better resolution

**Q5: What is sampling rate?**
A: Sampling rate is how many times per second the ADC converts analog to digital. Common rates: 10 kS/s (kilo-samples per second), 1 MS/s (megasamples per second). Nyquist theorem: Sampling rate must be ≥ 2× signal frequency.

**Q6: What is quantization error?**
A: Quantization error is the difference between actual analog value and the digital representation. Maximum error = ±0.5 LSB. Example: True voltage 2.46V quantized to 2.44V (10-bit ADC) = ~2 mV error.

**Q7: What's the difference between ADC and DAC?**
A: 
- **ADC**: Analog → Digital (e.g., sensor reading)
- **DAC**: Digital → Analog (e.g., audio speaker output)

---

### INTERMEDIATE LEVEL

**Q8: Explain Successive Approximation ADC.**
A:
```
Most common type in microcontrollers

Algorithm (Binary Search):
1. Start with MSB (Most Significant Bit)
2. Set current bit to 1
3. Compare Vin with DAC output
4. If Vin > VDAC: keep bit = 1
5. If Vin < VDAC: set bit = 0
6. Move to next lower bit
7. Repeat until all bits determined

Time: N iterations for N-bit conversion

Example (4-bit, Vin = 2.5V, Vref = 5V, ideal digital = 8):
   Bit 3 (MSB=8):     2.5 vs 2.5?  NO   → bit=0 (result=0xxx)
   Bit 2 (4):         2.5 vs 1.25? YES  → bit=1 (result=01xx)
   Bit 1 (2):         2.5 vs 1.875?YES  → bit=1 (result=011x)
   Bit 0 (LSB=1):     2.5 vs 2.34375? YES → bit=1 (result=0111 = 7)
   
Reconstructed: (7 × 5) / 15 = 2.33V (small quantization error)

Advantage: Quick, low power
```

**Q9: How do you calculate the actual voltage from ADC reading?**
A:
```
Formula: Vin = (Digital_Value × Vref) / (2^bits - 1)

Example 1: 10-bit reading = 512, Vref = 5V
Vin = (512 × 5) / (1024 - 1)
Vin = 2560 / 1023
Vin ≈ 2.502V

Example 2: 12-bit reading = 2048, Vref = 3.3V
Vin = (2048 × 3.3) / (4096 - 1)
Vin = 6758.4 / 4095
Vin ≈ 1.65V

Note: (2^bits - 1) because:
- 0 represents 0V
- Max value (2^bits - 1) represents Vref
```

**Q10: What is the Nyquist theorem for ADC?**
A:
```
Nyquist Theorem:
Fs ≥ 2 × Fmax

Where:
Fs = Sampling frequency
Fmax = Maximum frequency in signal

An ADC must sample at least twice per cycle of the highest frequency
to accurately reconstruct the signal.

Example:
Audio signal maximum frequency = 20 kHz (human hearing)
Minimum sampling rate = 2 × 20 kHz = 40 kHz
CD quality uses 44.1 kHz (meets Nyquist requirement)

Consequence of violation:
Sampling below Nyquist rate → Aliasing
High frequencies misrepresented as low frequencies
Data loss and distortion
```

**Q11: What is aliasing in ADC?**
A:
```
Aliasing occurs when sampling frequency < 2× signal frequency

Example:
True signal: 10 kHz sine wave
Sampling rate: 15 kHz (too slow)
ADC sees frequencies that are wrong
Signal appears as 5 kHz instead of 10 kHz

Visualization:
True:     /\  /\  /\  /\  /\  (10 kHz)
Sample:   •   •   •   •   •
Appears: \    /    \    /       (5 kHz - ALIASING!)

Prevention:
1. Use anti-aliasing filter (low-pass) before ADC
2. Sample at frequency >> 2× maximum signal frequency
3. Filter cutoff should be < Sampling_frequency / 2
```

**Q12: Explain ADC noise and filtering techniques.**
A:
```
Noise Sources:
1. Thermal noise (random fluctuations)
2. Power supply noise (ripple)
3. Electromagnetic interference (EMI)
4. Quantization noise

Filtering Techniques:

1. Averaging (Software):
   result = (adc_read() + adc_read() + ... ) / n_samples
   Reduces noise by √n
   Slower response time
   
2. Moving Average:
   result[i] = (adc[i] + adc[i-1] + ... + adc[i-n]) / n
   Good noise reduction
   Introduces lag
   
3. Low-pass Filter (Hardware):
   ┌─┬─────┐
   │R│     │
   └─┴──┬──┘
        │C
        │
       GND
   fc = 1 / (2πRC)
   Example: R=10K, C=100nF → fc = 159Hz
   
4. Exponential Moving Average (IIR):
   result = α × adc_read() + (1-α) × result_previous
   α = 0.1 (smooth, laggy)
   α = 0.5 (responsive)
   Computationally efficient
   
5. Median Filter:
   Sort n samples
   Take middle value
   Good for spike removal
   
6. Butterworth/Chebyshev Filter (DSP):
   More complex, better performance
   Phase compensation
```

---

### ADVANCED LEVEL

**Q13: Design a 10-bit ADC with R-2R ladder DAC.**
A:
```
R-2R Ladder DAC Architecture:
Converts digital bits to analog voltage

Circuit:
         R    2R   2R   2R
    ─────┬────┬────┬────┬─── (MSB to LSB)
         │    │    │    │
        B9   B8   B7   B6

         │    │    │    │
    ─────┴────┴────┴────┴─────
           │
          R │ (Load resistor)
           │
    Vin ──┴─────┬────── Vout
              (output)

Operation:
- Each bit controls a switch (0 or 1)
- Voltage from each bit = Vref × (bit_weight / total_weight)
- MSB contributes Vref/2
- Next bit contributes Vref/4
- Following binary pattern

Example (4-bit, Digital = 1010):
Vout = Vref × (B3×½ + B2×¼ + B1×⅛ + B0×1/16)
Vout = Vref × (1×½ + 0×¼ + 1×⅛ + 0×1/16)
Vout = Vref × (0.5 + 0 + 0.125 + 0)
Vout = Vref × 0.625 = 0.625 × 5V = 3.125V

10-bit Version:
10 bits = 1024 levels
Full voltage range covered
LSB = Vref / 1024

Code Implementation:
void dac_write(uint16_t value) {
    // Set GPIO pins for MSB down to LSB
    for (int i = 9; i >= 0; i--) {
        if (value & (1 << i)) {
            GPIO_SET_BIT(i);  // Set to Vref
        } else {
            GPIO_CLR_BIT(i);  // Set to GND
        }
    }
    // Voltage settles within microseconds
}

Advantages:
+ Accurate, no capacitors drifting
+ Monotonic (no glitches)
+ Suitable for audio-grade DAC

Disadvantages:
- Requires 2N resistors (10-bit = 1024 R + 512 2R)
- Impedance matching critical
- Temperature drift of resistors
```

**Q14: Implement an ADC with oversampling and decimation for noise reduction.**
A:
```cpp
#define OVERSAMPLE_RATIO 16  // Use 16× oversampling
#define TOTAL_SAMPLES (OVERSAMPLE_RATIO * 16)  // 256 samples total
#define OUTPUT_BITS 12       // Output 12-bit (from 10-bit ADC)

uint32_t oversampled_adc_read() {
    uint32_t sum = 0;
    // Collect TOTAL_SAMPLES at high frequency
    for (int i = 0; i < TOTAL_SAMPLES; i++) {
        uint16_t sample = adc_read_single();  // 10-bit reading
        sum += sample;
        // Sample every ~20µs (50kHz sampling rate)
    }
    
    // Decimation: Average all samples
    // sum / 256 shifts right by 8 bits
    uint16_t decimated = sum >> 8;  // Divide by 256
    
    // Result has extra 2 bits of resolution (16→4 gain bits)
    // Noise reduced by √256 = 16×
    return decimated;
}

Benefits:
1. Noise reduction without capacitors
2. Gain pseudo-bits (10-bit → effectively 12-14 bit)
3. DSP-based filtering
4. Temperature-stable performance

Trade-offs:
- Slower sampling (need to read 256× faster internally)
- More CPU/computation
- Results available slower

Alternative: Incremental ΣΔ ADC
These use massive oversampling (64-256×) internally
Much better resolution per power consumed
```

**Q15: Design multi-channel ADC with sequential conversion.**
A:
```cpp
#include <avr/io.h>

#define NUM_CHANNELS 8
#define CHANNEL_TMP36   0   // Temperature sensor
#define CHANNEL_LIGHT   1   // Light sensor
#define CHANNEL_SOUND   2   // Microphone
#define CHANNEL_MOTION  3   // PIR sensor
#define CHANNEL_4       4
#define CHANNEL_5       5
#define CHANNEL_6       6
#define CHANNEL_7       7

volatile uint16_t adc_results[NUM_CHANNELS];
volatile uint8_t current_channel = 0;

// ADC initialization for ATmega328P
void adc_init() {
    // Reference: AVcc, Right-adjust result
    ADMUX = (1 << REFS0) | (0 << ADLAR);  // AVcc reference
    
    // Enable ADC, prescaler = 128 (16MHz/128 = 125kHz)
    ADCSRA = (1 << ADEN) | (1 << ADIE) | (7 << ADPS0);
    
    // External trigger disabled, free-running mode
    ADCSRB = 0;
    
    // Start first conversion
    ADCSRA |= (1 << ADSC);
}

// ADC Complete Interrupt Service Routine
ISR(ADC_vect) {
    // Store result from completed conversion
    adc_results[current_channel] = ADC;
    
    // Switch to next channel
    current_channel++;
    if (current_channel >= NUM_CHANNELS) {
        current_channel = 0;
    }
    
    // Set MUX for next channel
    ADMUX = (ADMUX & 0xF0) | current_channel;
    
    // Start next conversion
    ADCSRA |= (1 << ADSC);
}

// Convert ADC reading to actual value
float get_temperature() {
    // TMP36: 750mV at 25°C, 10mV/°C
    float voltage = (adc_results[CHANNEL_TMP36] * 5.0) / 1024.0;
    float temp = (voltage - 0.75) / 0.01 + 25.0;
    return temp;  // °C
}

float get_light_intensity() {
    // LDR: 0-1023 ADC value
    float intensity = (adc_results[CHANNEL_LIGHT] * 100.0) / 1023.0;
    return intensity;  // Percentage
}

int main() {
    adc_init();
    sei();  // Enable global interrupts
    
    while (1) {
        // Results continuously updated by ISR
        float temp = get_temperature();
        float light = get_light_intensity();
        
        // Log or display results
        _delay_ms(100);  // 100ms display update
    }
    
    return 0;
}

Conversion Timing:
Channel change: ~5µs (MUX switch + recovery)
Conversion: ~105µs (13 clock cycles × 8µs)
Total per channel: ~110µs
8 channels: ~880µs per complete cycle
Cycle rate: ~1.1 kHz for all 8 channels
```

**Q16: Implement digital filtering (IIR) on ADC data.**
A:
```cpp
// Infinitely Impulse Response (IIR) Low-pass Filter
// Simple 1st-order IIR filter

#define ALPHA 0.1  // Filter coefficient (0-1)
                   // 0.1 = smooth, laggy
                   // 0.5 = responsive
                   // Smaller = smoother

typedef struct {
    float filtered_value;
    uint16_t raw_value;
} adc_filter_t;

adc_filter_t temp_sensor = {0.0, 0};

void iir_filter(adc_filter_t *channel, uint16_t new_sample) {
    channel->raw_value = new_sample;
    channel->filtered_value = 
        ALPHA * new_sample + 
        (1.0 - ALPHA) * channel->filtered_value;
}

// Multi-tap FIR (Finite Impulse Response) Filter
#define FIR_TAPS 16

typedef struct {
    uint16_t samples[FIR_TAPS];
    uint8_t index;
    uint32_t sum;
} fir_filter_t;

void fir_filter_init(fir_filter_t *f) {
    for (int i = 0; i < FIR_TAPS; i++) {
        f->samples[i] = 0;
    }
    f->index = 0;
    f->sum = 0;
}

uint16_t fir_filter(fir_filter_t *f, uint16_t new_sample) {
    // Remove oldest sample from sum
    f->sum -= f->samples[f->index];
    
    // Add new sample
    f->samples[f->index] = new_sample;
    f->sum += new_sample;
    
    // Move to next index
    f->index = (f->index + 1) % FIR_TAPS;
    
    // Return average (equivalent to dividing by TAPS)
    return f->sum / FIR_TAPS;
}

// Median Filter (spike removal)
#define MEDIAN_SIZE 5

uint16_t median_filter(uint16_t samples[MEDIAN_SIZE]) {
    // Bubble sort (simple for small N)
    for (int i = 0; i < MEDIAN_SIZE - 1; i++) {
        for (int j = 0; j < MEDIAN_SIZE - i - 1; j++) {
            if (samples[j] > samples[j + 1]) {
                uint16_t temp = samples[j];
                samples[j] = samples[j + 1];
                samples[j + 1] = temp;
            }
        }
    }
    
    // Return middle value
    return samples[MEDIAN_SIZE / 2];
}

// Usage Example
int main() {
    adc_init();
    
    fir_filter_t temp_fir;
    fir_filter_init(&temp_fir);
    
    while (1) {
        uint16_t raw = adc_read();
        
        // Method 1: IIR Filter
        iir_filter(&temp_sensor, raw);
        
        // Method 2: FIR Filter
        uint16_t fir_result = fir_filter(&temp_fir, raw);
        
        // Method 3: Median
        uint16_t samples[MEDIAN_SIZE];
        for (int i = 0; i < MEDIAN_SIZE; i++) {
            samples[i] = adc_read();
        }
        uint16_t median = median_filter(samples);
        
        _delay_ms(10);
    }
}

Filter Comparison:
┌──────────┬────────────┬────────────┬────────────┐
│Feature   │ IIR        │ FIR        │ Median     │
├──────────┼────────────┼────────────┼────────────┤
│Response  │ Fast       │ Moderate   │ Moderate   │
│CPU Usage │ Very low   │ Low        │ Moderate   │
│Memory    │ 1 value    │ N samples  │ N samples  │
│Phase lag │ Yes        │ Yes        │ No         │
│Spikes    │ Attenuated │ Attenuated │ Rejected   │
└──────────┴────────────┴────────────┴────────────┘
```

---

## Advantages & Disadvantages

### Advantages ✓
- Necessary for analog sensor interfacing
- Built-in to most modern microcontrollers
- Multiple channels available (multiplexed)
- Relatively low cost
- Low power consumption
- Decent resolution (8-16 bits)
- Simple to use with most MCUs

### Disadvantages ✗
- Bit resolution limits accuracy
- Conversion time delays (10-100 µs)
- Affected by power supply noise
- Requires reference voltage stability
- Quantization error (±0.5 LSB)
- Temperature drift over time
- Non-linear conversion (ideal for SAR)

---

## Applications

| Application | Sensor | ADC Bits | Sampling |
|-------------|--------|----------|----------|
| Temperature | TMP36/LM35 | 10 | 10 Hz |
| Pressure | BMP280 | 12 | 50 Hz |
| Light | LDR (photoresistor) | 8-10 | 10 Hz |
| Sound | Microphone | 12-16 | 44 kHz |
| Acceleration | ADXL345 | 10 | 100 Hz |
| Humidity | DHT22 | 10 | 1 Hz |
| Motor Speed | Encoder | 8 | 1 kHz |
| Battery Voltage | Divider | 10 | 10 Hz |

---

## Comparison with Other Protocols

| Feature | ADC | DAC | GPIO |
|---------|-----|-----|------|
| Input/Output | Input (analog) | Output (analog) | Digital I/O |
| Resolution | 8-16 bits | 8-16 bits | 1 bit |
| Speed | 10-100µs | 1-10µs | Instant |
| Applications | Sensors | Audio, PWM | On/Off signals |

---

## References
- Microcontroller Datasheets (AVR, STM32, PIC)
- ADC Application Notes from Manufacturers
- Analog Electronics Textbooks
- Signal Processing DSP documentation
