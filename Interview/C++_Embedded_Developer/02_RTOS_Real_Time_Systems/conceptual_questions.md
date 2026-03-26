# RTOS and Real-Time Systems - Conceptual Questions

## RTOS Fundamentals
1. What is a Real-Time Operating System (RTOS) and what are its key characteristics?
2. Differentiate between Hard, Firm, and Soft real-time systems with examples.
3. Explain determinism and predictability in RTOS.
4. What are the main components of an RTOS?

## Task and Process Management
1. What is a task/thread and how is it different from a process?
2. Explain task states: ready, running, blocked, suspended.
3. What is context switching and what is its overhead?
4. How does preemption differ from non-preemption scheduling?
5. What are task priorities and how do they affect scheduling?

## Scheduling Algorithms
1. Explain Rate Monotonic Scheduling (RMS) algorithm.
2. What is Earliest Deadline First (EDF) scheduling?
3. Compare priority-based scheduling vs. time-sliced scheduling.
4. What is priority inversion and how do you prevent it?
5. Explain the concept of schedulability analysis.

## Synchronization Primitives
1. What are mutexes and semaphores? How do they differ?
2. Explain binary vs. counting semaphores.
3. What is a recursive mutex and when would you use it?
4. What are condition variables and how are they used?
5. Explain the difference between spin-locks and semaphores.

## Inter-task Communication (ITC)
1. What are message queues and how do they work?
2. Explain mailboxes and message passing in RTOS.
3. What are the advantages of message passing over shared memory?
4. How do you implement producer-consumer pattern in RTOS?
5. What is priority inheritance in message passing?

## Deadlock Prevention
1. What is a deadlock and what are the necessary conditions for it?
2. Explain the four conditions for deadlock (Coffman conditions).
3. How does priority inheritance prevent deadlock?
4. What are resource allocation strategies to prevent deadlock?
5. How do you detect deadlocks in RTOS applications?

## Memory Management in RTOS
1. Why is dynamic memory allocation problematic in real-time systems?
2. What are static memory allocation strategies in RTOS?
3. Explain memory pools and their advantages.
4. What is cache coherence and how does it affect real-time performance?
5. How do you manage memory fragmentation in embedded RTOS?

## Interrupt Handling in RTOS
1. How does an RTOS handle interrupts?
2. What is an ISR (Interrupt Service Routine) and what are its constraints?
3. Explain the difference between ISR and deferred interrupt handling.
4. What is the interrupt latency and how does it affect real-time performance?
5. How do you protect shared resources in interrupt handlers?

## Timer Management
1. Explain software timers in RTOS.
2. What is the difference between periodic and one-shot timers?
3. How do you achieve precise timing in an RTOS?
4. What is timer resolution and overflow handling?
5. How do you implement watchdog timers in RTOS?

## Real-time Analysis
1. What is worst-case execution time (WCET) analysis?
2. How do you perform schedulability analysis?
3. Explain CPU utilization in real-time systems.
4. What is jitter and how does it affect real-time performance?
5. How do you measure latency in embedded RTOS?

## Common RTOS Platforms
1. Explain the architecture of FreeRTOS.
2. What are QNX real-time operating system characteristics?
3. Describe VxWorks RTOS features.
4. What are the differences between Linux RTOS and hard RTOS?
5. How do you choose between different RTOS platforms?

## Resource Management
1. How do you manage CPU affinity in multi-core systems?
2. Explain the difference between active and passive resources.
3. What is resource reservation in RTOS?
4. How do you implement resource sealing in RTOS?
5. What are the best practices for resource management?

## Real-world Scenario Questions

### Scenario 1: Multi-task Sensor System
**Context**: An autonomous vehicle needs to read data from 10 different sensors at different rates. Some sensors require 1ms response time (lidar, camera), others can tolerate 100ms (GPS, CAN).

**Questions**:
1. How would you design the task hierarchy?
2. What scheduling algorithm would you choose?
3. How would you prevent priority inversion between critical and non-critical tasks?
4. How would you handle inter-task communication efficiently?
5. What synchronization mechanisms would you use?

### Scenario 2: Deadline Miss Handling
**Context**: A control system task misses its deadline due to unpredictable interrupt handling.

**Questions**:
1. How would you diagnose the cause of the deadline miss?
2. What is the impact on the system output?
3. How would you redesign the system to meet hard real-time constraints?
4. What are the recovery mechanisms you would implement?

### Scenario 3: Multi-core Real-time System
**Context**: A dual-core embedded system running different RTOS tasks on each core.

**Questions**:
1. How would you achieve synchronization between cores?
2. What is cache coherency and how does it affect real-time performance?
3. How would you migrate a task from one core to another?
4. What is the worst-case inter-processor latency?

