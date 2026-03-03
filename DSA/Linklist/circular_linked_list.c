// Circular Linked List in C: All Operations
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

// Insert at end
void insertEnd(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    if (!*head) {
        newNode->next = newNode;
        *head = newNode;
        return;
    }
    struct Node* temp = *head;
    while (temp->next != *head) temp = temp->next;
    temp->next = newNode;
    newNode->next = *head;
}
// Delete by value
void deleteNode(struct Node** head, int key) {
    if (!*head) return;
    struct Node *curr = *head, *prev = NULL;
    do {
        if (curr->data == key) break;
        prev = curr;
        curr = curr->next;
    } while (curr != *head);
    if (curr->data != key) return;
    if (curr == *head && curr->next == *head) { free(curr); *head = NULL; return; }
    if (curr == *head) *head = curr->next;
    prev->next = curr->next;
    free(curr);
}
// Print
void printList(struct Node* head) {
    if (!head) return;
    struct Node* temp = head;
    do {
        printf("%d ", temp->data);
        temp = temp->next;
    } while (temp != head);
    printf("\n");
}
// Free
void freeList(struct Node* head) {
    if (!head) return;
    struct Node *curr = head, *next;
    do {
        next = curr->next;
        free(curr);
        curr = next;
    } while (curr != head);
}

int main() {
    struct Node* head = NULL;
    insertEnd(&head, 10);
    insertEnd(&head, 20);
    insertEnd(&head, 30);
    printList(head);
    deleteNode(&head, 20);
    printList(head);
    freeList(head);
    return 0;
}
