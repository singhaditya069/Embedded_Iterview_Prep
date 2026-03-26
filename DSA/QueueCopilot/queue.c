#include <stdio.h>
#include <stdlib.h>
#define MAX 5   // maximum size of queue

int queue[MAX];
int front = -1, rear = -1;

// Function to check if queue is full
int isFull() {
    return (rear == MAX - 1);
}

// Function to check if queue is empty
int isEmpty() {
    return (front == -1 || front > rear);
}

// Enqueue operation
void enqueue(int data) {
    if (isFull()) {
        printf("Queue Overflow! Cannot insert %d\n", data);
        return;
    }
    if (front == -1) front = 0;  // first element
    queue[++rear] = data;
    printf("%d inserted into queue.\n", data);
}

// Dequeue operation
void dequeue() {
    if (isEmpty()) {
        printf("Queue Underflow! Nothing to delete.\n");
        return;
    }
    printf("%d deleted from queue.\n", queue[front]);
    front++;
}

// Peek operation
void peek() {
    if (isEmpty()) {
        printf("Queue is empty.\n");
        return;
    }
    printf("Front element is: %d\n", queue[front]);
}

// Display operation
void display() {
    if (isEmpty()) {
        printf("Queue is empty.\n");
        return;
    }
    printf("Queue elements: ");
    for (int i = front; i <= rear; i++) {
        printf("%d ", queue[i]);
    }
    printf("\n");
}

int main() {
    int choice, value;
    while (1) {
        printf("\n--- Queue Menu ---\n");
        printf("1. Enqueue\n");
        printf("2. Dequeue\n");
        printf("3. Peek\n");
        printf("4. Display\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                enqueue(value);
                break;
            case 2:
                dequeue();
                break;
            case 3:
                peek();
                break;
            case 4:
                display();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
