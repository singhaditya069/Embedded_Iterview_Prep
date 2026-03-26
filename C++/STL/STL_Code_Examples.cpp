// STL_Code_Examples.cpp
// Comprehensive runnable code examples for STL learning
// Compile: g++ -std=c++17 STL_Code_Examples.cpp -o stl_examples

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include <functional>
#include <memory>
#include <tuple>
#include <utility>

using namespace std;

// ==================== VECTOR EXAMPLES ====================
void vectorExamples() {
    cout << \"\\n=== VECTOR EXAMPLES ===\\n\";
    
    // Create and initialize
    vector<int> v = {5, 2, 8, 1, 9};
    cout << \"Original vector: \";
    for(int x : v) cout << x << \" \";
    cout << endl;
    
    // Basic operations
    v.push_back(3);
    v.insert(v.begin(), 0);
    cout << \"After push_back(3) and insert(0): \";
    for(int x : v) cout << x << \" \";
    cout << endl;
    
    // Sorting
    vector<int> v2 = {5, 2, 8, 1, 9};
    sort(v2.begin(), v2.end());
    cout << \"Sorted ascending: \";
    for(int x : v2) cout << x << \" \";
    cout << endl;
    
    sort(v2.begin(), v2.end(), greater<int>());
    cout << \"Sorted descending: \";
    for(int x : v2) cout << x << \" \";
    cout << endl;
    
    // Find and count
    auto it = find(v.begin(), v.end(), 8);
    if(it != v.end()) cout << \"Found 8 at position: \" << (it - v.begin()) << endl;
    
    cout << \"Count of 5: \" << count(v.begin(), v.end(), 5) << endl;
}

// ==================== LIST EXAMPLES ====================
void listExamples() {
    cout << \"\\n=== LIST EXAMPLES ===\\n\";
    
    list<int> l1 = {10, 20, 30};
    list<int> l2 = {15, 25, 35};
    
    // Add elements
    l1.push_back(40);
    l1.push_front(5);
    cout << \"List after push operations: \";
    for(int x : l1) cout << x << \" \";
    cout << endl;
    
    // Remove duplicates (after sorting)
    list<int> l3 = {1, 2, 2, 3, 3, 3, 4};
    l3.sort();
    l3.unique();
    cout << \"After unique: \";
    for(int x : l3) cout << x << \" \";
    cout << endl;
    
    // Iteration
    cout << \"Reverse iteration: \";
    for(auto it = l1.rbegin(); it != l1.rend(); ++it) {
        cout << *it << \" \";
    }
    cout << endl;
}

// ==================== DEQUE EXAMPLES ====================
void dequeExamples() {
    cout << \"\\n=== DEQUE EXAMPLES ===\\n\";
    
    deque<int> dq = {1, 2, 3, 4, 5};
    
    // Add at both ends
    dq.push_back(6);
    dq.push_front(0);
    cout << \"After push operations: \";
    for(int x : dq) cout << x << \" \";
    cout << endl;
    
    // Remove from both ends
    dq.pop_front();
    dq.pop_back();
    cout << \"After pop operations: \";
    for(int x : dq) cout << x << \" \";
    cout << endl;
    
    // Random access
    cout << \"Deque[2]: \" << dq[2] << endl;
}

// ==================== SET EXAMPLES ====================
void setExamples() {
    cout << \"\\n=== SET EXAMPLES ===\\n\";
    
    set<int> s = {5, 2, 8, 2, 1, 8};  // Duplicates removed
    cout << \"Set (unique sorted): \";
    for(int x : s) cout << x << \" \";
    cout << endl;
    
    // Insert and operations
    s.insert(3);
    s.insert(9);
    cout << \"After insertions: \";
    for(int x : s) cout << x << \" \";
    cout << endl;
    
    // Find
    if(s.count(5)) cout << \"5 is in the set\" << endl;
    
    // Range operations
    auto it1 = s.lower_bound(3);
    auto it2 = s.upper_bound(7);
    cout << \"Elements from lower_bound(3) to upper_bound(7): \";
    for(auto it = it1; it != it2; ++it) cout << *it << \" \";
    cout << endl;
}

