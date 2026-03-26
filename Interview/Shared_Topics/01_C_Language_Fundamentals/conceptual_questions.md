# C Language Fundamentals - Conceptual Questions

## Data Types and Memory
1. Primitive data types (char, int, float, double).
2. sizeof operator and memory layout.
3. Signed vs. unsigned types.
4. Type casting and promotion rules.

## Pointers
1. Pointer declaration and usage.
2. Pointer arithmetic.
3. Void pointer and casting.
4. Function pointers and callbacks.
5. Pointer pitfalls: dangling pointers, null pointers.

## Arrays and Strings
1. Array declaration and initialization.
2. Multi-dimensional arrays.
3. Array to pointer decay.
4. String manipulation and null-termination.
5. String library functions (strcpy, strlen).

## Structs and Unions
1. Struct layout and packing.
2. Padding and alignment.
3. Unions and overlapping memory.
4. Bit-fields in structs.

## Functions
1. Function declaration vs. definition.
2. Parameter passing (by value, by pointer).
3. Return values and void functions.
4. Function scope and linkage.

## Storage Classes
1. auto, register, static, extern.
2. Global vs. local variables.
3. Static functions for file scope.
4. Lifetime vs. visibility.

## Preprocessor
1. #define and macros.
2. #include for dependencies.
3. Conditional compilation (#ifdef, #ifndef).
4. Predefined macros (__LINE__, __FILE__).

## Memory Layout
1. Stack and heap allocation.
2. Segment layout: text, rodata, data, bss.
3. Stack frame and function calls.
4. Heap fragmentation.

## Real-world Scenario Questions

### Scenario 1: Memory Corruption
**Context**: Embedded system exhibits random crashes due to memory corruption.

**Questions**:
1. How would you identify the culprit pointer?
2. What debugging techniques would you use?
3. How would you implement guards?

### Scenario 2: Performance-critical C Code
**Context**: Need to optimize C code for embedded systems.

**Questions**:
1. What optimizations would you consider?
2. How do inline functions help?
3. What compiler flags would you use?

