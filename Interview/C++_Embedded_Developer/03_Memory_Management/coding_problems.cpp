// Memory Management - Coding Problems

/**
 * Problem 1: Static Memory Pool Allocator
 * 
 * Implement a fixed-size memory pool for deterministic
 * memory allocation with zero fragmentation.
 */

#include <cstdint>
#include <array>
#include <cstring>

template<typename T, size_t BLOCK_COUNT>
class MemoryPool {
private:
    struct Block {
        uint8_t data[sizeof(T)];
        Block* next_free;
    };
    
    std::array<Block, BLOCK_COUNT> blocks_;
    Block* free_list_;
    
public:
    MemoryPool() {
        // Initialize free list
        for (size_t i = 0; i < BLOCK_COUNT - 1; ++i) {
            blocks_[i].next_free = &blocks_[i + 1];
        }
        blocks_[BLOCK_COUNT - 1].next_free = nullptr;
        free_list_ = &blocks_[0];
    }
    
    T* allocate() {
        if (!free_list_) {
            return nullptr;  // Pool exhausted
        }
        
        Block* block = free_list_;
        free_list_ = block->next_free;
        
        return new(block->data) T();  // Placement new
    }
    
    void deallocate(T* ptr) {
        if (!ptr) return;
        
        ptr->~T();  // Call destructor
        
        Block* block = reinterpret_cast<Block*>(ptr);
        block->next_free = free_list_;
        free_list_ = block;
    }
    
    size_t available_blocks() const {
        size_t count = 0;
        Block* current = free_list_;
        while (current) {
            count++;
            current = current->next_free;
        }
        return count;
    }
    
    size_t total_blocks() const {
        return BLOCK_COUNT;
    }
    
    bool is_exhausted() const {
        return free_list_ == nullptr;
    }
};

// Usage
struct DataBuffer {
    uint32_t timestamp;
    uint16_t samples[64];
};

// MemoryPool<DataBuffer, 16> buffer_pool;

/**
 * Problem 2: Stack-based String Builder (Zero Heap Allocation)
 * 
 * Design an efficient string builder that uses stack memory.
 */

template<size_t MAX_LENGTH>
class StackString {
private:
    std::array<char, MAX_LENGTH> buffer_;
    size_t length_;
    
public:
    StackString() : length_(0) {
        buffer_[0] = '\0';
    }
    
    bool append(const char* str) {
        size_t str_len = 0;
        while (str[str_len]) str_len++;
        
        if (length_ + str_len >= MAX_LENGTH) {
            return false;  // Buffer overflow
        }
        
        std::memcpy(&buffer_[length_], str, str_len);
        length_ += str_len;
        buffer_[length_] = '\0';
        return true;
    }
    
    bool append_int(int32_t value) {
        char temp[16];
        int pos = 0;
        
        if (value < 0) {
            if (length_ + 1 >= MAX_LENGTH) return false;
            buffer_[length_++] = '-';
            value = -value;
        }
        
        // Convert to string
        if (value == 0) {
            temp[pos++] = '0';
        } else {
            while (value > 0) {
                temp[pos++] = '0' + (value % 10);
                value /= 10;
            }
        }
        
        if (length_ + pos >= MAX_LENGTH) return false;
        
        // Reverse and append
        for (int i = pos - 1; i >= 0; --i) {
            buffer_[length_++] = temp[i];
        }
        buffer_[length_] = '\0';
        return true;
    }
    
    const char* c_str() const {
        return buffer_.data();
    }
    
    size_t length() const {
        return length_;
    }
    
    void clear() {
        length_ = 0;
        buffer_[0] = '\0';
    }
};

/**
 * Problem 3: Ring Buffer for DMA Data
 * 
 * Implement an efficient circular buffer for DMA transfers.
 */

template<typename T, size_t CAPACITY>
class RingBuffer {
private:
    std::array<T, CAPACITY> buffer_;
    size_t write_pos_;
    size_t read_pos_;
    size_t count_;
    
public:
    RingBuffer() : write_pos_(0), read_pos_(0), count_(0) {}
    
    bool write(const T& item) {
        if (count_ >= CAPACITY) {
            return false;  // Buffer full, overwrite not allowed
        }
        
        buffer_[write_pos_] = item;
        write_pos_ = (write_pos_ + 1) % CAPACITY;
        count_++;
        return true;
    }
    
