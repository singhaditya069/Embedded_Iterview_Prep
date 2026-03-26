# 7. Stack and Queue - Container Adapters

## What are Adapters?

**Container adapters** wrap existing containers and provide specialized interfaces. They're not independent containers but use existing ones internally.

---

## Stack - Last In First Out (LIFO)

A stack follows the LIFO principle: last element added is first to be removed.

### Creating and Using Stacks

```cpp
#include <stack>
using namespace std;

// Create stack
stack<int> s;

// Push (add on top)
s.push(10);
s.push(20);
s.push(30);

// Size and empty check
s.size();           // 3
s.empty();          // false

// Top (peek at top element)
int top = s.top();  // 30 (without removing)

// Pop (remove from top)
s.pop();            // Stack now has {10, 20}

// Stack operations
cout << s.top();    // 20
s.pop();
cout << s.top();    // 10
```

### Stack with Different Underlying Containers

```cpp
// Stack with vector (default for deque, but can use vector)
stack<int, vector<int>> s1;

// Stack with list
stack<int, list<int>> s2;

// Stack with deque (default)
stack<int, deque<int>> s3;
// Same as: stack<int> s3;
```

### Practical Examples

#### Example 1: Balanced Parentheses
```cpp
#include <stack>
#include <string>
using namespace std;

bool isBalanced(string s) {
    stack<char> st;
    
    for(char c : s) {
        if(c == '(' || c == '{' || c == '[') {
            st.push(c);
        } else if(c == ')' || c == '}' || c == ']') {
            if(st.empty()) return false;
            
            char top = st.top();
            if((c == ')' && top != '(') ||
               (c == '}' && top != '{') ||
               (c == ']' && top != '[')) {
                return false;
            }
            st.pop();
        }
    }
    
    return st.empty();
}
```

#### Example 2: Reverse String
```cpp
#include <stack>
#include <string>
using namespace std;

string reverseString(string s) {
    stack<char> st;
    
    for(char c : s) {
        st.push(c);
    }
    
    string result = "";
    while(!st.empty()) {
        result += st.top();
        st.pop();
    }
    
    return result;
}
```

#### Example 3: Decimal to Binary
```cpp
#include <stack>
#include <string>
using namespace std;

string decimalToBinary(int n) {
    stack<int> s;
    
    while(n > 0) {
        s.push(n % 2);
        n /= 2;
    }
    
    string binary = "";
    while(!s.empty()) {
        binary += to_string(s.top());
        s.pop();
    }
    
    return binary;
}
```

---

## Queue - First In First Out (FIFO)

A queue follows the FIFO principle: first element added is first to be removed.

### Creating and Using Queues

```cpp
#include <queue>
using namespace std;

// Create queue
queue<int> q;

// Enqueue (add at back)
q.push(10);
q.push(20);
q.push(30);

// Size and empty check
q.size();           // 3
q.empty();          // false

// Front (peek at first element)
int front = q.front();  // 10 (without removing)

// Back (peek at last element)
int back = q.back();    // 30

// Dequeue (remove from front)
q.pop();            // Queue now has {20, 30}

// Queue operations
cout << q.front();  // 20
q.pop();
cout << q.front();  // 30
```

### Practical Examples

#### Example 1: Print Level Order (BFS)
```cpp
#include <queue>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

void levelOrderTraversal(TreeNode* root) {
    if(!root) return;
    
    queue<TreeNode*> q;
    q.push(root);
    
    while(!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        
        cout << node->val << " ";
        
        if(node->left) q.push(node->left);
        if(node->right) q.push(node->right);
    }
}
```

#### Example 2: First Unique Character in String
```cpp
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

char firstUnique(string s) {
    unordered_map<char, int> freq;
    queue<char> q;
    
    for(char c : s) {
        freq[c]++;
        q.push(c);
    }
    
    while(!q.empty()) {
        if(freq[q.front()] == 1) {
            return q.front();
        }
        q.pop();
    }
    
    return ' ';  // No unique character
}
```

#### Example 3: Rotating Array
```cpp
#include <queue>
#include <vector>
using namespace std;

vector<int> rotate(vector<int> arr, int k) {
    queue<int> q(arr.begin(), arr.end());
    
    // Move k elements from front to back
    for(int i = 0; i < k; i++) {
        q.push(q.front());
        q.pop();
    }
    
    vector<int> result;
    while(!q.empty()) {
        result.push_back(q.front());
        q.pop();
    }
    
    return result;
}
```

