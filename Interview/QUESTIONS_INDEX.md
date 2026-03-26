# Interview Questions Index - Quick Reference

## How to Use This Index

This document provides quick access to major interview questions organized by topic. 
Use Ctrl+F to search for specific keywords to find relevant questions.

---

## C++ EMBEDDED DEVELOPER TOPICS

### C++ Core Concepts
**RAII & Resource Management:**
- What is RAII and why is it critical in embedded systems?
- Explain the concept of acquiring resources in constructors and releasing in destructors
- How does RAII prevent memory leaks in embedded systems?
- Provide examples of RAII for GPIO, ADC, and SPI interfaces

**Smart Pointers:**
- Explain differences between unique_ptr, shared_ptr, and weak_ptr
- When should you use smart pointers in embedded systems?
- What are overhead implications in resource-constrained environments?
- How does move semantics reduce smart pointer overhead?

**Const Correctness:**
- Why is const correctness important in embedded systems?
- Explain const, constexpr, volatile, and mutable keywords
- What is const_cast and its implications?
- How do you ensure thread-safety using const?

**Template Metaprogramming:**
- What are compile-time computations using templates?
- Explain template specialization and use cases
- How can variadic templates optimize embedded code?

**Virtual Functions & Polymorphism:**
- When should you use virtual functions in embedded systems?
- What is vtable overhead and how does it affect real-time performance?
- How do you implement polymorphism efficiently?
- Explain interface-based design in embedded systems

**Move Semantics:**
- What is an rvalue reference and why is it useful?
- How do move semantics improve performance?
- When should you implement move semantics explicitly?

### RTOS and Real-Time Systems
**Fundamentals:**
- What is RTOS and key characteristics?
- Differentiate hard, firm, and soft real-time systems
- Explain determinism and predictability in RTOS
- What are main RTOS components?

**Task Management:**
- What is a task/thread and how differs from process?
- Explain task states: ready, running, blocked, suspended
- What is context switching and its overhead?
- How does preemption differ from non-preemption?

**Scheduling:**
- Explain Rate Monotonic Scheduling (RMS)
- What is Earliest Deadline First (EDF) scheduling?
- Compare priority-based vs time-sliced scheduling
- What is priority inversion and how to prevent?

**Synchronization:**
- What are mutexes and semaphores; how do they differ?
- Explain binary vs counting semaphores
- What is recursive mutex and when to use?
- What are condition variables and how are they used?

**Deadlock & Priority Inversion:**
- What is deadlock and necessary conditions?
- Explain Coffman's four conditions for deadlock
- How does priority inheritance prevent deadlock?
- How do you detect deadlocks?

**Timers & Resource Management:**
- Explain software timers in RTOS
- What is difference between periodic and one-shot timers?
- How do you achieve precise timing in RTOS?
- How do you implement watchdog timers?

### Memory Management
**Memory Hierarchy:**
- Explain memory hierarchy (registers, cache, SRAM, DRAM, flash)
- How does cache affect real-time performance?
- What is cache coherency in multi-core systems?

**Stack vs Heap:**
- Explain stack and heap memory allocation
- What is stack overflow and prevention?
- Advantages/disadvantages of heap in embedded?
- How do you estimate stack usage?

**Allocation Strategies:**
- What is static memory allocation and why preferred?
- What are challenges of dynamic memory allocation?
- What are memory pools and slab allocators?
- How do you implement free-list allocator?

**Fragmentation:**
- What is external and internal fragmentation?
- How to diagnose memory fragmentation?
- What are defragmentation strategies?
- Why is fragmentation critical in long-running systems?

**Memory Protection & DMA:**
- What is MMU and its role?
- Explain virtual memory in embedded
- What are Memory Protection Units (MPU)?
- What are DMA and cache coherency issues?

### Hardware Interface
**Digital Logic:**
- What are combinational vs sequential circuits?
- Explain logic gates and flip-flops
- What are multiplexers and decoders?

**Microcontroller Architecture:**
- Difference between microcontroller and microprocessor
- Explain Von Neumann and Harvard architectures
- What is fetch-execute cycle?
- Explain instruction pipelining benefits

**Registers & Memory-Mapped I/O:**
- What are CPU registers and their types?
- How does memory-mapped I/O work?
- Port-mapped I/O vs memory-mapped I/O?
- What is purpose of control registers?

**Interrupt System:**
- What is interrupt and how differs from polling?
- Explain interrupt priorities and nesting
- What is Interrupt Service Routine (ISR)?
- How does interrupt masking work?

