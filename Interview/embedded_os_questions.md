# Embedded OS Interview Questions (Hardware Product Companies)

---

**Q: What is an RTOS? Name some popular RTOSes.**

**A:** An RTOS (Real-Time Operating System) provides deterministic task scheduling and timing guarantees. Examples: FreeRTOS, VxWorks, ThreadX, QNX, Zephyr.

---

**Q: What is a task, and how is it different from a thread?**

**A:** A task is an independent unit of execution managed by the OS. A thread is a lightweight process sharing the same address space. In embedded, the terms are often used interchangeably.

---

**Q: Explain context switching and its overhead.**

**A:** Context switching is saving the state of a running task and loading the state of another. Overhead includes saving/restoring registers, stack pointer, and program counter.

---

**Q: What is priority inversion? How can it be solved?**

**A:** Priority inversion occurs when a high-priority task is blocked by a lower-priority task. Solutions: priority inheritance, priority ceiling protocols.

---

**Q: What is a semaphore? Difference between binary and counting semaphore?**

**A:** A semaphore is a synchronization primitive. Binary semaphore has two states (locked/unlocked); counting semaphore can have a value >1, allowing multiple resources.

---

**Q: How do you implement inter-task communication?**

**A:** Use message queues, mailboxes, event flags, or shared memory with synchronization.

---

**Q: What is a watchdog timer?**

**A:** A hardware or software timer that resets the system if the main program fails to reset it periodically, used to recover from software faults.

---

**Q: How do you implement double buffering in embedded systems?**

**A:** Use two buffers: one is filled by an ISR or DMA, the other is processed by the main loop. Swap when one is full.

---

<!-- Add more embedded OS questions as needed -->
