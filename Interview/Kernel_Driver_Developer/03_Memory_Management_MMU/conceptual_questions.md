# Memory Management and MMU - Conceptual Questions

## Virtual Memory
1. What is virtual memory and its purpose?
2. Virtual address space layout.
3. Page size and page tables.
4. Virtual memory benefits: protection, large address space.

## Memory Management Unit (MMU)
1. How does MMU translate virtual addresses?
2. TLB (Translation Lookaside Buffer) operation.
3. Page table hierarchies (x86, ARM).
4. Context switching and TLB invalidation.

## Page Replacement Algorithms
1. LRU (Least Recently Used) algorithm.
2. FIFO and Clock algorithms.
3. Optimal page replacement.
4. Dirty pages and writeback.

## Memory Protection
1. Page protection bits (read, write, execute).
2. Memory access enforcement.
3. Segmentation vs. paging.
4. SMEP (Supervisor Mode Execution Prevention).

## Kernel Memory Management
1. Slab allocator in Linux kernel.
2. Buddy system for page allocation.
3. vmalloc for virtual memory allocation.
4. DMA allocation and coherency.

## User Memory Management
1. Heap management in user space.
2. Malloc and free implementations.
3. Memory overcommit in Linux.
4. Out-of-memory (OOM) killer.

## Real-world Scenario Questions

### Scenario 1: Memory Leak in Kernel Driver
**Context**: Kernel memory usage increases over time.

**Questions**:
1. How would you detect memory leaks?
2. What tools would you use (kmemleak)?
3. How would you debug kernel allocations?
4. How would you verify the fix?

### Scenario 2: TLB Thrashing
**Context**: Application with poor virtual address locality.

**Questions**:
1. How do you detect TLB thrashing?
2. What are performance implications?
3. How would you optimize memory access patterns?
4. How does huge pages help?