**Communication Protocols Overview:**
- Serial vs parallel communication
- Synchronous vs asynchronous
- What is baud rate and calculation?
- Overview of UART, SPI, I2C

**Timers & PWM:**
- What are timer/counter modules?
- Explain PWM (Pulse Width Modulation)
- What are capture and compare functions?
- How to measure frequency/period?

**ADC:**
- What is ADC and its role?
- Explain sampling, quantization, resolution
- What is Nyquist theorem?
- Explain ADC architectures

### Communication Protocols
**UART:**
- How does UART asynchronous communication work?
- Explain start bit, data bits, parity, stop bits
- How is baud rate achieved?
- Typical configurations and buffer overflow handling?

**SPI:**
- Explain SPI protocol
- What are Master, Slave, MOSI, MISO roles?
- What are SPI modes (CPOL, CPHA)?
- How does SPI differ from UART?

**I2C:**
- How does I2C two-wire communication work?
- Explain open-drain and pull-up resistors
- What is I2C clock stretching?
- What are I2C speed modes?

**CAN:**
- What is CAN and why used in automotive?
- Explain CAN message structure
- What is arbitration in CAN?
- How does CAN error handling work?

### System Design & Architecture
**Architecture Patterns:**
- Layered vs modular architecture
- Explain bootloader and firmware organization
- What is Hardware Abstraction Layer (HAL)?
- How to organize code for maintainability?

**State Machines:**
- What are Finite State Machines (FSM)?
- How to implement FSM efficiently?
- What are state machine design patterns?
- How to handle complexity?

**Design Patterns:**
- Observer pattern and event handling
- Strategy pattern for algorithms
- Factory pattern for creation
- Singleton pattern pitfalls
- Template Method pattern

**Real-time Architecture:**
- How to design with hard real-time constraints?
- What is jitter minimization?
- Latency analysis and optimization
- Predictable resource management

### Debugging & Testing
**Debugging Techniques:**
- What debugging tools exist (GDB, J-Link)?
- Explain step execution, breakpoints, watchpoints
- Debug without debugger (logging, assertions)?
- Remote debugging in embedded?

**Testing Strategies:**
- Unit testing in embedded
- Integration testing approach
- Hardware-in-the-loop (HIL) testing
- Software-in-the-loop (SIL) testing

**Logging & Analysis:**
- How to implement efficient logging?
- Logging levels and filtering
- Circular buffer logging
- System tracing and profiling

### Performance Optimization
**Profiling:**
- How to measure execution time?
- What are CPU cycles metrics?
- How to measure code coverage?

**Code Optimization:**
- Loop unrolling trade-offs
- Function inlining impact
- Compiler optimization levels
- Tight loop optimization

**Algorithm & System:**
- Algorithm selection for constraints
- Time-space trade-offs
- Lookup tables vs computation
- Jitter reduction techniques

---

## FIRMWARE DEVELOPER TOPICS

### Hardware Fundamentals
**Electrical:**
- Voltage, current, resistance, power relationships
- Ohm's law applications
- Circuit analysis
- Ground plane and signal integrity

**Digital Electronics:**
- Logic levels (TTL, CMOS)
- Fan-in and fan-out
- Pull-up and pull-down resistors
- Open-drain outputs

**Analog Circuits:**
- Analog signals and characteristics
- Operational amplifiers
- Filters (low-pass, high-pass)
- Analog/Digital conversion

**Power & Oscillators:**
- Voltage regulators and LDOs
- Decoupling capacitors
- Crystal oscillators
- PLL basics

### Microcontroller Programming
**Register Programming:**
- Reading/writing microcontroller registers
- Bit fields usage
- Volatile keyword with registers
- Register permissions

**GPIO:**
- GPIO pin configuration (input, output, AF)
- GPIO modes (push-pull, open-drain)
- GPIO interrupt implementation
- Button debouncing

**Timer/Counter:**
- Timer initialization
- Different timer modes (normal, CTC, PWM)
- PWM signal generation
- Timer interrupts

**ADC:**
- ADC configuration
- Single-ended vs differential
- ADC interrupt vs polling
- DMA for ADC

**Serial Communication:**
- UART initialization
- UART interrupt handling
- Circular buffers for UART
- Flow control (CTS/RTS)

**Interrupts:**
- Interrupt priority configuration
- Interrupt enable/disable
- Critical sections
- Nested interrupts

### STM32 Specific
**Architecture:**
- STM32 family variants
- Key STM32 features
- Clock system (HSE, HSI, PLL)
- Flash and RAM organization

