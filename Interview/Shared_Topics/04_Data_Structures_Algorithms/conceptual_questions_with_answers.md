# Data Structures & Algorithms - Comprehensive Q&A

## Linked Lists

### Q1: Singly Linked List - implementation and operations
**Answer:**
```c
#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Create new node
Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

// Insert at beginning
void insert_head(Node **head, int data) {
    Node *new_node = create_node(data);
    if (new_node == NULL) return;
    new_node->next = *head;
    *head = new_node;
}

// Insert at end
void insert_tail(Node **head, int data) {
    Node *new_node = create_node(data);
    if (new_node == NULL) return;
    
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

// Search
Node *search(Node *head, int data) {
    Node *current = head;
    while (current != NULL) {
        if (current->data == data) return current;
        current = current->next;
    }
    return NULL;
}

// Delete node
void delete_node(Node **head, int data) {
    if (*head == NULL) return;
    
    // Delete head
    if ((*head)->data == data) {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }
    
    // Delete middle/end
    Node *current = *head;
    while (current->next != NULL) {
        if (current->next->data == data) {
            Node *temp = current->next;
            current->next = current->next->next;
            free(temp);
            return;
        }
        current = current->next;
    }
}

// Print list
void print_list(Node *head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

// Free list
void free_list(Node **head) {
    while (*head != NULL) {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

// Usage example
int main(void) {
    Node *list = NULL;
    
    insert_tail(&list, 10);
    insert_tail(&list, 20);
    insert_head(&list, 5);
    
    print_list(list);  // 5 -> 10 -> 20 -> NULL
    
    delete_node(&list, 10);
    print_list(list);  // 5 -> 20 -> NULL
    
    free_list(&list);
    return 0;
}
```

**Complexity:**
- Insert head: O(1)
- Insert tail: O(n)
- Search: O(n)
- Delete: O(n)

**Use in embedded:** Queue implementation, task scheduling, memory block management

---

### Q2: Reversing a linked list
**Answer:**
```c
// Reverse linked list iteratively
Node *reverse_list_iterative(Node *head) {
    Node *prev = NULL;
    Node *current = head;
    
    while (current != NULL) {
        Node *next = current->next;  // Save next
        current->next = prev;         // Reverse link
        prev = current;               // Move prev forward
        current = next;               // Move current forward
    }
    
    return prev;  // New head
}

// Example:
// Original: 1 -> 2 -> 3 -> NULL
// Reversed: 3 -> 2 -> 1 -> NULL

// Reverse linked list recursively
Node *reverse_list_recursive(Node *head) {
    if (head == NULL || head->next == NULL) {
        return head;  // Base case
    }
    
    Node *new_head = reverse_list_recursive(head->next);
    
    head->next->next = head;  // Reverse the link
    head->next = NULL;        // Set new tail
    
    return new_head;
}
```

---

## Stacks and Queues

### Q1: Stack implementation using array
**Answer:**
```c
#define STACK_SIZE 100

typedef struct {
    int data[STACK_SIZE];
    int top;
} Stack;

void stack_init(Stack *s) {
    s->top = -1;
}

int stack_is_empty(Stack *s) {
    return s->top == -1;
}

int stack_is_full(Stack *s) {
    return s->top == STACK_SIZE - 1;
}

void stack_push(Stack *s, int data) {
    if (stack_is_full(s)) {
        printf("Stack overflow!\n");
        return;
    }
    s->data[++s->top] = data;
}

int stack_pop(Stack *s) {
    if (stack_is_empty(s)) {
        printf("Stack underflow!\n");
        return -1;
    }
    return s->data[s->top--];
}

int stack_peek(Stack *s) {
    if (stack_is_empty(s)) {
        printf("Stack empty!\n");
        return -1;
    }
    return s->data[s->top];
}

// Usage
int main(void) {
    Stack s;
    stack_init(&s);
    
    stack_push(&s, 10);
    stack_push(&s, 20);
    stack_push(&s, 30);
    
    printf("%d\n", stack_pop(&s));  // 30
    printf("%d\n", stack_pop(&s));  // 20
    
    return 0;
}
```

