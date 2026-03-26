// RTOS and Real-Time Systems - Coding Problems

/**
 * Problem 1: Implement a Priority-based Task Scheduler
 * 
 * Create a simple preemptive scheduler that manages
 * tasks of different priorities.
 */

#include <cstdint>
#include <array>
#include <queue>

enum class TaskState {
    READY,
    RUNNING,
    BLOCKED,
    SUSPENDED
};

struct Task {
    uint32_t id;
    uint8_t priority;  // 0 = highest, 255 = lowest
    TaskState state;
    void (*entry_point)(void);
    
    bool operator<(const Task& other) const {
        // For priority queue: lower priority value = higher priority
        return priority > other.priority;
    }
};

class TaskScheduler {
private:
    static constexpr size_t MAX_TASKS = 32;
    std::array<Task, MAX_TASKS> tasks_;
    size_t task_count_;
    Task* current_task_;
    uint32_t next_task_id_;
    
public:
    TaskScheduler() : task_count_(0), current_task_(nullptr), next_task_id_(1) {}
    
    uint32_t create_task(uint8_t priority, void (*entry_point)(void)) {
        if (task_count_ >= MAX_TASKS) return 0;
        
        Task& task = tasks_[task_count_];
        task.id = next_task_id_++;
        task.priority = priority;
        task.state = TaskState::READY;
        task.entry_point = entry_point;
        task_count_++;
        
        return task.id;
    }
    
    void schedule() {
        // Find highest priority ready task
        Task* next_task = nullptr;
        uint8_t highest_priority = 255;
        
        for (size_t i = 0; i < task_count_; ++i) {
            if (tasks_[i].state == TaskState::READY && 
                tasks_[i].priority < highest_priority) {
                highest_priority = tasks_[i].priority;
                next_task = &tasks_[i];
            }
        }
        
        if (next_task != nullptr && next_task != current_task_) {
            if (current_task_ != nullptr) {
                current_task_->state = TaskState::READY;
            }
            current_task_ = next_task;
            current_task_->state = TaskState::RUNNING;
            current_task_->entry_point();
        }
    }
    
    void block_current_task() {
        if (current_task_) {
            current_task_->state = TaskState::BLOCKED;
        }
    }
    
    void unblock_task(uint32_t task_id) {
        for (size_t i = 0; i < task_count_; ++i) {
            if (tasks_[i].id == task_id && tasks_[i].state == TaskState::BLOCKED) {
                tasks_[i].state = TaskState::READY;
                break;
            }
        }
    }
};

/**
 * Problem 2: Implement a Binary Semaphore with Priority Inheritance
 * 
 * Design a semaphore that prevents priority inversion
 * using basic priority inheritance protocol.
 */

class BinarySemaphore {
private:
    bool is_available_;
    Task* holder_;
    uint8_t original_priority_;
    
public:
    BinarySemaphore() : is_available_(true), holder_(nullptr), original_priority_(255) {}
    
    bool acquire(Task* caller, TaskScheduler& scheduler) {
        if (is_available_) {
            is_available_ = false;
            holder_ = caller;
            return true;
        }
        
        if (holder_ != nullptr && caller->priority < holder_->priority) {
            // Priority inheritance: temporarily raise holder's priority
            original_priority_ = holder_->priority;
            holder_->priority = caller->priority;
        }
        
        // Block the caller wait for semaphore
        scheduler.block_current_task();
        return false;
    }
    
    bool release(TaskScheduler& scheduler) {
        if (holder_ == nullptr || !is_available_) return false;
        
        // Restore original priority
        if (original_priority_ != 255) {
            holder_->priority = original_priority_;
        }
        
        is_available_ = true;
        holder_ = nullptr;
        return true;
    }
    
    bool try_acquire(Task* caller) {
        if (is_available_) {
            is_available_ = false;
            holder_ = caller;
            return true;
        }
        return false;
    }
};

/**
 * Problem 3: Message Queue Implementation for Embedded RTOS
 * 
 * Create a fixed-size message queue for task-to-task communication.
 */

template<typename MessageType, size_t QUEUE_SIZE>
class MessageQueue {
private:
    std::array<MessageType, QUEUE_SIZE> buffer_;
    size_t head_;
    size_t tail_;
    size_t count_;
    
public:
    MessageQueue() : head_(0), tail_(0), count_(0) {}
    
    bool send(const MessageType& msg) {
        if (count_ >= QUEUE_SIZE) {
            return false;  // Queue full
        }
        
        buffer_[tail_] = msg;
        tail_ = (tail_ + 1) % QUEUE_SIZE;
        count_++;
        return true;
    }
    
    bool receive(MessageType& msg) {
        if (count_ == 0) {
            return false;  // Queue empty
        }
        
        msg = buffer_[head_];
        head_ = (head_ + 1) % QUEUE_SIZE;
        count_--;
        return true;
    }
    
    size_t available() const {
        return count_;
    }
    