**HAL vs LL Drivers:**
- Hardware Abstraction Layer basics
- Low-Level drivers
- When to use HAL vs LL
- STM32CubeMX for code generation

**Clock Configuration:**
- Clock source configuration
- PLL configuration
- Peripheral clock gating
- Clock distribution

**Peripherals:**
- GPIO port organization
- UART/USART configuration
- SPI master/slave setup
- I2C communication
- ADC and DMA
- Timer and PWM

**Interrupt & DMA:**
- EXTI (External Interrupt) configuration
- NVIC (Interrupt Controller)
- DMA channels and usage
- DMA priority

**Power Management:**
- Power modes (Sleep, Stop, Standby)
- Wake-up sources
- Power consumption optimization
- LDO and SMPS modes

### Boot Process & BSP
**Bootloader:**
- Bootloader role and process
- Boot from ROM, Flash, external memory
- Two-stage bootloading
- Bootloader vs application organization

**Start Sequence:**
- ARM reset vector
- Power-on reset process
- CPU mode initialization
- Stack pointer initialization

**Vector Table:**
- Interrupt vector table purpose
- Vector table address setup
- Vector table relocation
- Custom handlers

**Memory Initialization:**
- Stack and heap initialization
- Data segment initialization
- BSS section clearing
- Constructor/destructor calls

**Firmware Organization:**
- Application sections (text, rodata, data, bss)
- Linker script organization
- Flash and RAM layout
- Size optimization

### Device Drivers & HAL
**Driver Architecture:**
- Character vs block drivers
- Device driver interfaces
- Driver lifecycle
- Platform-specific implementations

**HAL Abstraction:**
- HAL purpose and design
- API design principles
- Balancing abstraction and performance
- Platform-specific implementations

**Peripheral Drivers:**
- Serial driver implementation
- Timer driver with modes
- ADC driver with calibration
- GPIO driver
- I2C and SPI drivers
- DMA controller driver

**State Management:**
- Device states
- State transitions
- Error condition handling

**Performance:**
- Zero-copy data transfers with DMA
- Interrupt-driven vs polling
- Buffer management
- ISR latency minimization

### Board Bringup
**Initial Testing:**
- Power-on sequence
- Clock verification
- RAM testing
- Peripheral verification
- Calibration

**Bare Metal:**
- Bare metal programming definition
- Super-loop architecture
- Interrupt-driven architecture
- Minimal startup code
- CPU register initialization

**Hardware Testing:**
- Power supply verification
- Clock signal verification
- Reset sequence testing
- Communication interface testing

**Debugging Bare Metal:**
- Serial logging for diagnostics
- LED status indication
- JTAG debugging
- Post-mortem debugging

### RTOS Integration
**Selection:**
- FreeRTOS features
- When to choose FreeRTOS
- Licensing and support
- Resource requirements

**FreeRTOS:**
- FreeRTOS porting
- Tick timer configuration
- Context switch implementation
- ISR handling

**RTOS Configuration:**
- Task stack sizing
- Heap configuration
- Timer resolution
- Priority levels

**Firmware Modularization:**
- Task-based modularization
- Service-oriented architecture
- Configuration management

### Peripheral Interfaces
**UART:**
- UART frame format
- Timing and baud rate
- Flow control (hardware/software)
- Error detection

**SPI:**
- Transmission modes (CPOL, CPHA)
- Chip select (CS) timing
- Clock frequency constraints
- SPI peripherals

**I2C:**
- I2C protocol and timing
- Master/slave addressing
- 7-bit vs 10-bit addressing
- Clock stretching
- Multi-master arbitration

**CAN:**
- CAN frame structure and arbitration
- Message ID and filtering
- Timeout and error handling
- CAN FD protocol

**Memory Interfaces:**
- SRAM interface and timing
- Serial flash (NOR) interface
- NAND flash interface
- Cache coherency

### Debugging Tools
**Interfaces:**
- JTAG protocol basics
- SWD (Serial Wire Debug) protocol
- SEGGER J-Link and debuggers
- OpenOCD

**GDB Usage:**
- GDB basics (step, breakpoint, watchpoint)
- Remote GDB debugging
- Post-mortem debugging
- Symbol table and debug info

**Tools & Analysis:**
- Logic analyzers
- Oscilloscopes
- CAN/SPI bus analyzers
- Power measurement

**Testing Frameworks:**
- Unit testing for embedded
- Mocking hardware
- Test coverage
- CI for firmware