// ==================== MAP EXAMPLES ====================
void mapExamples() {
    cout << \"\\n=== MAP EXAMPLES ===\\n\";
    
    map<string, int> ages;
    ages[\"Alice\"] = 25;
    ages[\"Bob\"] = 30;
    ages[\"Charlie\"] = 22;
    
    cout << \"Map contents: \" << endl;
    for(auto& [name, age] : ages) {
        cout << \"  \" << name << \": \" << age << endl;
    }
    
    // Find
    if(ages.find(\"Bob\") != ages.end()) {
        cout << \"Bob's age: \" << ages[\"Bob\"] << endl;
    }
    
    // Word frequency
    map<string, int> freq;
    string text = \"hello world hello test world world\";
    stringstream ss(text);
    string word;
    while(ss >> word) {
        freq[word]++;
    }
    
    cout << \"Word frequencies: \" << endl;
    for(auto& [w, f] : freq) {
        cout << \"  \" << w << \": \" << f << endl;
    }
}

// ==================== UNORDERED_MAP EXAMPLES ====================
void unorderedMapExamples() {
    cout << \"\\n=== UNORDERED_MAP EXAMPLES ===\\n\";
    
    unordered_map<string, int> scores;
    scores[\"Player1\"] = 100;
    scores[\"Player2\"] = 85;
    scores[\"Player3\"] = 95;
    
    cout << \"Scores (order may vary): \" << endl;
    for(auto& [player, score] : scores) {
        cout << \"  \" << player << \": \" << score << endl;
    }
}

// ==================== STACK EXAMPLES ====================
void stackExamples() {
    cout << \"\\n=== STACK EXAMPLES ===\\n\";
    
    stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);
    
    cout << \"Stack (LIFO) - popping: \";
    while(!s.empty()) {
        cout << s.top() << \" \";
        s.pop();
    }
    cout << endl;
}

// ==================== QUEUE EXAMPLES ====================
void queueExamples() {
    cout << \"\\n=== QUEUE EXAMPLES ===\\n\";
    
    queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);
    
    cout << \"Queue (FIFO) - dequeuing: \";
    while(!q.empty()) {
        cout << q.front() << \" \";
        q.pop();
    }
    cout << endl;
}

// ==================== PRIORITY QUEUE EXAMPLES ====================
void priorityQueueExamples() {
    cout << \"\\n=== PRIORITY QUEUE EXAMPLES ===\\n\";
    
    // Max heap (default)
    priority_queue<int> pq_max;
    pq_max.push(5);
    pq_max.push(10);
    pq_max.push(3);
    
    cout << \"Max heap: \";
    while(!pq_max.empty()) {
        cout << pq_max.top() << \" \";
        pq_max.pop();
    }
    cout << endl;
    
    // Min heap
    priority_queue<int, vector<int>, greater<int>> pq_min;
    pq_min.push(5);
    pq_min.push(10);
    pq_min.push(3);
    
    cout << \"Min heap: \";
    while(!pq_min.empty()) {
        cout << pq_min.top() << \" \";
        pq_min.pop();
    }
    cout << endl;
}

// ==================== ALGORITHM EXAMPLES ====================
void algorithmExamples() {
    cout << \"\\n=== ALGORITHM EXAMPLES ===\\n\";
    
    vector<int> v = {1, 2, 3, 4, 5};
    
    // Transform
    vector<int> doubled(v.size());
    transform(v.begin(), v.end(), doubled.begin(), 
              [](int x) { return x * 2; });
    cout << \"Original: \";
    for(int x : v) cout << x << \" \";
    cout << \"\\nDoubled: \";
    for(int x : doubled) cout << x << \" \";
    cout << endl;
    
    // Accumulate (sum)
    int sum = accumulate(v.begin(), v.end(), 0);
    cout << \"Sum: \" << sum << endl;
    
    // Product
    int product = accumulate(v.begin(), v.end(), 1,
                            [](int a, int b) { return a * b; });
    cout << \"Product: \" << product << endl;
    
    // All/Any/None
    bool all_positive = all_of(v.begin(), v.end(),
                               [](int x) { return x > 0; });
    cout << \"All positive: \" << (all_positive ? \"Yes\" : \"No\") << endl;
    
    // Remove duplicates
    vector<int> v2 = {1, 2, 2, 3, 3, 3, 4};
    sort(v2.begin(), v2.end());
    v2.erase(unique(v2.begin(), v2.end()), v2.end());
    cout << \"After removing duplicates: \";
    for(int x : v2) cout << x << \" \";
    cout << endl;
}

// ==================== PAIR & TUPLE EXAMPLES ====================
void pairTupleExamples() {
    cout << \"\\n=== PAIR & TUPLE EXAMPLES ===\\n\";
    
    // Pairs
    pair<string, int> p = {\"Alice\", 25};
    cout << \"Pair: \" << p.first << \", \" << p.second << endl;
    
    // Tuples
    tuple<int, string, double> t = {42, \"hello\", 3.14};
    cout << \"Tuple: \" << get<0>(t) << \", \" << get<1>(t) << \", \" 
         << get<2>(t) << endl;
    
    // Structured bindings (C++17)
    auto [num, str] = p;
    cout << \"Structured binding: \" << num << \", \" << str << endl;
}

// ==================== CUSTOM COMPARATOR EXAMPLES ====================
void comparatorExamples() {
    cout << \"\\n=== CUSTOM COMPARATOR EXAMPLES ===\\n\";
    
    // Sorting with comparator
    vector<int> v = {5, 2, 8, 1, 9};
    
    // Descending
    sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    cout << \"Sorted descending: \";
    for(int x : v) cout << x << \" \";
    cout << endl;
    
    // By absolute value
    vector<int> v2 = {-5, 3, -8, 2, -1};
    sort(v2.begin(), v2.end(), [](int a, int b) { 
        return abs(a) < abs(b); 
    });
    cout << \"Sorted by absolute value: \";
    for(int x : v2) cout << x << \" \";
    cout << endl;
}

// ==================== ITERATOR EXAMPLES ====================
void iteratorExamples() {
    cout << \"\\n=== ITERATOR EXAMPLES ===\\n\";
    
    vector<int> v = {1, 2, 3, 4, 5};
    
    // Forward iteration
    cout << \"Forward: \";
    for(auto it = v.begin(); it != v.end(); ++it) {
        cout << *it << \" \";
    }
    cout << endl;
    
    // Reverse iteration
    cout << \"Reverse: \";
    for(auto it = v.rbegin(); it != v.rend(); ++it) {
        cout << *it << \" \";
    }
    cout << endl;
    
    // Distance
    auto it = v.begin();
    advance(it, 2);
    cout << \"Element after advance(2): \" << *it << endl;
    
    cout << \"Distance from begin() to found element: \"
         << distance(v.begin(), find(v.begin(), v.end(), 3)) << endl;
}

// ==================== PRACTICAL PROBLEM: LRU CACHE ====================
class LRUCache {
    map<int, pair<int, list<int>::iterator>> cache;
    list<int> lru;
    int capacity;
    
public:
    LRUCache(int cap) : capacity(cap) {}
    
    int get(int key) {
        if(cache.find(key) == cache.end()) return -1;
        lru.erase(cache[key].second);
        lru.push_front(key);
        cache[key].second = lru.begin();
        return cache[key].first;
    }
    
    void put(int key, int value) {
        if(cache.find(key) != cache.end()) {
            lru.erase(cache[key].second);
        } else if(cache.size() == capacity) {
            int lru_key = lru.back();
            lru.pop_back();
            cache.erase(lru_key);
        }
        lru.push_front(key);
        cache[key] = {value, lru.begin()};
    }
};

void lruCacheExample() {
    cout << \"\\n=== LRU CACHE EXAMPLE ===\\n\";
    
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cout << \"Get 1: \" << cache.get(1) << endl;  // 1
    cache.put(3, 3);
    cout << \"Get 2: \" << cache.get(2) << endl;  // -1 (evicted)
}

// ==================== MAIN ====================
int main() {
    cout << \"======================================\"
         << \"\\nSTL COMPREHENSIVE CODE EXAMPLES\\n\"
         << \"======================================\\n\";
    
    vectorExamples();
    listExamples();
    dequeExamples();
    setExamples();
    mapExamples();
    unorderedMapExamples();
    stackExamples();
    queueExamples();
    priorityQueueExamples();
    algorithmExamples();
    pairTupleExamples();
    comparatorExamples();
    iteratorExamples();
    lruCacheExample();
    
    cout << \"\\n======================================\\n\"
         << \"All examples completed successfully!\\n\"
         << \"======================================\\n\";
    
    return 0;
}
