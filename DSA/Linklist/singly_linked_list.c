// Singly Linked List in C: All Operations
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

// Insert at beginning
void insertBegin(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
}
// Insert at end
void insertEnd(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    if (!*head) { *head = newNode; return; }
    struct Node* temp = *head;
    while (temp->next) temp = temp->next;
    temp->next = newNode;
}
// Delete by value
void deleteNode(struct Node** head, int key) {
    struct Node *temp = *head, *prev = NULL;
    if (temp && temp->data == key) { *head = temp->next; free(temp); return; }
    while (temp && temp->data != key) { prev = temp; temp = temp->next; }
    if (!temp) return;
    prev->next = temp->next;
    free(temp);
}
// Search
int search(struct Node* head, int key) {
    while (head) { if (head->data == key) return 1; head = head->next; }
    return 0;
}
// Print
void printList(struct Node* head) {
    while (head) { printf("%d ", head->data); head = head->next; }
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
    deleteNode(&head, 10);
    printList(head);
    printf("Search 20: %d\n", search(head, 20));
    freeList(head);
    return 0;
}