---

## KERNEL & DRIVER DEVELOPER TOPICS

### Kernel Architecture
**Fundamentals:**
- What is kernel and responsibilities?
- Monolithic vs microkernel
- Kernel vs user space
- Kernel initialization steps

**Boot & Load:**
- How bootloader loads kernel?
- Kernel initialization
- Device tree loading
- Kernel command line

**Interrupt System:**
- CPU exception handling
- Context saving
- Interrupt nesting
- Trap handling for syscalls

**Modes & Privileges:**
- Kernel mode vs user mode
- Mode switching
- Privilege enforcement
- Secure vs non-secure execution

### Process & Thread Management
**Process Concept:**
- What is process in Linux?
- Process address space
- Process states
- Process control block (PCB)

**Creation:**
- fork() system call and COW
- exec() family
- Creation overhead
- POSIX spawn

**Threads:**
- What is thread and role?
- Threads vs processes
- Thread sharing with processes
- Thread-local storage (TLS)

**Scheduling:**
- Linux Completely Fair Scheduler (CFS)
- Task runqueue
- Nice and priority levels
- Real-time scheduling (SCHED_FIFO, SCHED_RR)

**CPU Affinity:**
- Task affinity to cores
- CPU set management
- Affinity inheritance
- Load balancing

**Synchronization:**
- Race conditions and mutual exclusion
- Semaphores and mutexes
- Spinlocks
- Atomic operations

### Memory Management & MMU
**Virtual Memory:**
- Virtual memory purpose
- Virtual address space layout
- Page size and tables
- Benefits of virtual memory

**MMU:**
- MMU virtual address translation
- TLB (Translation Lookaside Buffer)
- Page table hierarchies
- Context switching and TLB

**Page Replacement:**
- LRU algorithm
- FIFO and Clock algorithms
- Optimal replacement
- Dirty pages and writeback

**Protection:**
- Page protection bits
- Memory access enforcement
- Segmentation vs paging
- SMEP (Supervisor Mode Execution Prevention)

**Kernel Memory:**
- Slab allocator
- Buddy system
- vmalloc for virtual memory
- DMA allocation and coherency

**User Memory:**
- Heap management
- Malloc and free
- Memory overcommit
- Out-of-memory killer

### Device Driver Development
**Character Drivers:**
- Character device basics
- Major/minor device numbers
- File operations (open, read, write, close)
- ioctl system call

**Block Drivers:**
- Block device characteristics
- Block I/O and batching
- Request queue and scheduling
- Filesystem interactions

**Linux Device Model:**
- Device, driver, bus abstractions
- kobject and reference counting
- sysfs interface
- Device tree binding

**Probe & Removal:**
- probe() function
- Module init/exit
- Device power management
- Hot plug support

**Memory Sharing:**
- Mapping kernel memory to user space (mmap)
- DMA buffers and coherency
- Memory sharing patterns
- Security issues

**Concurrency:**
- Mutual exclusion (mutexes, semaphores)
- Spinlocks
- Per-CPU variables
- Atomic operations

### Interrupt & Exception Handling
**Mechanism:**
- Interrupt sources and priorities
- Handler registration
- Interrupt sharing
- Interrupt affinity

**IRQ Flow:**
- Top-half (hardirq) vs bottom-half
- deferred_work (tasklets, workqueues)
- Interrupt vs process context
- Interrupt disable impact

**Exceptions:**
- Processor exceptions
- Page faults
- Invalid instruction
- Protection faults

**Nesting:**
- Interrupt nesting and re-entrance
- Preventing nesting
- Critical sections in handlers

**Performance:**
- Interrupt latency analysis
- Critical section minimization
- Interrupt coalescing
- MSI vs pin-based

### Synchronization Primitives
**Semaphores:**
- Binary vs counting
- Implementation
- Sleep vs spin
- Priority inversion

**Mutexes:**
- Mutex vs semaphore
- Priority inheritance
- Recursive mutex
- Deadlock prevention

**Spinlocks:**
- When to use spinlocks
- Reader-writer spinlocks
- CPU contention
- Deadlock prevention

**Atomic & Lock-free:**
- Atomic variable operations
- CAS (Compare-And-Swap)
- Memory ordering
- RCU (Read-Copy-Update)

**Condition Variables:**
- Wait and notify
- Spurious wakeups
- Signal vs broadcast

### System Calls Interface
**Mechanism:**
- User to kernel transition
- Hardware support
- Argument passing
- Return value and error handling

