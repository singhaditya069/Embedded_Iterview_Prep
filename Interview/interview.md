# Interview Questions and Answers for Top Hardware Product Companies

---

## 1. Bit Manipulation

**Q: Count the number of set bits in an integer.**

```cpp
int countSetBits(int n) {
    int count = 0;
    while(n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}
```
**Explanation:** This is a classic bit manipulation question asked in Google, Qualcomm, and Intel interviews. It uses bitwise AND and right shift to count set bits.

---

**Q: Find the only non-repeating element in an array where every other element repeats twice.**

```cpp
int singleNumber(int arr[], int n) {
    int res = 0;
    for(int i=0; i<n; i++) res ^= arr[i];
    return res;
}
```
**Explanation:** XOR-ing all elements cancels out pairs, leaving the unique element. Common in Samsung and Apple interviews.

---

## 2. Data Structures

**Q: Implement a stack using arrays.**

```cpp
#define MAX 100
int stack[MAX], top = -1;
void push(int x) { if(top < MAX-1) stack[++top] = x; }
int pop() { return (top >= 0) ? stack[top--] : -1; }
```
**Explanation:** Basic stack implementation, often asked in Texas Instruments and STM interviews.

---

**Q: Reverse a singly linked list.**

```cpp
struct Node { int data; Node* next; };
Node* reverse(Node* head) {
    Node* prev = NULL;
    while(head) {
        Node* next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    return prev;
}
```
**Explanation:** Reversing a linked list is a favorite in Google and Apple interviews.

---

## 3. OOPS Concepts

**Q: What is polymorphism? Give an example.**

```cpp
class Base { public: virtual void show() { cout << "Base"; } };
class Derived : public Base { public: void show() { cout << "Derived"; } };
Base* b = new Derived(); b->show(); // Output: Derived
```
**Explanation:** Polymorphism allows methods to behave differently based on the object. Common in all product company interviews.

---

## 4. Embedded/RTOS

**Q: Simulate a binary semaphore in C.**

```c
volatile int sem = 1;
void wait() { while(sem <= 0); sem--; }
void signal() { sem++; }
```
**Explanation:** Semaphore simulation is often asked in Qualcomm, Intel, and embedded interviews.

---

**Q: Design a circular buffer for UART communication.**

```cpp
#define SIZE 8
char buf[SIZE]; int head=0, tail=0;
void put(char c) { buf[head] = c; head = (head+1)%SIZE; }
char get() { char c = buf[tail]; tail = (tail+1)%SIZE; return c; }
```
**Explanation:** Circular buffer is a must-know for device driver and embedded OS interviews (Samsung, STM, TI).

---

## 5. System Design

**Q: Design an LRU Cache.**

```cpp
// See lru_cache.cpp in system design folder for full code
```
**Explanation:** LRU cache design is a favorite in Google and Apple system design rounds.

---

## 6. C/C++ Language

**Q: What is the difference between malloc and new?**

**A:** `malloc` is a C library function for memory allocation, does not call constructors, returns void*. `new` is a C++ operator, calls constructors, returns typed pointer.

---

**Q: How do you prevent memory leaks in C++?**

**A:** Use RAII (Resource Acquisition Is Initialization), smart pointers (`std::unique_ptr`, `std::shared_ptr`), and always free/delete allocated memory.

---

## 7. Microcontroller/Hardware

**Q: How do you access a 32-bit hardware register in C?**

```c
#define REG (*(volatile unsigned int*)0x40021000)
REG = 0x01; // Write
unsigned int val = REG; // Read
```
**Explanation:** Direct register access is essential for embedded and device driver interviews (STM, TI, Qualcomm).

---

**Q: How do you write an ISR (Interrupt Service Routine) in C?**

```c
void __attribute__((interrupt)) myISR(void) {
    // ISR code
}
```
**Explanation:** ISR syntax varies by compiler, but the concept is key for hardware interviews.

---

## 8. Algorithms

**Q: Find the missing number in an array of 1 to n.**

```cpp
int missingNumber(int arr[], int n) {
    int total = n*(n+1)/2;
    int sum = 0;
    for(int i=0; i<n-1; i++) sum += arr[i];
    return total - sum;
}
```
**Explanation:** Simple math-based solution, often asked in all product company interviews.

