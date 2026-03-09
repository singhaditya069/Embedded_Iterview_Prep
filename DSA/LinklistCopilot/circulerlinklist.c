#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* head = NULL;

// Function to create a new node
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

// Insert at beginning
void insertBeginning(int value) {
    struct Node* newNode = createNode(value);
    if (head == NULL) {
        head = newNode;
        head->next = head;
    } else {
        struct Node* temp = head;
        while (temp->next != head) {
            temp = temp->next;
        }
        newNode->next = head;
        temp->next = newNode;
        head = newNode;
    }
}

// Insert at end
void insertEnd(int value) {
    struct Node* newNode = createNode(value);
    if (head == NULL) {
        head = newNode;
        head->next = head;
    } else {
        struct Node* temp = head;
        while (temp->next != head) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->next = head;
    }
}

// Insert at specific position
void insertAtPosition(int value, int pos) {
    if (pos == 1) {
        insertBeginning(value);
        return;
    }
    struct Node* newNode = createNode(value);
    struct Node* temp = head;
    for (int i = 1; i < pos - 1 && temp->next != head; i++) {
        temp = temp->next;
    }
    newNode->next = temp->next;
    temp->next = newNode;
}

// Delete from beginning
void deleteBeginning() {
    if (head == NULL) {
        printf("List is empty!\n");
        return;
    }
    if (head->next == head) {
        free(head);
        head = NULL;
    } else {
        struct Node* temp = head;
        while (temp->next != head) {
            temp = temp->next;
        }
        struct Node* del = head;
        head = head->next;
        temp->next = head;
        free(del);
    }
}

// Delete from end
void deleteEnd() {
    if (head == NULL) {
        printf("List is empty!\n");
        return;
    }
    if (head->next == head) {
        free(head);
        head = NULL;
    } else {
        struct Node* temp = head;
        while (temp->next->next != head) {
            temp = temp->next;
        }
        struct Node* del = temp->next;
        temp->next = head;
        free(del);
    }
}

// Delete from specific position
void deleteAtPosition(int pos) {
    if (head == NULL) {
        printf("List is empty!\n");
        return;
    }
    if (pos == 1) {
        deleteBeginning();
        return;
    }
    struct Node* temp = head;
    for (int i = 1; i < pos - 1 && temp->next != head; i++) {
        temp = temp->next;
    }
    struct Node* del = temp->next;
    temp->next = del->next;
    free(del);
}

// Search for an element
void search(int key) {
    if (head == NULL) {
        printf("List is empty!\n");
        return;
    }
    struct Node* temp = head;
    int pos = 1;
    do {
        if (temp->data == key) {
            printf("Element %d found at position %d\n", key, pos);
            return;
        }
        temp = temp->next;
        pos++;
    } while (temp != head);
    printf("Element %d not found!\n", key);
}

// Update element at position
void update(int pos, int newValue) {
    if (head == NULL) {
        printf("List is empty!\n");
        return;
    }
    struct Node* temp = head;
    for (int i = 1; i < pos && temp->next != head; i++) {
        temp = temp->next;
    }
    temp->data = newValue;
}

// Count nodes
int countNodes() {
    if (head == NULL) return 0;
    int count = 0;
    struct Node* temp = head;
    do {
        count++;
        temp = temp->next;
    } while (temp != head);
    return count;
}

// Display list
void display() {
    if (head == NULL) {
        printf("List is empty!\n");
        return;
    }
    struct Node* temp = head;
    printf("Circular Linked List: ");
    do {
        printf("%d -> ", temp->data);
        temp = temp->next;
    } while (temp != head);
    printf("HEAD\n");
}

// Menu-driven program
int main() {
    int choice, value, pos;
    while (1) {
        printf("\n--- Circular Linked List Menu ---\n");
        printf("1. Insert at Beginning\n");
        printf("2. Insert at End\n");
        printf("3. Insert at Position\n");
        printf("4. Delete from Beginning\n");
        printf("5. Delete from End\n");
        printf("6. Delete from Position\n");
        printf("7. Search\n");
        printf("8. Update\n");
        printf("9. Count Nodes\n");
        printf("10. Display\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value: ");
                scanf("%d", &value);
                insertBeginning(value);
                break;
            case 2:
                printf("Enter value: ");
                scanf("%d", &value);
                insertEnd(value);
                break;
            case 3:
                printf("Enter value and position: ");
                scanf("%d %d", &value, &pos);
                insertAtPosition(value, pos);
                break;
            case 4:
                deleteBeginning();
                break;
            case 5:
                deleteEnd();
                break;
            case 6:
                printf("Enter position: ");
                scanf("%d", &pos);
                deleteAtPosition(pos);
                break;
            case 7:
                printf("Enter element to search: ");
                scanf("%d", &value);
                search(value);
                break;
            case 8:
                printf("Enter position and new value: ");
                scanf("%d %d", &pos, &value);
                update(pos, value);
                break;
            case 9:
                printf("Total nodes: %d\n", countNodes());
                break;
            case 10:
                display();
                break;
            case 11:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
