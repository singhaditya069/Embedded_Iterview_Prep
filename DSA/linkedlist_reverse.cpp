#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(NULL) {}
};

Node* reverse(Node* head) {
    Node* prev = NULL;
    Node* curr = head;
    while(curr) {
        Node* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

int main() {
    Node* head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(3);
    head = reverse(head);
    cout << "Reversed Linked List: ";
    while(head) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl;
    return 0;
}