---

## 9. Advanced Bit Manipulation

**Q: Find the position of the rightmost set bit in an integer.**

```cpp
int rightmostSetBitPos(int n) {
    if(n == 0) return 0;
    return __builtin_ctz(n) + 1; // GCC/Clang
}
```
**Explanation:** This is a common question in Qualcomm and Intel interviews. `__builtin_ctz` counts trailing zeros.

---

**Q: Reverse the bits of a 32-bit unsigned integer.**

```cpp
unsigned int reverseBits(unsigned int n) {
    unsigned int rev = 0;
    for(int i=0; i<32; i++) {
        rev <<= 1;
        rev |= (n & 1);
        n >>= 1;
    }
    return rev;
}
```
**Explanation:** Bit reversal is often asked in Samsung and Apple interviews for firmware and driver roles.

---

## 10. Embedded/RTOS/System Design

**Q: Implement a simple producer-consumer problem using mutex and condition variable.**

```cpp
// See producer_consumer_rtos.cpp in system design folder for full code
```
**Explanation:** Producer-consumer is a classic RTOS and embedded OS synchronization problem (Qualcomm, TI, Samsung).

---

**Q: Design a UART driver with RX/TX buffer and ISR simulation.**

```cpp
// See uart_driver_design.cpp in system design folder for full code
```
**Explanation:** UART driver design is a must for embedded and device driver interviews (STM, TI, Qualcomm).

---

## 11. C/C++ Language and Memory

**Q: What is a memory leak? How do you detect and prevent it?**

**A:** A memory leak occurs when dynamically allocated memory is not freed. Use tools like Valgrind, AddressSanitizer, and smart pointers to detect and prevent leaks.

---

**Q: What is the difference between stack and heap memory?**

**A:** Stack is used for static memory allocation (local variables, function calls), is fast and limited in size. Heap is for dynamic allocation (`malloc`, `new`), is larger but slower and must be managed manually.

---

## 12. Algorithms/Data Structures

**Q: Detect a cycle in a linked list.**

```cpp
struct Node { int data; Node* next; };
bool hasCycle(Node* head) {
    Node *slow = head, *fast = head;
    while(fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast) return true;
    }
    return false;
}
```
**Explanation:** Floyd’s cycle-finding algorithm is a favorite in Google and Apple interviews.

---

**Q: Find the lowest common ancestor (LCA) in a binary tree.**

```cpp
struct Node { int data; Node* left; Node* right; };
Node* LCA(Node* root, int n1, int n2) {
    if(!root) return NULL;
    if(root->data == n1 || root->data == n2) return root;
    Node* left = LCA(root->left, n1, n2);
    Node* right = LCA(root->right, n1, n2);
    if(left && right) return root;
    return left ? left : right;
}
```
**Explanation:** LCA is a classic tree problem, often asked in Google, Apple, and Intel interviews.

---

## 13. DSA (Data Structures & Algorithms)

**Q: Implement a min-heap and extract the minimum element.**

```cpp
#include <vector>
#include <algorithm>
using namespace std;
vector<int> heap;
void insert(int x) { heap.push_back(x); push_heap(heap.begin(), heap.end(), greater<int>()); }
int extractMin() { pop_heap(heap.begin(), heap.end(), greater<int>()); int minVal = heap.back(); heap.pop_back(); return minVal; }
```
**Explanation:** Heap operations are common in Google and Apple interviews for priority queue and scheduling problems.

---

**Q: Find the kth largest element in an array.**

```cpp
#include <queue>
using namespace std;
int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for(int n : nums) {
        minHeap.push(n);
        if(minHeap.size() > k) minHeap.pop();
    }
    return minHeap.top();
}
```
**Explanation:** Kth largest/smallest is a favorite for DSA rounds in all product companies.

---

## 14. Embedded OS

**Q: What is a context switch? How is it handled in an RTOS?**

**A:** A context switch is the process of saving the state of a running task and loading the state of the next task. In RTOS, this is managed by the scheduler and involves saving CPU registers, stack pointer, and program counter.

---

**Q: Explain priority inversion and how to solve it.**

