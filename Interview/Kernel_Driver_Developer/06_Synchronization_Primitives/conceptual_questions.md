# Synchronization Primitives - Conceptual Questions

## Semaphores
1. Binary vs. counting semaphores.
2. Semaphore implementation and down/up operations.
3. Sleep vs. spin during wait.
4. Priority inversion with semaphores.

## Mutexes
1. Mutex vs. semaphore differences.
2. Priority inheritance in mutexes.
3. Recursive mutex implementation.
4. Deadlock prevention with mutexes.

## Spinlocks
1. When to use spinlocks vs. mutexes.
2. Reader-writer spinlocks.
3. Spinlock CPU contention.
4. Nested spinlocks and deadlock.

## Read-Write Locks
1. Reader-writer synchronization.
2. Starvation issues and fairness.
3. Seqlock for frequently read data.
4. Performance of read-write locks.

## Atomic Operations
1. Atomic variable operations (compare-and-swap, compare-and-exchange).
2. Memory ordering (acquire, release, full barrier).
3. Lockless programming.
4. RCU (Read-Copy-Update) synchronization.

## Condition Variables
1. Wait and notify operations.
2. Spurious wakeups.
3. Signaling patterns (signal vs. broadcast).

## Real-world Scenario Questions

### Scenario 1: Deadlock in Multi-core System
**Context**: System occasionally deadlocks under heavy load.

**Questions**:
1. How would you diagnose deadlock?
2. What are common deadlock patterns?
3. How would you design to prevent deadlock?
4. How would you detect and recover from deadlock?

### Scenario 2: Lock Contention Bottleneck
**Context**: Multi-core system has performance degradation due to lock contention.

**Questions**:
1. How would you measure lock contention?
2. What lock-free techniques would you try?
3. How would you implement RCU?
4. How would you validate correctness?