**Common Syscalls:**
- Process (fork, exec, exit, wait)
- Files (open, read, write, close)
- Memory (brk, mmap, munmap)
- IPC (pipe, socket, shared memory)

**Performance:**
- Syscall overhead
- Frequency minimization
- Batch syscalls
- Vsyscall and vDSO

**Real-time:**
- clock_nanosleep
- sched_setaffinity
- mlock
- Real-time signals

### File System
**Fundamentals:**
- File system types
- Inode and dentry
- File metadata
- Hierarchy and mount points

**VFS:**
- Virtual File System abstraction
- File and inode operations
- Superblock and registration
- Mounting/unmounting

**I/O Operations:**
- Read and write on files
- File position and seeking
- Buffering and page cache
- Direct I/O

**Journaling:**
- Journaling for reliability
- Ext4 modes (journal, ordered, writeback)
- Crash consistency
- Performance vs reliability

**Security:**
- POSIX permissions
- Setuid and setgid bits
- Sticky bit
- ACLs

### Networking Stack
**OSI in Linux:**
- OSI layer implementation
- Protocol families
- Network subsystem

**Sockets:**
- Socket creation and binding
- TCP and UDP
- Blocking vs non-blocking
- Async I/O (select, poll, epoll)

**TCP/IP:**
- IP layer (routing, fragmentation)
- TCP (connection, flow control)
- UDP (connectionless)
- ICMP

**Network Driver:**
- Network device operations
- Packet transmission/reception
- DMA for packets
- Interrupt handling

**QoS & Real-time:**
- Traffic shaping and policing
- Priority queuing
- TSN (Time Sensitive Networking)

### Embedded Linux Kernel
**Characteristics:**
- Resource-constrained devices
- Minimal kernel build
- Kernel size optimization
- Memory footprint reduction

**Device Tree:**
- Device tree structure
- Device tree compilation
- Bindings
- Runtime modifications

**U-Boot Bootloader:**
- U-Boot features
- Configuration and scripting
- Kernel loading
- Boot media options

**Kernel Configuration:**
- Menuconfig
- Feature minimization
- Static vs dynamic modules
- Critical drivers

**Real-time Linux:**
- PREEMPT_RT patch
- Deterministic scheduling
- Capabilities and limitations
- Tracing real-time systems

**Root Filesystem:**
- busybox utilities
- Buildroot
- Yocto Project
- Init scripts

---

## SHARED FUNDAMENTAL TOPICS

### C Language Fundamentals
**Data Types:**
- Primitive types
- sizeof operator
- Signed vs unsigned
- Type casting and promotion

**Pointers:**
- Pointer declaration and usage
- Arithmetic and dereferencing
- Void pointers
- Function pointers

**Arrays & Strings:**
- Declaration and initialization
- Multi-dimensional arrays
- String manipulation
- Null-termination

**Structs & Unions:**
- Struct layout and packing
- Padding and alignment
- Unions and overlapping memory
- Bit-fields

**Functions:**
- Declaration vs definition
- Parameter passing
- Return values
- Scope and linkage

**Storage Classes:**
- auto, register, static, extern
- Global vs local
- Lifetime vs visibility

**Preprocessor:**
- #define and macros
- #include
- Conditional compilation
- Predefined macros

**Memory Layout:**
- Stack and heap
- Segments (text, rodata, data, bss)
- Stack frame
- Heap fragmentation

### Bit Manipulation
**Operators:**
- AND, OR, XOR, NOT
- Shift operators
- Operator precedence
- Common patterns

**Masking & Fields:**
- Creating masks
- Extracting bits
- Setting/clearing bits
- Rotating bits

**Tricks:**
- Power of 2 checking
- Popcount (set bits)
- MSB/LSB finding
- Gray code conversion

**Embedded Applications:**
- Register configuration
- Status flags
- Boolean packing
- CRC calculation

### Pointers & References
**Basics:**
- Pointer declaration and initialization
- Address-of and dereference
- Null pointers
- Void pointers

**Advanced:**
- Function pointers
- Pointers to pointers
- Pointer arithmetic
- Callbacks

**Dangers:**
- Dangling pointers
- Null dereference
- Buffer overflow
- Use-after-free

**C++ References:**
- Reference vs pointer
- Reference parameters
- Const references
- Reference binding

**Smart Pointers:**
- unique_ptr
- shared_ptr
- weak_ptr
- RAII

**Safety:**
- Bounds checking
- Pointer encryption
- Memory tagging (MTE)