**A:** Priority inversion occurs when a high-priority task is blocked by a lower-priority task holding a resource. Solutions include priority inheritance and priority ceiling protocols.

---

## 15. Microcontrollers

**Q: What is the difference between Harvard and Von Neumann architectures?**

**A:** Harvard architecture has separate memory and buses for instructions and data, allowing simultaneous access. Von Neumann uses a single memory and bus for both, which can cause bottlenecks.

---

**Q: How do you debounce a button in embedded systems?**

**A:** Use a software delay or timer to ignore rapid changes in button state, or use hardware debouncing circuits (RC filter, Schmitt trigger).

---

## 16. Hardware Protocols

**Q: Explain the difference between I2C, SPI, and UART.**

**A:**
- **I2C:** Two-wire, multi-master, multi-slave, supports addressing, slower but simple.
- **SPI:** Four-wire, full-duplex, faster, no addressing, supports multiple slaves with chip select.
- **UART:** Asynchronous, two-wire, point-to-point, used for serial communication.

---

**Q: How do you implement a simple I2C master write in C?**

```c
void I2C_Start();
void I2C_Stop();
void I2C_Write(uint8_t data);
void I2C_MasterWrite(uint8_t addr, uint8_t data) {
    I2C_Start();
    I2C_Write(addr << 1);
    I2C_Write(data);
    I2C_Stop();
}
```
**Explanation:** I2C protocol implementation is a must for hardware and embedded interviews (STM, TI, Qualcomm).

---

## 17. Advanced DSA

**Q: Design a lock-free queue for multi-threaded environments.**

```cpp
// Use std::atomic and compare_exchange for lock-free queue (see Michael-Scott queue for production)
#include <atomic>
struct Node { int data; Node* next; };
struct LockFreeQueue {
    std::atomic<Node*> head, tail;
    LockFreeQueue() {
        Node* dummy = new Node{0, nullptr};
        head = tail = dummy;
    }
    void enqueue(int val) {
        Node* node = new Node{val, nullptr};
        Node* t;
        while(true) {
            t = tail;
            Node* next = t->next;
            if(t == tail) {
                if(next == nullptr) {
                    if(std::atomic_compare_exchange_weak(&(t->next), &next, node)) break;
                } else {
                    std::atomic_compare_exchange_weak(&tail, &t, next);
                }
            }
        }
        std::atomic_compare_exchange_weak(&tail, &t, node);
    }
    // Dequeue omitted for brevity
};
```
**Explanation:** Lock-free data structures are advanced topics for Google, Apple, and Qualcomm interviews.

---

**Q: Implement a Trie (Prefix Tree) for efficient string search.**

```cpp
struct TrieNode {
    TrieNode* children[26] = {nullptr};
    bool isEnd = false;
};
void insert(TrieNode* root, const string& word) {
    TrieNode* node = root;
    for(char c : word) {
        int idx = c - 'a';
        if(!node->children[idx]) node->children[idx] = new TrieNode();
        node = node->children[idx];
    }
    node->isEnd = true;
}
bool search(TrieNode* root, const string& word) {
    TrieNode* node = root;
    for(char c : word) {
        int idx = c - 'a';
        if(!node->children[idx]) return false;
        node = node->children[idx];
    }
    return node->isEnd;
}
```
**Explanation:** Tries are used for autocomplete, spell-check, and are common in Google/Apple interviews.

---

## 18. Advanced Embedded OS

**Q: How do you implement a real-time task scheduler with deadline and priority support?**

**A:** Use a priority queue (heap) to schedule tasks by earliest deadline first (EDF) or rate-monotonic scheduling (RMS). Each task has a deadline/priority, and the scheduler always runs the highest-priority ready task. Preemption and context switching are required.

---

**Q: How do you implement inter-task communication in an RTOS?**

**A:** Use message queues, mailboxes, or event flags. For example, FreeRTOS provides xQueueSend/xQueueReceive for safe communication between tasks.

---

## 19. Advanced Microcontrollers

**Q: How do you implement DMA (Direct Memory Access) in embedded systems?**

**A:** Configure the DMA controller registers with source, destination, and transfer size. Enable DMA and handle completion via interrupt. DMA allows peripherals to transfer data without CPU intervention, improving efficiency.

---