---

## Priority Queue

A **priority_queue** is a queue where elements are processed based on priority (largest by default).

### Creating and Using Priority Queues

```cpp
#include <queue>
using namespace std;

// Max-heap (default) - largest element has highest priority
priority_queue<int> pq1;
pq1.push(5);
pq1.push(10);
pq1.push(3);
cout << pq1.top();  // 10 (largest)
pq1.pop();
cout << pq1.top();  // 5

// Min-heap - smallest element has highest priority
priority_queue<int, vector<int>, greater<int>> pq2;
pq2.push(5);
pq2.push(10);
pq2.push(3);
cout << pq2.top();  // 3 (smallest)
pq2.pop();
cout << pq2.top();  // 5
```

### Custom Comparator for Priority Queue

```cpp
#include <queue>
#include <vector>
using namespace std;

// Define a struct
struct Task {
    string name;
    int priority;
};

// Custom comparator (for max-heap based on priority)
struct Compare {
    bool operator()(const Task& a, const Task& b) const {
        return a.priority < b.priority;  // Higher priority numbers first
    }
};

priority_queue<Task, vector<Task>, Compare> pq;

pq.push({"Task A", 3});
pq.push({"Task B", 5});
pq.push({"Task C", 2});

while(!pq.empty()) {
    cout << pq.top().name << " (Priority: " << pq.top().priority << ")" << endl;
    pq.pop();
}
// Output:
// Task B (Priority: 5)
// Task A (Priority: 3)
// Task C (Priority: 2)
```

### Practical Examples

#### Example 1: Kth Largest Element
```cpp
#include <queue>
#include <vector>
using namespace std;

int kthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    
    for(int num : nums) {
        minHeap.push(num);
        if(minHeap.size() > k) {
            minHeap.pop();
        }
    }
    
    return minHeap.top();
}
```

#### Example 2: Task Scheduler
```cpp
#include <queue>
#include <vector>
using namespace std;

struct Task {
    string name;
    int priority;
    
    bool operator<(const Task& other) const {
        return priority < other.priority;  // Max-heap
    }
};

void processTasks(vector<Task>& tasks) {
    priority_queue<Task> pq(tasks.begin(), tasks.end());
    
    while(!pq.empty()) {
        Task t = pq.top();
        pq.pop();
        cout << "Processing: " << t.name << endl;
    }
}
```

#### Example 3: Merge K Sorted Lists
```cpp
#include <queue>
#include <vector>
using namespace std;

struct Node {
    int val;
    Node* next;
    
    bool operator>(const Node& other) const {
        return val > other.val;
    }
};

Node* mergeKLists(vector<Node*>& lists) {
    priority_queue<Node*, vector<Node*>, greater<Node*>> minHeap;
    
    for(auto head : lists) {
        if(head) minHeap.push(head);
    }
    
    Node dummy(0);
    Node* current = &dummy;
    
    while(!minHeap.empty()) {
        Node* minNode = minHeap.top();
        minHeap.pop();
        
        current->next = minNode;
        current = current->next;
        
        if(minNode->next) {
            minHeap.push(minNode->next);
        }
    }
    
    return dummy.next;
}
```

---

## Stack vs Queue vs Priority Queue

| Feature | Stack | Queue | Priority Queue |
|---------|-------|-------|----------------|
| Order | LIFO | FIFO | By priority |
| Push | O(1) | O(1) | O(log n) |
| Pop/Top | O(1) | O(1) | O(log n) |
| Use Case | Undo, Parsing | BFS, Scheduling | Dijkstra, Heap Sort |

---

## Key Takeaways

✅ Stack: Last In First Out (LIFO)  
✅ Queue: First In First Out (FIFO)  
✅ Priority Queue: Elements by priority  
✅ All have O(1) push and pop  
✅ Priority Queue uses O(log n) due to heap  
✅ Use stack for recursion, parsing, DFS  
✅ Use queue for BFS, scheduling  
✅ Use priority_queue for greedy algorithms  

**Next**: [Pairs and Tuples](8_Pairs_and_Tuples.md)
