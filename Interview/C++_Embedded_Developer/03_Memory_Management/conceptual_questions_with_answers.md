# Memory Management - Comprehensive Q&A

## Dynamic Memory Allocation

### Q1: malloc/free and memory leaks
**Answer:**
```c
// malloc - allocate memory on heap
int *ptr = (int *)malloc(sizeof(int) * 100);  // 400 bytes
if (ptr == NULL) {
    printf("Allocation failed\n");
    return;
}

// Use the memory
ptr[0] = 42;
ptr[99] = 99;

// MUST free when done
free(ptr);
ptr = NULL;  // Good practice - prevents use-after-free

// Memory leak example:
void leak_function(void) {
    int *leak = (int *)malloc(1024);
    return;  // LEAK! Forgot free()
}

// Fixed version:
void no_leak_function(void) {
    int *data = (int *)malloc(1024);
    if (data == NULL) {
        printf("Allocation failed\n");
        return;
    }
    
    // Use data...
    
    free(data);
    data = NULL;
}
```

**Common patterns to avoid:**
```c
// Pattern 1: Lost reference
int *p1 = malloc(100);
int *p2 = malloc(200);
p1 = p2;  // p1's original allocation lost (LEAK!)
free(p1);
free(p2);  // Frees p2 twice - ERROR

// Pattern 2: Conditional leak
if (condition) {
    int *temp = malloc(100);
    return;  // LEAK if condition true
}

// Pattern 3: Exception (C doesn't have try/catch)
int *data = malloc(1000);
if (validate(data) == ERROR) {
    return;  // LEAK!
}
```

---

### Q2: calloc vs malloc
**Answer:**
```c
// malloc: Uninitialized (may contain garbage)
int *arr = (int *)malloc(10 * sizeof(int));
// arr[0] contains unpredictable value

// calloc: Allocated AND zero-initialized
int *arr2 = (int *)calloc(10, sizeof(int));
// arr2[0] == 0, all elements zero

// Equivalent but different APIs:
// malloc(total_size)
int *m = (int *)malloc(10 * sizeof(int));

// calloc(num_items, item_size)
int *c = (int *)calloc(10, sizeof(int));

// calloc advantages:
// 1. Automatic zero initialization (safer for data)
// 2. Overflow protection (checks num_items * size)

// Slightly slower due to memset operation
// So use malloc if you'll initialize anyway
```

---

### Q3: realloc - resizing memory
**Answer:**
```c
// Resize existing allocation
int *buffer = (int *)malloc(10 * sizeof(int));
buffer[0] = 42;

// Need more space
int *new_buffer = (int *)realloc(buffer, 20 * sizeof(int));
if (new_buffer == NULL) {
    printf("Realloc failed\n");
    free(buffer);  // Original still allocated
    return;
}

buffer = new_buffer;  // Use resized buffer
buffer[15] = 100;

free(buffer);

// DANGEROUS PATTERN:
ptr = realloc(ptr, 1000);  // If realloc fails, ptr = NULL
// Original allocation lost!

// CORRECT pattern:
void *temp = realloc(ptr, 1000);
if (temp == NULL) {
    printf("Resize failed\n");
    free(ptr);  // Free original
    return;
}
ptr = temp;
```

---

## Memory Layout and Objects

### Q1: Stack vs Heap allocation tradeoffs
**Answer:**
```
STACK:
✓ Automatic allocation/deallocation
✓ Very fast (just pointer increment)
✓ No fragmentation
✗ Limited size (typically 1-8 MB)
✗ Fixed at compile time

Typical stack allocation:
void calculate(void) {
    int local_array[100];      // 400 bytes on stack
    int result;
    // Automatically destroyed at return
}

HEAP:
✓ Dynamic sizing (runtime)
✓ Large available memory (rest of RAM)
✓ Persistent across function calls
✗ Slower (fragmentation possible)
✗ Manual management required
✗ Risk of leaks

Typical heap allocation:
int *data = malloc(100 * sizeof(int));
// Exists until explicitly freed
```

**Memory usage example:**
```c
#include <stdio.h>
#include <stdlib.h>

void memory_analysis(void) {
    // Stack
    int local = 10;                    // 4 bytes, auto
    char buffer[1000];                 // 1000 bytes, auto
    
    // Heap
    int *heap_array = malloc(10 * sizeof(int));  // Dynamic
    
    // Don't declare huge arrays on stack!
    // char huge[10000000];  // 10MB - stack overflow!
    
    // Instead use heap
    char *safe = malloc(10000000);      // OK - heap
    
    free(heap_array);
    free(safe);
}
```

