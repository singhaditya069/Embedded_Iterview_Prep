#ifndef STACK_H
#define STACK_H

#define MAX_SIZE 100

/**
 * Stack Structure - Array-Based Implementation
 */
typedef struct {
    int arr[MAX_SIZE];
    int top;  // Index of the top element (-1 means empty)
} Stack;

/**
 * Function Declarations
 */

/**
 * Initialize Stack
 * Time Complexity: O(1)
 */
void initStack(Stack *stack);

/**
 * Check if Stack is Empty
 * Time Complexity: O(1)
 * Returns: 1 if empty, 0 otherwise
 */
int isEmpty(Stack *stack);

/**
 * Check if Stack is Full
 * Time Complexity: O(1)
 * Returns: 1 if full, 0 otherwise
 */
int isFull(Stack *stack);

/**
 * Push Operation - Add element to top of stack
 * Time Complexity: O(1)
 * Parameters: stack pointer, value to push
 * Returns: 1 if successful, 0 if stack is full
 */
int push(Stack *stack, int value);

/**
 * Pop Operation - Remove and return top element
 * Time Complexity: O(1)
 * Parameters: stack pointer
 * Returns: Top element if successful, INT_MIN if stack is empty
 */
int pop(Stack *stack);

/**
 * Peek Operation - View top element without removing
 * Time Complexity: O(1)
 * Parameters: stack pointer
 * Returns: Top element if stack not empty, INT_MIN otherwise
 */
int peek(Stack *stack);

/**
 * Get Stack Size - Count of elements
 * Time Complexity: O(1)
 * Parameters: stack pointer
 * Returns: Number of elements in stack
 */
int size(Stack *stack);

/**
 * Display Stack Contents
 * Time Complexity: O(n)
 * Parameters: stack pointer
 */
void display(Stack *stack);

/**
 * Clear Stack - Remove all elements
 * Time Complexity: O(1)
 * Parameters: stack pointer
 */
void clear(Stack *stack);

/**
 * Check if element exists in stack
 * Time Complexity: O(n)
 * Parameters: stack pointer, value to search
 * Returns: 1 if found, 0 otherwise
 */
int search(Stack *stack, int value);

/**
 * Print stack statistics
 * Time Complexity: O(1)
 * Parameters: stack pointer
 */
void printStats(Stack *stack);

#endif
