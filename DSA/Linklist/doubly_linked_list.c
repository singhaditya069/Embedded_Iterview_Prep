// Doubly Linked List in C: All Operations
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};

// Insert at beginning
void insertBegin(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = *head;
    if (*head) (*head)->prev = newNode;
    *head = newNode;
}
// Insert at end
void insertEnd(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    if (!*head) { newNode->prev = NULL; *head = newNode; return; }
    struct Node* temp = *head;
    while (temp->next) temp = temp->next;
    temp->next = newNode;
    newNode->prev = temp;
}
// Delete by value
void deleteNode(struct Node** head, int key) {
    struct Node* temp = *head;
    while (temp && temp->data != key) temp = temp->next;
    if (!temp) return;
    if (temp->prev) temp->prev->next = temp->next;
    else *head = temp->next;
    if (temp->next) temp->next->prev = temp->prev;
    free(temp);
}
// Print forward
void printList(struct Node* head) {
    while (head) { printf("%d ", head->data); head = head->next; }
    printf("\n");
}
// Print backward
void printReverse(struct Node* head) {
    if (!head) return;
    while (head->next) head = head->next;
    while (head) { printf("%d ", head->data); head = head->prev; }
    printf("\n");
}
// Free
void freeList(struct Node* head) {
    struct Node* tmp;
    while (head) { tmp = head; head = head->next; free(tmp); }
}

int main() {
    struct Node* head = NULL;
    insertBegin(&head, 10);
    insertEnd(&head, 20);
    insertBegin(&head, 5);
    printList(head);
    printReverse(head);
    deleteNode(&head, 10);
    printList(head);
    freeList(head);
    return 0;
}
