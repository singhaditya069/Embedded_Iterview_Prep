# Assembly Language - Conceptual Questions

## Assembly Basics
1. Assembly syntax (AT&T vs. Intel).
2. Instructions, operands, and addressing modes.
3. Registers and their usage conventions.
4. Labels and symbols.

## ARM Assembly
1. ARM instruction format.
2. Common ARM instructions (MOV, LDR, STR, ADD, SUB).
3. Conditional execution.
4. Branch instructions (B, BL).

## Addressing Modes
1. Immediate addressing.
2. Register addressing.
3. Memory addressing (offset, pre-indexed, post-indexed).
4. PC-relative addressing.

## Function Calls
1. Calling conventions (parameter passing, return values).
2. Stack frame organization.
3. Function prologue and epilogue.
4. Calling C from assembly and vice versa.

## Inline Assembly
1. Inline assembly in C/C++.
2. GCC inline syntax (asm keyword).
3. Constraints and operands.
4. Optimization with inline assembly.

## Compiler Generated Assembly
1. Compiler optimization levels and assembly.
2. Register allocation.
3. Loop unrolling and pipelining.
4. Dead code elimination.

## Performance Analysis
1. Instruction timing and throughput.
2. Pipeline stages and stalls.
3. Data hazards and dependencies.
4. Profiling assembly code.

## Real-world Scenario Questions

### Scenario 1: Critical Function Optimization
**Context**: Performance-critical function needs assembly optimization.

**Questions**:
1. How would you profile to find hotspots?
2. How would you write optimized assembly?
3. How would you maintain code readability?
4. What are portability concerns?

### Scenario 2: Platform-specific Code
**Context**: Implementing ARM-specific critical section using assembly.

**Questions**:
1. How would you ensure atomicity?
2. What are instruction ordering issues?
3. How would you handle different ARM variants?