**Complexity:** Push/Pop/Peek: O(1)

**Embedded use:** Function call stack, interrupt handling, expression evaluation

---

### Q2: Queue implementation (circular buffer)
**Answer:**
```c
#define QUEUE_SIZE 100

typedef struct {
    int data[QUEUE_SIZE];
    int front;
    int rear;
    int count;
} Queue;

void queue_init(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int queue_is_empty(Queue *q) {
    return q->count == 0;
}

int queue_is_full(Queue *q) {
    return q->count == QUEUE_SIZE;
}

void queue_enqueue(Queue *q, int data) {
    if (queue_is_full(q)) {
        printf("Queue overflow!\n");
        return;
    }
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->data[q->rear] = data;
    q->count++;
}

int queue_dequeue(Queue *q) {
    if (queue_is_empty(q)) {
        printf("Queue underflow!\n");
        return -1;
    }
    int data = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    return data;
}

// Usage
int main(void) {
    Queue q;
    queue_init(&q);
    
    queue_enqueue(&q, 1);
    queue_enqueue(&q, 2);
    queue_enqueue(&q, 3);
    
    printf("%d\n", queue_dequeue(&q));  // 1
    printf("%d\n", queue_dequeue(&q));  // 2
    
    return 0;
}
```

**Embedded use:** RTOS message queues, DMA buffers, UART receive buffers

---

## Trees

### Q1: Binary Search Tree (BST)
**Answer:**
```c
typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *create_tree_node(int data) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL) return NULL;
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Insert into BST
TreeNode *bst_insert(TreeNode *root, int data) {
    if (root == NULL) {
        return create_tree_node(data);
    }
    
    if (data < root->data) {
        root->left = bst_insert(root->left, data);
    } else if (data > root->data) {
        root->right = bst_insert(root->right, data);
    }
    
    return root;
}

// Search in BST
TreeNode *bst_search(TreeNode *root, int data) {
    if (root == NULL) return NULL;
    
    if (data == root->data) return root;
    if (data < root->data) return bst_search(root->left, data);
    return bst_search(root->right, data);
}

// In-order traversal (sorted)
void inorder_traversal(TreeNode *root) {
    if (root == NULL) return;
    inorder_traversal(root->left);
    printf("%d ", root->data);
    inorder_traversal(root->right);
}

// Delete from BST
TreeNode *bst_delete(TreeNode *root, int data) {
    if (root == NULL) return NULL;
    
    if (data < root->data) {
        root->left = bst_delete(root->left, data);
    } else if (data > root->data) {
        root->right = bst_delete(root->right, data);
    } else {
        // Node found
        
        // Case 1: No children (leaf)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        
        // Case 2: One child
        if (root->left == NULL) {
            TreeNode *temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            TreeNode *temp = root->left;
            free(root);
            return temp;
        }
        
        // Case 3: Two children
        // Find min in right subtree (in-order successor)
        TreeNode *min_right = root->right;
        while (min_right->left != NULL) {
            min_right = min_right->left;
        }
        root->data = min_right->data;
        root->right = bst_delete(root->right, min_right->data);
    }
    
    return root;
}
```

**Complexity:**
- Search: O(log n) average, O(n) worst case
- Insert: O(log n) average, O(n) worst case  
- Delete: O(log n) average, O(n) worst case

---

## Sorting Algorithms

### Q1: QuickSort for embedded systems
**Answer:**
```c
// Partition function
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            // Swap
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    // Place pivot in correct position
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}

// QuickSort
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// Usage
int main(void) {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = 7;
    
    quicksort(arr, 0, n - 1);
    
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);  // 11 12 22 25 34 64 90
    }
    
    return 0;
}
```

**Complexity:** O(n log n) average, O(n²) worst case

**Embedded use:** Sensor data sorting, result ordering, signal processing

