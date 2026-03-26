# System Calls Interface - Conceptual Questions

## System Call Mechanism
1. How system calls work (user to kernel transition).
2. Hardware support for system calls (interrupts, syscall instructions).
3. Arguments passing conventions (registers, stack).
4. Return value and error handling.

## Common System Calls
1. Process management (fork, exec, exit, wait).
2. File operations (open, read, write, close).
3. Memory operations (brk, mmap, munmap).
4. IPC (pipe, socket, shared memory).

## System Call Performance
1. System call overhead and latency.
2. Minimizing system call frequency.
3. Vsyscall and vDSO optimization.
4. Batch system calls where possible.

## vsyscall and vDSO
1. Virtual dynamic shared object (vDSO) concept.
2. Fast system calls without context switch.
3. Using vDSO for time queries (gettimeofday).
4. Performance benefits.

## ARM System Calls
1. ARM EABI calling conventions.
2. SWI (Software Interrupt) instruction.
3. System call number encoding.
4. ARM specific optimizations.

## Real-time System Calls
1. clock_nanosleep for precise timing.
2. sched_setaffinity for CPU affinity.
3. mlock for memory locking (RAM).
4. Real-time signal handling.

## Real-world Scenario Questions

### Scenario 1: System Call Overhead
**Context**: Profiling shows excessive system call overhead.

**Questions**:
1. How would you analyze system calls?
2. What tools would you use (strace, perf)?
3. How would you batch operations?
4. How would you leverage vDSO?

### Scenario 2: Timing Sensitivity
**Context**: Application needs micro-second timing precision.

**Questions**:
1. How would you implement high-resolution timing?
2. What are vDSO benefits for timing?
3. How would you synchronize with hardware timers?

