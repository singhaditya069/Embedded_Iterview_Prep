# Bit Manipulation and Bitwise Operations - Conceptual Questions

## Bitwise Operators
1. AND (&), OR (|), XOR (^), NOT (~).
2. Left shift (<<) and right shift (>>).
3. Operator precedence.
4. Common patterns (bit testing, setting, clearing).

## Bit Masking
1. Creating masks for bit fields.
2. Extracting bits.
3. Setting and clearing specific bits.
4. Rotating bits.

## Bit Manipulation Tricks
1. Checking if a number is power of 2.
2. Counting set bits (popcount).
3. Finding most/least significant bit.
4. Gray code and binary conversion.

## Applications in Embedded
1. Register configuration using bit masking.
2. Status flags manipulation.
3. Efficient boolean packing.
4. CRC and checksum calculation.

## Performance Considerations
1. Shift vs. division/multiplication.
2. Lookup tables for bit operations.
3. Assembly optimization for bit ops.
4. Hardware bit manipulation instructions.

## Real-world Scenario Questions

### Scenario 1: Complex Register Configuration
**Context**: Configuring peripheral with multiple control bits.

**Questions**:
1. How would you safely manipulate bits?
2. What techniques prevent errors?
3. How would you verify configuration?

### Scenario 2: Efficient Data Packing
**Context**: Storing multiple boolean flags in limited memory.

**Questions**:
1. How would you pack bits efficiently?
2. How do you know which bit is which?
3. What about thread safety?

