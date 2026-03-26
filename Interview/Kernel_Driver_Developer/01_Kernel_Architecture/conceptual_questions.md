# Kernel Architecture - Conceptual Questions

## Kernel Overview
1. What is an operating system kernel?
2. Explain monolithic vs. microkernel architecture.
3. What are kernel responsibilities (process, memory, I/O management)?
4. Kernel vs. user space.

## Bootloader and Kernel Loading
1. How does the bootloader load the kernel?
2. Kernel initialization steps.
3. Device tree loading and parsing.
4. Kernel command line arguments.

## Interrupt and Exception Handling
1. CPU exception handling mechanism.
2. Context saving and restoration.
3. Interrupt nesting and priority.
4. Trap handling for system calls.

## CPU Modes and Privilege Levels
1. Kernel mode vs. user mode.
2. Mode switching and transitions.
3. Privilege level enforcement.
4. Secure vs. non-secure execution.

## Kernel Entry Points
1. What are syscall entry points?
2. Exception vectors and ISRs.
3. Signal handlers in kernel.
4. Traps and their handling.

## Real-world Scenario Questions

### Scenario 1: Kernel Panic Investigation
**Context**: Kernel crashes with panic message.

**Questions**:
1. How would you read panic message?
2. What information does it provide?
3. How would you analyze stack trace?
4. How would you fix the issue?

### Scenario 2: Performance in Kernel Context
**Context**: Frequent context switches causing slowdown.

**Questions**:
1. How would you measure context switch overhead?
2. How would you reduce context switches?
3. How does CPU affinity help?

