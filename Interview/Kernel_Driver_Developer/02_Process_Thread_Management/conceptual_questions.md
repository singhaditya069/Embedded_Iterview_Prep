# Process and Thread Management - Conceptual Questions

## Process Concept
1. What is a process in Linux?
2. Process address space (code, data, heap, stack).
3. Process states and transitions.
4. Process control block (PCB) and task_struct.

## Process Creation
1. fork() system call and copy-on-write.
2. exec() family of system calls.
3. Process creation overhead.
4. POSIX spawn alternative.

## Thread Concept
1. What is a thread and its role?
2. Threads vs. processes: advantages and disadvantages.
3. Thread sharing with processes.
4. Thread-local storage (TLS).

## Process Scheduling
1. Linux scheduler (Completely Fair Scheduler - CFS).
2. Task runqueue and load balancing.
3. Nice and priority levels.
4. Real-time scheduling (SCHED_FIFO, SCHED_RR).

## CPU Affinity
1. Task affinity to CPU cores.
2. CPU set management.
3. Affinity inheritance.
4. Load balancing with affinity.

## Process Synchronization
1. race conditions and mutual exclusion.
2. Semaphores and mutexes in kernel.
3. Spinlocks and their usage.
4. Atomic operations.

## Real-world Scenario Questions

### Scenario 1: Process Hierarchy
**Context**: Complex application with many child processes.

**Questions**:
1. How would you manage process dependencies?
2. How would you handle zombie processes?
3. How would you implement graceful shutdown?

### Scenario 2: Thread Safety Issue
**Context**: Multi-threaded application has race condition.

**Questions**:
1. How would you identify the race condition?
2. What synchronization primitives would you use?
3. How would you validate the fix?

