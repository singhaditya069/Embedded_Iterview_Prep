# ARM Architecture - Conceptual Questions

## ARM Architecture Overview
1. ARM architecture versions (ARMv7, ARMv8, etc.).
2. ARM ISA (Instruction Set Architecture).
3. Thumb-2 instruction set.
4. ARM vs. Thumb-2 trade-offs.

## CPU Modes and Privileges
1. User, Supervisor, System modes.
2. Secure vs. Non-secure execution.
3. Exception levels in ARMv8.
4. Privilege level transitions.

## Registers
1. 16 general-purpose registers (R0-R15).
2. Special registers (SP, LR, PC).
3. Program Status Register (PSR).
4. CPSR and SPSR.

## Memory Management
1. Memory Protection Unit (MPU) in Cortex-M.
2. Memory Management Unit (MMU) in Cortex-A.
3. TLB operation.
4. Cache coherency bits.

## Interrupts and Exceptions
1. Exception types (Reset, Abort, Interrupt).
2. Interrupt handling flow.
3. NVIC (Nested Vectored Interrupt Controller).
4. Exception priority and preemption.

## Cache Architecture
1. L1 instruction and data caches.
2. L2 unified cache.
3. Cache coherency protocols.
4. Cache policies (write-through, write-back).

## SIMD Extensions
1. NEON technology for ARM.
2. Vector operations.
3. Data parallelism in embedded systems.

## Cortex-M Series
1. Cortex-M0, M4, M7 comparison.
2. Feature differences and use cases.
3. Performance characteristics.

## Cortex-A Series
1. Cortex-A53, A72 characteristics.
2. Out-of-order execution.
3. Branch prediction in Cortex-A.

## Real-world Scenario Questions

### Scenario 1: ARM Optimization
**Context**: Optimizing critical code section for ARM Cortex-M4.

**Questions**:
1. What ARM instructions would you use?
2. How do you leverage NEON?
3. What are cache implications?

### Scenario 2: ARMv8 Secure Execution
**Context**: Implementing secure enclave on ARMv8-A.

**Questions**:
1. What are TrustZone features?
2. How do you switch between secure/non-secure?
3. What are shared memory considerations?