### Data Structures & Algorithms
**Structures:**
- Array, linked list, stack, queue
- Trees (binary, BST, balanced)
- Hash tables
- Graphs

**Sorting:**
- Bubble, insertion, selection
- Quick sort, merge sort, heap sort
- Complexity analysis
- Stability

**Searching:**
- Linear and binary search
- Tree algorithms
- Hash lookups
- BFS/DFS

**Complexity:**
- Big-O notation
- Best/average/worst case
- Time-space trade-offs
- Practical performance

**Graphs:**
- Dijkstra's shortest path
- BFS/DFS traversal
- Topological sorting
- MST (Kruskal, Prim)

**Dynamic Programming:**
- Memoization and tabulation
- Fibonacci and variants
- Knapsack problems
- Optimal substructure

**Embedded Algorithms:**
- Real-time constraints
- Fixed-point arithmetic
- Approximation algorithms
- Online algorithms

### OOP Concepts
**Fundamentals:**
- Classes and objects
- Encapsulation
- Inheritance
- Polymorphism

**Classes:**
- Members and methods
- Constructors/destructors
- Copy constructor assignment
- this pointer

**Inheritance:**
- Base and derived classes
- Method overriding
- Multiple inheritance
- Protected members

**Polymorphism:**
- Virtual functions
- Abstract classes
- Static vs dynamic dispatch
- Covariance

**Design Patterns:**
- Creational (Singleton, Factory)
- Structural (Adapter, Decorator)
- Behavioral (Observer, Strategy)
- Embedded patterns

**Embedded OOP:**
- Virtual function overhead
- Exception handling
- Abstraction vs performance
- When to avoid OOPS

**Best Practices:**
- Const correctness
- RAII
- Move semantics
- Template metaprogramming

### ARM Architecture
**Overview:**
- Architecture versions
- ARM ISA
- Thumb-2 instruction set
- Trade-offs

**Registers:**
- General-purpose registers (R0-R15)
- Special registers (SP, LR, PC)
- PSR
- CPSR and SPSR

**Modes:**
- User, Supervisor, System
- Secure vs non-secure
- Exception levels
- Privilege transitions

**Memory:**
- MPU (Cortex-M)
- MMU (Cortex-A)
- TLB operation
- Cache coherency

**Interrupts:**
- Exception types
- NVIC
- Priority and preemption

**Cache:**
- L1 I-cache and D-cache
- L2 unified cache
- Coherency protocols
- Write policies

**Series:**
- Cortex-M variants comparison
- Cortex-A characteristics
- Feature differences
- Performance characteristics

### Assembly Language
**Basics:**
- Syntax (AT&T vs Intel)
- Instructions and operands
- Addressing modes
- Labels and symbols

**ARM Assembly:**
- Instruction format
- Common instructions (MOV, LDR, STR, ADD)
- Conditional execution
- Branch instructions

**Addressing Modes:**
- Immediate
- Register
- Memory (offset, pre/post-indexed)
- PC-relative

**Functions:**
- Calling conventions
- Parameter passing
- Stack frame
- Prologue and epilogue

**Inline Assembly:**
- Inline assembly in C/C++
- GCC asm syntax
- Constraints and operands
- Optimization with inline

**Compiler Output:**
- Optimization levels
- Register allocation
- Loop unrolling
- Dead code elimination

**Performance:**
- Instruction timing
- Pipeline stages
- Data hazards
- Profiling assembly

---

## Quick Search Tips

Use Ctrl+F to search for:
- **"What is"** - Definition questions
- **"How to"** - Implementation questions
- **"Explain"** - Conceptual questions
- **"Compare"** - Comparison questions
- **"Trade-off"** - Design decisions
- **"Performance"** - Optimization questions
- **"Debug"** - Troubleshooting questions

---

## Most Critical Questions (Must Know)

**Top 10 Questions Everyone Should Know:**

1. **Explain concurrency control** (mutexes, semaphores, atomics)
2. **Design memory management system** (heap, stack, pools)
3. **Handle real-time constraints** (scheduling, latency)
4. **Debug complex issues** (tools, techniques, strategies)
5. **Port driver to new platform** (abstraction, HAL)
6. **Optimize performance** (profiling, hotspots)
7. **Manage power consumption** (strategies, trade-offs)
8. **Implement RTOS task** (design, synchronization)
9. **Analyze and prevent deadlock** (detection, recovery)
10. **Design fault-tolerant system** (error handling, recovery)

---

**End of Index. Good luck with your interviews!** 🚀

