// System Design Interview: LRU Cache Implementation
#include <iostream>
#include <unordered_map>
using namespace std;

// Doubly Linked List Node
struct Node {
    int key, value;
    Node *prev, *next;
    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    int capacity;
    unordered_map<int, Node*> cache;
    Node *head, *tail;

    // Add node to front
    void addNode(Node* node) {
        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = node;
    }
    // Remove node
    void removeNode(Node* node) {
        if (node->prev) node->prev->next = node->next;
        else head = node->next;
        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
    }
    // Move node to front
    void moveToFront(Node* node) {
        removeNode(node);
        addNode(node);
    }
    // Remove tail node
    Node* removeTail() {
        Node* node = tail;
        removeNode(node);
        return node;
    }
public:
    LRUCache(int cap) : capacity(cap), head(nullptr), tail(nullptr) {}
    int get(int key) {
        if (cache.count(key)) {
            Node* node = cache[key];
            moveToFront(node);
            return node->value;
        }
        return -1;
    }
    void put(int key, int value) {
        if (cache.count(key)) {
            Node* node = cache[key];
            node->value = value;
            moveToFront(node);
        } else {
            Node* node = new Node(key, value);
            addNode(node);
            cache[key] = node;
            if (cache.size() > capacity) {
                Node* old = removeTail();
                cache.erase(old->key);
                delete old;
            }
        }
    }
    // Display cache for demo
    void display() {
        Node* curr = head;
        cout << "Cache: ";
        while (curr) {
            cout << "[" << curr->key << ":" << curr->value << "] ";
            curr = curr->next;
        }
        cout << endl;
    }
};

int main() {
    LRUCache cache(3);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);
    cache.display();
    cout << "Get 2: " << cache.get(2) << endl;
    cache.display();
    cache.put(4, 40);
    cache.display();
    cout << "Get 1: " << cache.get(1) << endl;
    cache.display();
    return 0;
}