---

### Q2: Object lifecycle in C
**Answer:**
```c
// In C, there's no true constructor/destructor
// Must manually manage initialization/cleanup

// Resource acquisition
typedef struct {
    int *data;
    FILE *file;
    size_t size;
} Resource;

// Constructor-like function
Resource *resource_create(const char *filename, size_t size) {
    Resource *r = malloc(sizeof(Resource));
    if (r == NULL) return NULL;
    
    r->data = malloc(size);
    if (r->data == NULL) {
        free(r);
        return NULL;
    }
    
    r->file = fopen(filename, "w");
    if (r->file == NULL) {
        free(r->data);
        free(r);
        return NULL;
    }
    
    r->size = size;
    return r;
}

// Destructor-like function
void resource_destroy(Resource *r) {
    if (r == NULL) return;
    
    if (r->file) fclose(r->file);
    if (r->data) free(r->data);
    free(r);
}

// Usage - must be matched
Resource *res = resource_create("data.txt", 1024);
if (res) {
    // Use resource
    resource_destroy(res);
}
```

---

## Advanced Memory Management

### Q1: Memory alignment and padding
**Answer:**
```c
// CPUs access aligned memory faster/safer
// Alignment = multiple of data size

#define CACHE_LINE_SIZE 64  // ARM Cortex-M7

// Manual alignment
uint8_t *allocate_aligned(size_t size, size_t align) {
    // Allocate extra for alignment
    void *unaligned = malloc(size + align - 1);
    if (unaligned == NULL) return NULL;
    
    // Round up to alignment boundary
    uintptr_t addr = (uintptr_t)unaligned;
    uintptr_t aligned = (addr + align - 1) & ~(align - 1);
    
    return (uint8_t *)aligned;
}

// Modern C11 provides aligned_alloc
#ifdef __STDC_VERSION__ >= 201112L
uint8_t *buffer = (uint8_t *)aligned_alloc(64, 1024);
// Returns 1024-byte buffer aligned to 64-byte boundary
free(buffer);
#endif

// For DMA (must be cache-aligned):
uint8_t *dma_buffer = aligned_alloc(CACHE_LINE_SIZE, DMA_SIZE);
// Ensures DMA doesn't affect other cache lines
```

---

### Q2: Memory pools for embedded systems
**Answer:**
```c
// Fixed-size pools avoid fragmentation
typedef struct {
    uint8_t memory[4096];       // Pool capacity
    uint8_t allocation_map[64]; // Bitmap: 1=allocated, 0=free
    size_t block_size;          // 64 bytes per block
    size_t block_count;         // 64 blocks
} MemoryPool;

MemoryPool pool = {0, 0, 64, 64};

void *pool_alloc(MemoryPool *pool) {
    // Find first available block
    for (int i = 0; i < pool->block_count; i++) {
        if ((pool->allocation_map[i / 8] & (1 << (i % 8))) == 0) {
            // Found free block
            pool->allocation_map[i / 8] |= (1 << (i % 8));
            return &pool->memory[i * pool->block_size];
        }
    }
    return NULL;  // Pool full
}

void pool_free(MemoryPool *pool, void *ptr) {
    if (ptr == NULL) return;
    
    // Calculate block index
    uint8_t *p = (uint8_t *)ptr;
    uint8_t *base = pool->memory;
    int block_idx = (p - base) / pool->block_size;
    
    if (block_idx >= 0 && block_idx < pool->block_count) {
        pool->allocation_map[block_idx / 8] &= ~(1 << (block_idx % 8));
    }
}

// Stack pool (LIFO - simpler, faster)
typedef struct {
    uint8_t pool[4096];
    size_t watermark;  // High water mark
} StackPool;

void *stack_alloc(StackPool *pool, size_t size) {
    if (pool->watermark + size > sizeof(pool->pool)) {
        return NULL;  // Out of memory
    }
    void *ptr = &pool->pool[pool->watermark];
    pool->watermark += size;
    return ptr;
}

void stack_reset(StackPool *pool) {
    pool->watermark = 0;  // Deallocate all at once
}
```

---