    size_t space_available() const {
        return QUEUE_SIZE - count_;
    }
    
    void clear() {
        head_ = tail_ = count_ = 0;
    }
};

// Usage example
struct SensorData {
    uint32_t timestamp;
    int16_t temperature;
    uint16_t pressure;
};

using SensorQueue = MessageQueue<SensorData, 16>;

/**
 * Problem 4: Mutex with Deadlock Detection
 * 
 * Implement a mutex that detects potential deadlocks.
 */

class Mutex {
private:
    bool locked_;
    Task* owner_;
    uint32_t lock_count_;  // For recursive mutex
    uint32_t wait_count_;
    
public:
    Mutex() : locked_(false), owner_(nullptr), lock_count_(0), wait_count_(0) {}
    
    bool lock(Task* caller) {
        if (!locked_) {
            locked_ = true;
            owner_ = caller;
            lock_count_ = 1;
            return true;
        }
        
        if (owner_ == caller) {
            // Recursive lock by same owner
            lock_count_++;
            return true;
        }
        
        // Deadlock detection: check if caller already holds other locks
        wait_count_++;
        if (wait_count_ > 10) {
            // Potential deadlock condition
            return false;
        }
        
        return false;
    }
    
    bool unlock(Task* caller) {
        if (owner_ != caller) {
            return false;  // Task doesn't own the mutex
        }
        
        lock_count_--;
        if (lock_count_ == 0) {
            locked_ = false;
            owner_ = nullptr;
            wait_count_ = 0;
        }
        return true;
    }
};

/**
 * Problem 5: Software Timer Implementation
 * 
 * Implement periodic and one-shot timers for RTOS.
 */

enum class TimerType {
    ONE_SHOT,
    PERIODIC
};

class SoftwareTimer {
private:
    uint32_t period_ms_;
    uint32_t elapsed_ms_;
    TimerType type_;
    void (*callback_)(void);
    bool active_;
    
public:
    SoftwareTimer() : period_ms_(0), elapsed_ms_(0), 
                      type_(TimerType::ONE_SHOT), 
                      callback_(nullptr), active_(false) {}
    
    void start(uint32_t period_ms, TimerType type, void (*callback)(void)) {
        period_ms_ = period_ms;
        type_ = type;
        callback_ = callback;
        elapsed_ms_ = 0;
        active_ = true;
    }
    
    void stop() {
        active_ = false;
        elapsed_ms_ = 0;
    }
    
    void tick(uint32_t delta_ms) {
        if (!active_) return;
        
        elapsed_ms_ += delta_ms;
        
        if (elapsed_ms_ >= period_ms_) {
            if (callback_) {
                callback_();
            }
            
            if (type_ == TimerType::PERIODIC) {
                elapsed_ms_ = 0;
            } else {
                active_ = false;
            }
        }
    }
    
    bool is_active() const {
        return active_;
    }
    
    uint32_t get_remaining() const {
        return (elapsed_ms_ < period_ms_) ? (period_ms_ - elapsed_ms_) : 0;
    }
};

/**
 * Problem 6: Watchdog Timer Manager
 * 
 * Implement a watchdog system to detect task failures.
 */

class WatchdogTimer {
private:
    static constexpr size_t MAX_MONITORED_TASKS = 16;
    
    struct WatchEntry {
        uint32_t task_id;
        uint32_t timeout_ms;
        uint32_t last_kick_ms;
        bool active;
    };
    
    std::array<WatchEntry, MAX_MONITORED_TASKS> watch_list_;
    size_t entry_count_;
    uint32_t current_time_ms_;
    
public:
    WatchdogTimer() : entry_count_(0), current_time_ms_(0) {}
    
    bool add_task(uint32_t task_id, uint32_t timeout_ms) {
        if (entry_count_ >= MAX_MONITORED_TASKS) return false;
        
        watch_list_[entry_count_].task_id = task_id;
        watch_list_[entry_count_].timeout_ms = timeout_ms;
        watch_list_[entry_count_].last_kick_ms = current_time_ms_;
        watch_list_[entry_count_].active = true;
        entry_count_++;
        return true;
    }
    
    void kick(uint32_t task_id) {
        for (size_t i = 0; i < entry_count_; ++i) {
            if (watch_list_[i].task_id == task_id) {
                watch_list_[i].last_kick_ms = current_time_ms_;
                break;
            }
        }
    }
    
    void update(uint32_t delta_ms) {
        current_time_ms_ += delta_ms;
        
        for (size_t i = 0; i < entry_count_; ++i) {
            if (!watch_list_[i].active) continue;
            
            uint32_t elapsed = current_time_ms_ - watch_list_[i].last_kick_ms;
            if (elapsed > watch_list_[i].timeout_ms) {
                // Task watchdog timeout!
                handle_timeout(watch_list_[i].task_id);
                watch_list_[i].active = false;
            }
        }
    }
    
private:
    void handle_timeout(uint32_t task_id) {
        // Emergency recovery: reset, restart, or log error
    }
};