**Q: How do you implement low-power modes in microcontrollers?**

**A:** Configure the microcontroller to enter sleep/standby/deep sleep modes by setting power management registers. Use interrupts to wake up the MCU. Example: `__WFI()` (Wait For Interrupt) instruction in ARM Cortex-M.

---

## 20. Advanced Hardware Protocols

**Q: How do you implement a custom SPI protocol with CRC checking?**

```c
uint8_t spi_transfer(uint8_t data);
uint8_t calc_crc(uint8_t* data, int len);
void spi_send_with_crc(uint8_t* data, int len) {
    uint8_t crc = calc_crc(data, len);
    for(int i=0; i<len; i++) spi_transfer(data[i]);
    spi_transfer(crc);
}
```
**Explanation:** Custom protocol and CRC are advanced topics for hardware/embedded interviews (Qualcomm, TI, Intel).

---

**Q: How do you implement a bootloader for firmware update over UART?**

**A:** The bootloader receives firmware data over UART, writes it to flash memory, and verifies integrity (e.g., CRC). It then jumps to the new application code. Key steps: UART receive, flash erase/write, CRC check, jump to address.

---

## 21. Advanced DSA & Algorithms

**Q: Implement a Disjoint Set (Union-Find) with path compression and union by rank.**

```cpp
struct DSU {
    vector<int> parent, rank;
    DSU(int n) : parent(n), rank(n, 0) { for(int i=0; i<n; i++) parent[i] = i; }
    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
    void unite(int x, int y) {
        int xr = find(x), yr = find(y);
        if(xr == yr) return;
        if(rank[xr] < rank[yr]) parent[xr] = yr;
        else if(rank[xr] > rank[yr]) parent[yr] = xr;
        else { parent[yr] = xr; rank[xr]++; }
    }
};
```
**Explanation:** Disjoint Set is used in Kruskal's MST, connected components, and is a favorite in Google, Apple, and Samsung interviews.

---

**Q: Implement Dijkstra's algorithm for shortest path in a graph.**

```cpp
#include <vector>
#include <queue>
using namespace std;
vector<int> dijkstra(int n, vector<vector<pair<int,int>>>& adj, int src) {
    vector<int> dist(n, 1e9);
    dist[src] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, src});
    while(!pq.empty()) {
        int d = pq.top().first, u = pq.top().second; pq.pop();
        if(d > dist[u]) continue;
        for(auto& e : adj[u]) {
            int v = e.first, w = e.second;
            if(dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```
**Explanation:** Dijkstra's is a must-know for all product company interviews.

---

## 22. Advanced Embedded OS

**Q: How do you implement a software watchdog timer?**

**A:** Use a timer interrupt to periodically check if the main loop or critical task is running. If not, reset the system. Example: increment a counter in the timer ISR, reset it in the main loop; if it exceeds a threshold, trigger a system reset.

---

**Q: How do you implement double buffering for real-time data acquisition?**

**A:** Use two buffers: while one is being filled by an ISR (DMA or ADC), the other is processed by the main loop. Swap buffers when one is full. This ensures no data is lost during processing.

---

## 23. Advanced Microcontrollers

**Q: How do you implement a boot-time self-test for RAM and Flash?**

**A:** Write known patterns to RAM, read back and verify. For Flash, read and verify CRC or checksum. Report errors or halt if verification fails. This is required for safety-critical systems (automotive, medical).

---

**Q: How do you implement a custom protocol over CAN bus?**

**A:** Define message IDs, data payload structure, and error handling. Use CAN controller APIs to send/receive frames. Implement state machines for protocol logic and handle retransmissions and timeouts.

---

## 24. Advanced Hardware Protocols

**Q: How do you implement a multi-master arbitration in I2C?**

**A:** Monitor the SDA line during transmission. If a master detects a mismatch between sent and observed bits, it loses arbitration and stops transmitting. This is handled automatically by most I2C hardware.

---

**Q: How do you implement time synchronization over SPI or UART?**

**A:** Exchange timestamp messages and calculate offset/delay. Use hardware timers/counters for precise measurement. Apply correction to local clock. This is used in distributed embedded systems (industrial, automotive).

---

<!-- Add more advanced questions as needed for your preparation -->