---

### Q2: Selection Sort for embedded (simple, minimal memory)
**Answer:**
```c
// Selection sort - O(n²) but cache-friendly
void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        
        // Swap
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

// Intersection sort - even simpler for small datasets
void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        
        arr[j + 1] = key;
    }
}
```

**When to use in embedded:**
- n < 10: Insertion sort (simplest)
- n < 100: Selection sort (predictable)
- n > 100: QuickSort (fastest)

---

## Complexity Analysis

### Q1: Big O notation
**Answer:**
```
Big O Complexity (from fastest to slowest):

O(1)    - Constant       Array access, hash lookup
O(log n) - Logarithmic   Binary search, balanced BST
O(n)    - Linear         Linear search, simple loop
O(n log n) - Linearithmic Merge sort, quicksort
O(n²)   - Quadratic      Bubble sort, selection sort
O(2^n)  - Exponential    Fibonacci recursive
O(n!)   - Factorial      Permutations

Rule of thumb for 1M elements:
O(1):       < 1 microsecond
O(log n):   ~20 microseconds
O(n):       ~1 millisecond
O(n log n): ~20 milliseconds
O(n²):      ~15+ seconds (NOT acceptable)
```

**Space Complexity:**
```
O(1)  - Constant space (in-place)
O(log n) - Recursive call stack (binary search)
O(n)  - Need to store all elements
O(n²) - Need matrix or 2D storage (avoid!)
```

---

### Q2: Selecting algorithms for embedded
**Answer:**
```c
// Embedded algorithm selection guide

// 1. For frequency analysis (counting occurrences)
#define COUNT_FREQ 1
void count_frequencies(int arr[], int n, int max_val) {
    int freq[max_val + 1];
    memset(freq, 0, (max_val + 1) * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        freq[arr[i]]++;
    }
    
    // O(n) time, O(max_val) space - best for embedded!
}

// 2. For sorted data insertion
// Use insertion sort - optimal for already sorted data
void insert_sorted(int arr[], int *n, int val) {
    int i = *n - 1;
    while (i >= 0 && arr[i] > val) {
        arr[i + 1] = arr[i];
        i--;
    }
    arr[i + 1] = val;
    (*n)++;
}

// 3. For finding top-K elements (use heap/partial sort)
#define HEAP_SIZE 10
typedef struct {
    int data[HEAP_SIZE];
    int size;
} MinHeap;

// Better: partial sort with limited memory
void find_top_k(int arr[], int n, int k) {
    // Only keep top k in memory - O(n log k) time, O(k) space
    // Use QuickSelect for median/k-th element: O(n) average
}
```

---

## Real-World Embedded Scenario

```c
// Sensor data management (temperature sensors)

typedef struct {
    uint16_t timestamp;
    int16_t temperature;
} TempReading;

// Collect readings in circular buffer
#define BUFFER_SIZE 100
TempReading readings[BUFFER_SIZE];
int buffer_idx = 0;

// Store reading
void store_reading(int16_t temp) {
    readings[buffer_idx].temperature = temp;
    readings[buffer_idx].timestamp = get_ticks();
    buffer_idx = (buffer_idx + 1) % BUFFER_SIZE;
}

// Find max temperature (O(n) scan)
int16_t find_max_temp(void) {
    int16_t max_temp = INT16_MIN;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (readings[i].temperature > max_temp) {
            max_temp = readings[i].temperature;
        }
    }
    return max_temp;
}

// Find average (used for trending)
int16_t calculate_average(void) {
    int32_t sum = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        sum += readings[i].temperature;
    }
    return sum / BUFFER_SIZE;
}

// Exponential moving average (for smoothing, constant memory!)
int16_t ema;
#define ALPHA 8  // 1/8 weighting

void update_ema(int16_t new_reading) {
    ema = (ema * 7 + new_reading) / 8;  // O(1) time, O(1) space
}
```

This comprehensive DSA guide provides essential algorithms for embedded systems!