### Q3: Embedded RTOS memory patterns
**Answer:**
```c
// Most embedded systems pre-allocate all memory
// at startup, then run without malloc/free

// Pattern 1: Static allocation
static Task tasks[MAX_TASKS];
static Queue queues[MAX_QUEUES];
static Timer timers[MAX_TIMERS];

void system_init(void) {
    // Initialize all structures from static pools
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].state = TASK_FREE;
    }
}

// Pattern 2: Object pool for dynamic creation
typedef struct {
    Task tasks[MAX_TASKS];
    int used_count;
} TaskPool;

Task *create_task(TaskPool *pool) {
    if (pool->used_count >= MAX_TASKS) {
        return NULL;  // All tasks used
    }
    return &pool->tasks[pool->used_count++];
}

// Pattern 3: Ring buffer for queues (bounded size)
typedef struct {
    uint8_t buffer[1024];
    int write_pos;
    int read_pos;
    int count;
} RingBuffer;

int ring_buffer_write(RingBuffer *rb, uint8_t data) {
    if (rb->count >= sizeof(rb->buffer)) {
        return -1;  // Full
    }
    rb->buffer[rb->write_pos] = data;
    rb->write_pos = (rb->write_pos + 1) % sizeof(rb->buffer);
    rb->count++;
    return 0;
}
```

---

## Memory Protection

### Q1: Memory access violations
**Answer:**
```c
// Common violations:
int arr[10];

// Out of bounds - undefined behavior
arr[100] = 42;  // Writes to wrong memory
int val = arr[100];

// Use-after-free
int *ptr = malloc(100);
free(ptr);
*ptr = 5;  // Access freed memory - CRASH

// Buffer overflow (string handling)
char buffer[10];
strcpy(buffer, "This string is way too long");  // Overflow

// Stack smashing
void vulnerable(char *input) {
    char buffer[10];
    strcpy(buffer, input);  // If input > 10, overwrites return address
}

// Accessing member of NULL
struct Point {int x; int y;};
struct Point *p = NULL;
int val = p->x;  // Dereferencing NULL

// Uninitialized pointer
int *ptr;  // Contains garbage
*ptr = 5;  // Access garbage address - CRASH
```

---

### Q2: Memory debugging tools
**Answer:**
```c
// Valgrind (on Linux development system)
// Detects: leaks, invalid access, use-after-free
// gcc -g -O0 program.c -o program
// valgrind --leak-check=full ./program

// AddressSanitizer (compiler runtime checks)
// gcc -fsanitize=address program.c -o program
// ./program  // Shows access violations

// Memory guard implementation
typedef struct {
    uint32_t before_magic;
    uint8_t *ptr;
    size_t size;
    uint32_t after_magic;
} ProtectedAlloc;

#define MAGIC_BEFORE 0xABCDABCD
#define MAGIC_AFTER  0xDCDCDCDC

ProtectedAlloc *protected_malloc(size_t size) {
    ProtectedAlloc *pa = malloc(sizeof(ProtectedAlloc));
    pa->ptr = malloc(size);
    pa->size = size;
    pa->before_magic = MAGIC_BEFORE;
    pa->after_magic = MAGIC_AFTER;
    return pa;
}

void protected_check(ProtectedAlloc *pa) {
    ASSERT(pa->before_magic == MAGIC_BEFORE);
    ASSERT(pa->after_magic == MAGIC_AFTER);
}

// Usage
ProtectedAlloc *data = protected_malloc(100);
protected_check(data);  // Verify not overwritten
```

---

## Memory Performance

### Q1: Cache optimization
**Answer:**
```c
// Cache behavior matters in embedded systems

// Poor: Cache misses
int matrix[1000][1000];

for (int col = 0; col < 1000; col++) {
    for (int row = 0; row < 1000; row++) {
        sum += matrix[row][col];  // Column-major = cache misses
    }
}

// Better: Cache-friendly
for (int row = 0; row < 1000; row++) {
    for (int col = 0; col < 1000; col++) {
        sum += matrix[row][col];  // Row-major = cache hits
    }
}

// Cache line awareness (typically 32-64 bytes)
// Align frequently accessed data to cache lines
struct TimingCritical {
    uint32_t a;     // Accessed frequently
    uint32_t b;     // Different from a
    uint32_t padding[14];  // Pad to cache line
};
```

This memory management guide provides essential knowledge for embedded systems!