    bool read(T& item) {
        if (count_ == 0) {
            return false;  // Buffer empty
        }
        
        item = buffer_[read_pos_];
        read_pos_ = (read_pos_ + 1) % CAPACITY;
        count_--;
        return true;
    }
    
    size_t available() const {
        return count_;
    }
    
    size_t space_available() const {
        return CAPACITY - count_;
    }
    
    void clear() {
        write_pos_ = read_pos_ = count_ = 0;
    }
    
    // For DMA: get contiguous block pointers
    T* get_write_ptr(size_t& contiguous_available) {
        contiguous_available = CAPACITY - write_pos_;
        if (read_pos_ > write_pos_) {
            contiguous_available = read_pos_ - write_pos_ - 1;
        }
        return &buffer_[write_pos_];
    }
    
    void advance_write(size_t count) {
        write_pos_ = (write_pos_ + count) % CAPACITY;
        count_ += count;
    }
};

/**
 * Problem 4: Memory Allocator with Alignment Support
 * 
 * Implement a custom allocator that handles aligned memory.
 */

class AlignedAllocator {
private:
    uint8_t buffer_[4096];
    uint8_t* current_pos_;
    
public:
    AlignedAllocator() : current_pos_(buffer_) {}
    
    void* allocate(size_t size, size_t alignment = 8) {
        // Align current position
        uintptr_t addr = reinterpret_cast<uintptr_t>(current_pos_);
        uintptr_t aligned_addr = (addr + alignment - 1) & ~(alignment - 1);
        
        // Check if we have enough space
        size_t padding = aligned_addr - addr;
        if (current_pos_ + padding + size > buffer_ + 4096) {
            return nullptr;
        }
        
        uint8_t* alloc_ptr = buffer_ + (aligned_addr - reinterpret_cast<uintptr_t>(buffer_));
        current_pos_ = alloc_ptr + size;
        
        return alloc_ptr;
    }
    
    void reset() {
        current_pos_ = buffer_;
    }
    
    size_t remaining() const {
        return (buffer_ + 4096) - current_pos_;
    }
};

/**
 * Problem 5: Cache-aware Data Layout
 * 
 * Design data structures optimized for cache performance.
 */

// Poor cache performance
struct PoorLayout {
    uint32_t field_a;
    uint64_t field_b;
    uint16_t field_c;
};

// Optimized cache layout (arrange by access pattern and alignment)
struct OptimizedLayout {
    uint64_t field_b;      // Most frequently accessed
    uint32_t field_a;
    uint16_t field_c;
    uint16_t padding_;     // Align to 16 bytes (cache line)
} __attribute__((aligned(16)));

/**
 * Problem 6: Memory Barrier and Volatile Usage
 * 
 * Implement correct volatile usage for memory-mapped I/O.
 */

struct UART_RegisterSet {
    volatile uint32_t data;      // Read/Write data register
    volatile uint32_t status;    // Read-only status register
    volatile uint32_t control;   // Write-only control register
} __attribute__((packed));

// Memory-mapped at address 0x40000000
volatile UART_RegisterSet* const UART0 = 
    reinterpret_cast<volatile UART_RegisterSet*>(0x40000000);

void uart_init() {
    UART0->control = 0x01;  // Enable UART
    UART0->control = 0x03;  // Enable TX and RX
}

void uart_send_byte(uint8_t byte) {
    while (!(UART0->status & 0x01));  // Wait for TX ready
    UART0->data = byte;
}

uint8_t uart_receive_byte() {
    while (!(UART0->status & 0x02));  // Wait for RX ready
    return (uint8_t)UART0->data;
}

/**
 * Problem 7: Implement __builtin_expect for Branch Prediction
 * 
 * Optimize hot/cold paths using compiler hints.
 */

#define likely(x)   (__builtin_expect(!!(x), 1))
#define unlikely(x) (__builtin_expect(!!(x), 0))

void process_sensor_data(uint16_t raw_value) {
    if (likely(raw_value < 4000)) {
        // Hot path: normal operation
        int temperature = (raw_value * 125) / 100;
    } else if (unlikely(raw_value >= 4000)) {
        // Cold path: error handling
        // Handle sensor fault
    }
}

