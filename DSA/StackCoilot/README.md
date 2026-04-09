# Stack Implementation (StackCoilot)

## Overview
A comprehensive array-based stack implementation in C with all fundamental operations and example usage.

## Stack Data Structure
A **Stack** is a linear data structure that follows the **LIFO (Last-In-First-Out)** principle:
- Last element added is the first element removed
- Operations occur at one end called the **top**

## Operations Implemented

### 1. **initStack(Stack *stack)** 
- Initializes an empty stack
- Sets top = -1 (indicating empty)
- **Time Complexity:** O(1)

### 2. **push(Stack *stack, int value)**
- Adds an element to the top of the stack
- Increments top pointer
- Handles overflow with error message
- **Time Complexity:** O(1)
- **Returns:** 1 if successful, 0 if stack is full

### 3. **pop(Stack *stack)**
- Removes and returns the top element
- Decrements top pointer
- Handles underflow with error message
- **Time Complexity:** O(1)
- **Returns:** Top element or INT_MIN if empty

### 4. **peek(Stack *stack)**
- Views the top element WITHOUT removing it
- Does not modify the stack
- **Time Complexity:** O(1)
- **Returns:** Top element or INT_MIN if empty

### 5. **isEmpty(Stack *stack)**
- Checks if stack is empty
- **Time Complexity:** O(1)
- **Returns:** 1 if empty, 0 otherwise

### 6. **isFull(Stack *stack)**
- Checks if stack has reached maximum capacity
- **Time Complexity:** O(1)
- **Returns:** 1 if full, 0 otherwise

### 7. **size(Stack *stack)**
- Returns the number of elements in the stack
- **Time Complexity:** O(1)
- **Returns:** Element count

### 8. **display(Stack *stack)**
- Prints all stack elements from top to bottom
- **Time Complexity:** O(n) where n = number of elements
- Shows empty stack message if stack is empty

### 9. **search(Stack *stack, int value)**
- Searches for a specific element in the stack
- **Time Complexity:** O(n)
- **Returns:** 1 if found, 0 otherwise
- Prints position from bottom if found

### 10. **clear(Stack *stack)**
- Removes all elements from the stack
- Resets top to -1
- **Time Complexity:** O(1)

### 11. **printStats(Stack *stack)**
- Displays stack statistics
- Shows capacity, current size, available space
- Shows empty/full status

## Memory Management
- **Array-Based:** Fixed size array (MAX_SIZE = 100)
- **Fixed Capacity:** Cannot dynamically grow
- **Space Complexity:** O(MAX_SIZE)

## Use Cases
1. **Function Call Stack** - Managing function execution and returns
2. **Expression Evaluation** - Converting infix to postfix notation
3. **Backtracking Problems** - DFS, maze solving
4. **Undo/Redo Functionality** - In text editors
5. **Balanced Parentheses** - Checking bracket matching
6. **Browser History** - Back button functionality
7. **Memory Management** - Local variables storage

## Example Usage

```c
Stack myStack;
initStack(&myStack);

// Push elements
push(&myStack, 10);
push(&myStack, 20);
push(&myStack, 30);

// Peek top element
int top = peek(&myStack);  // Returns 30

// Pop elements
pop(&myStack);  // Removes 30

// Check status
if (!isEmpty(&myStack)) {
    display(&myStack);  // Shows 20, 10
}
```

## Advantages
- **Fast Access:** O(1) for push, pop, peek
- **Simple Implementation:** Easy to understand
- **Memory Efficient:** No extra pointers needed (compared to linked list)

## Disadvantages
- **Fixed Size:** Cannot grow beyond MAX_SIZE
- **Wasting Memory:** If MAX_SIZE is too large
- **No Dynamic Allocation:** Cannot adjust capacity at runtime

## Possible Extensions
1. **Dynamic Stack** - Using malloc() for growable stack
2. **Generic Stack** - Using void* for any data type
3. **Linked List Implementation** - Unlimited size
4. **Stack of Pointers** - Store addresses instead of values
5. **Template Stack** - In C++, for type-safe design

## Compilation & Execution

### Compile
```bash
gcc -o stack stack.c
```

### Run
```bash
./stack
```

### Output
Shows all stack operations with status messages and statistics.

## Key Concepts to Remember
- **LIFO Principle** - Last element pushed is first to pop
- **Top Pointer** - Always indicates the position of top element
- **Overflow** - Attempting to push to a full stack
- **Underflow** - Attempting to pop from an empty stack
- **Time Efficient** - All basic operations are O(1)

## Learning Outcomes
After studying this implementation, you should understand:
1. ✅ Stack behavior and principles
2. ✅ How array-based stacks work internally
3. ✅ LIFO vs FIFO data structures
4. ✅ Time complexity analysis
5. ✅ Error handling in data structures
6. ✅ Practical applications of stacks

---
**Author:** DSA Learning Series | StackCoilot  
**Date:** 2026  
**Status:** Complete Implementation
