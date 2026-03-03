# RTOS Interview Questions

## Concepts
- Explain the difference between a general-purpose OS and an RTOS.
- What are hard and soft real-time systems?
- Describe task states in an RTOS (ready, running, blocked, suspended).
- What is a context switch? How is it handled in an embedded RTOS?
- Explain priority inversion and how to mitigate it (e.g., priority inheritance).
- What is a scheduler? Compare preemptive vs cooperative scheduling.
- Discuss inter-task communication mechanisms: queues, semaphores, mutexes, event flags.

## Theoretical
- How does an RTOS achieve determinism?
- Explain tickless idle and its advantages.
- What are memory protection and MPU/MPU features in RTOS?
- Describe Kernel objects and their lifecycle.

## Coding
- Write pseudo-code for a periodic task that reads a sensor and sends data over UART using an RTOS.
- Implement a producer-consumer pattern using FreeRTOS queues.
- Demonstrate how to create a mutex and use it to protect a shared resource.
- Show how to configure and start a timer callback in an RTOS.

## MAANG-Level
- Design an RTOS for a multi-core embedded processor with heterogeneous cores.
- Performance analysis of context switching and its impact on latency.
- Debugging a deadlock scenario in a complex task graph.

---
