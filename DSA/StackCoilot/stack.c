#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_SIZE 100

/**
 * Stack Structure - Array-Based Implementation
 */
typedef struct {
    int arr[MAX_SIZE];
    int top;  // Index of the top element (-1 means empty)
} Stack;

/**
 * Initialize Stack
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 */
void initStack(Stack *stack) {
    stack->top = -1;
    printf("Stack initialized.\n");
}

/**
 * Check if Stack is Empty
 * Time Complexity: O(1)
 * Returns: 1 if empty, 0 otherwise
 */
int isEmpty(Stack *stack) {
    return stack->top == -1;
}

/**
 * Check if Stack is Full
 * Time Complexity: O(1)
 * Returns: 1 if full, 0 otherwise
 */
int isFull(Stack *stack) {
    return stack->top == MAX_SIZE - 1;
}

/**
 * Push Operation - Add element to top of stack
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 * Parameters: stack pointer, value to push
 * Returns: 1 if successful, 0 if stack is full
 */
int push(Stack *stack, int value) {
    if (isFull(stack)) {
        printf("ERROR: Stack Overflow! Cannot push %d.\n", value);
        return 0;
    }
    stack->arr[++stack->top] = value;
    printf("Pushed %d onto stack.\n", value);
    return 1;
}

/**
 * Pop Operation - Remove and return top element
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 * Parameters: stack pointer
 * Returns: Top element if successful, INT_MIN if stack is empty
 */
int pop(Stack *stack) {
    if (isEmpty(stack)) {
        printf("ERROR: Stack Underflow! Cannot pop from empty stack.\n");
        return INT_MIN;
    }
    int poppedValue = stack->arr[stack->top--];
    printf("Popped %d from stack.\n", poppedValue);
    return poppedValue;
}

/**
 * Peek Operation - View top element without removing
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 * Parameters: stack pointer
 * Returns: Top element if stack not empty, INT_MIN otherwise
 */
int peek(Stack *stack) {
    if (isEmpty(stack)) {
        printf("ERROR: Stack is empty! Cannot peek.\n");
        return INT_MIN;
    }
    return stack->arr[stack->top];
}

/**
 * Get Stack Size - Count of elements
 * Time Complexity: O(1)
 * Parameters: stack pointer
 * Returns: Number of elements in stack
 */
int size(Stack *stack) {
    return stack->top + 1;
}

/**
 * Display Stack Contents
 * Time Complexity: O(n) where n is number of elements
 * Space Complexity: O(1)
 * Parameters: stack pointer
 */
void display(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty.\n");
        return;
    }
    printf("Stack elements (top to bottom): ");
    for (int i = stack->top; i >= 0; i--) {
        printf("%d ", stack->arr[i]);
    }
    printf("\n");
}

/**
 * Clear Stack - Remove all elements
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 * Parameters: stack pointer
 */
void clear(Stack *stack) {
    stack->top = -1;
    printf("Stack cleared.\n");
}

/**
 * Check if element exists in stack
 * Time Complexity: O(n) where n is number of elements
 * Space Complexity: O(1)
 * Parameters: stack pointer, value to search
 * Returns: 1 if found, 0 otherwise
 */
int search(Stack *stack, int value) {
    for (int i = 0; i <= stack->top; i++) {
        if (stack->arr[i] == value) {
            printf("Element %d found at position %d from bottom.\n", value, i + 1);
            return 1;
        }
    }
    printf("Element %d not found in stack.\n", value);
    return 0;
}

/**
 * Print stack statistics
 * Time Complexity: O(1)
 * Parameters: stack pointer
 */
void printStats(Stack *stack) {
    printf("\n=== Stack Statistics ===\n");
    printf("Capacity: %d\n", MAX_SIZE);
    printf("Current Size: %d\n", size(stack));
    printf("Available Space: %d\n", MAX_SIZE - size(stack));
    printf("Is Empty: %s\n", isEmpty(stack) ? "Yes" : "No");
    printf("Is Full: %s\n", isFull(stack) ? "Yes" : "No");
    printf("=======================\n\n");
}

/**
 * Main function - Demonstrates all stack operations
 */
int main() {
    Stack stack;
    
    // Initialize stack
    initStack(&stack);
    printStats(&stack);
    
    // Push operations
    printf("\n--- PUSH Operations ---\n");
    push(&stack, 10);
    push(&stack, 20);
    push(&stack, 30);
    push(&stack, 40);
    push(&stack, 50);
    display(&stack);
    printStats(&stack);
    
    // Peek operation
    printf("\n--- PEEK Operation ---\n");
    int topElement = peek(&stack);
    if (topElement != INT_MIN) {
        printf("Top element without removal: %d\n", topElement);
    }
    display(&stack);
    
    // Pop operations
    printf("\n--- POP Operations ---\n");
    pop(&stack);
    pop(&stack);
    display(&stack);
    printStats(&stack);
    
    // Size operation
    printf("\n--- SIZE Operation ---\n");
    printf("Current stack size: %d\n", size(&stack));
    
    // Search operation
    printf("\n--- SEARCH Operation ---\n");
    search(&stack, 30);
    search(&stack, 50);
    search(&stack, 100);
    
    // Add more elements
    printf("\n--- More PUSH Operations ---\n");
    push(&stack, 60);
    push(&stack, 70);
    display(&stack);
    
    // Clear operation
    printf("\n--- CLEAR Operation ---\n");
    clear(&stack);
    display(&stack);
    printStats(&stack);
    
    // Test error conditions
    printf("\n--- ERROR Handling ---\n");
    pop(&stack);  // Pop from empty stack
    peek(&stack); // Peek empty stack
    
    // Stress test - fill the stack
    printf("\n--- STRESS Test (Fill Stack) ---\n");
    for (int i = 1; i <= MAX_SIZE; i++) {
        if (!push(&stack, i * 10)) {
            break;
        }
    }
    printStats(&stack);
    
    // Try to push when full
    printf("\nAttempting to push when stack is full:\n");
    push(&stack, 9999);
    
    // Empty the stack
    printf("\n--- EMPTYING Stack ---\n");
    while (!isEmpty(&stack)) {
        pop(&stack);
    }
    printStats(&stack);
    
    return 0;
}
