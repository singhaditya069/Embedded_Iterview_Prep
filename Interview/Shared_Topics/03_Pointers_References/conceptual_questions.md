# Pointers and References - Conceptual Questions

## Pointer Basics
1. Pointer declaration and initialization.
2. The &(address-of) and *(dereference) operators.
3. Null pointers and checking.
4. Void pointers and generic programming.

## Advanced Pointer Concepts
1. Pointers to functions.
2. Pointers to pointers (double indirection).
3. Pointer arithmetic and array indexing.
4. Function pointers in callbacks and virtual functions.

## Pointer Dangers
1. Dangling pointers after free.
2. Null pointer dereference.
3. Buffer overflow through pointers.
4. Use-after-free vulnerabilities.

## References in C++
1. Reference vs. pointer differences.
2. Reference parameters and return.
3. Const references and temporaries.
4. Reference binding rules.

## Smart Pointers in C++
1. unique_ptr for exclusive ownership.
2. shared_ptr for shared ownership.
3. weak_ptr to avoid circular references.
4. RAII and exception safety.

## Memory Safety Techniques
1. Bounds checking.
2. Pointer encryption (for security).
3. Pointer compression for 64-bit systems.
4. Memory tagging extensions (MTE).

## Real-world Scenario Questions

### Scenario 1: Complex Pointer Structures
**Context**: Embedded system with linked lists, trees, and graphs.

**Questions**:
1. How would you safely manage pointers?
2. How would you prevent memory leaks?
3. How would you debug pointer errors?

### Scenario 2: Callback System
**Context**: Implementing event-driven system with callbacks.

**Questions**:
1. How would you store function pointers?
2. How would you ensure type safety?
3. How would you handle cleanup?

