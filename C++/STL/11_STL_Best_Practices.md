# 11. STL Best Practices

## Container Selection Guide

### Decision Tree

```
Need to store elements?
├─ Yes, need fast random access?
│  ├─ Yes, frequent insertions in middle?
│  │  └─ No → Use vector
│  │  └─ Yes → Use deque
│  └─ No, frequent insertions/deletions in middle?
│     ├─ Yes → Use list
│     └─ No → Use vector (still good)
└─ No, need fast lookup?
   ├─ By key? → Use map/unordered_map
   ├─ Unique elements? → Use set/unordered_set
   └─ Key-value, duplicates? → Use multimap/multiset
```

### Quick Reference

```cpp
// Pros and cons summary
vector:          // Most versatile, use by default
                 // Pro: Fast random access, compact memory
                 // Con: Slow insert/delete in middle

deque:           // Fast operations at both ends
                 // Pro: O(1) at both ends with random access
                 // Con: Slightly more memory

list:            // Fast insert/delete in middle
                 // Pro: O(1) insert/delete anywhere
                 // Con: No random access, more memory

set:             // Fast sorted lookup and uniqueness
                 // Pro: Automatic sorting, O(log n) ops
                 // Con: More memory than vector

map:             // Fast key-value lookup
                 // Pro: Automatic sorting by key
                 // Con: More memory than vector

unordered_set:   // Fast unique lookup (no sorting)
                 // Pro: O(1) average hash lookup
                 // Con: No guaranteed order

unordered_map:   // Fast key-value lookup (no sorting)
                 // Pro: O(1) average hash lookup
                 // Con: No guaranteed order, more complex
```

---

## Performance Best Practices

### 1. Pre-allocate When Possible

```cpp
// ❌ Poor: Multiple reallocations
vector<int> v;
for(int i = 0; i < 10000; i++) {
    v.push_back(i);  // Capacity grows: 1, 2, 4, 8, 16...
}

// ✅ Good: Single allocation
vector<int> v;
v.reserve(10000);  // Allocate once
for(int i = 0; i < 10000; i++) {
    v.push_back(i);
}

// ✅ Better: Know exact size
vector<int> v(10000);  // Allocate and initialize
for(int i = 0; i < 10000; i++) {
    v[i] = i;
}
```

### 2. Use Move Semantics (C++11+)

```cpp
// ❌ Copy (expensive)
vector<string> v1 = {"a", "b", "c"};
vector<string> v2 = v1;  // Copies all strings

// ✅ Move (efficient)
vector<string> v2 = move(v1);  // Moves references

// ✅ Constructor initialization
vector<string> v = {"a", "b", "c"};  // Uses move internally

// ✅ Return by value (RVO/move)
vector<int> makeVector() {
    vector<int> v;
    // ... populate v ...
    return v;  // Compiler optimizes with RVO or move
}
```

### 3. Avoid Unnecessary Copies with References

```cpp
// ❌ Copying large objects
void process(vector<int> v) {  // COPIES!
    // ...
}

// ✅ Use const reference for read-only
void process(const vector<int>& v) {  // No copy
    // ...
}

// ✅ Use lvalue reference for modification
void modify(vector<int>& v) {
    v.push_back(1);
}

// ✅ Use const reference in range-based for
for(const auto& item : large_container) {
    process(item);  // No copy
}
```

### 4. Use Appropriate Iterator Types

```cpp
// ❌ Inefficient: List with random access
list<int> l = {1, 2, 3};
for(int i = 0; i < l.size(); i++) {
    // advance(l.begin(), i) is O(n)!
}

// ✅ Efficient: Use iterators
for(auto it = l.begin(); it != l.end(); ++it) {
    // O(1) per iteration
}

// ✅ Or use range-based for
for(int val : l) {
    // Compiler optimizes to iterator usage
}
```

---

## Memory Management

### 1. Use Appropriate Container Sizes

```cpp
// ❌ Wasteful: Allocates more than needed
vector<int> v(1000000);  // All elements initialized to 0

// ✅ Efficient: Only allocate what you use
vector<int> v;
v.reserve(1000000);  // Ready for growth
// Or populate with actual values

// ✅ Clean up unused memory
vector<int> temp = {1, 2, 3, 4, 5};
temp.erase(temp.begin() + 2);  // Removed 3
temp.shrink_to_fit();          // Release unused capacity
```

### 2. Avoid Iterator Invalidation

```cpp
// ❌ Dangerous: Iterator invalidated
vector<int> v = {1, 2, 3, 4, 5};
auto it = v.begin();
v.push_back(6);  // May invalidate 'it' if reallocation occurs

// ✅ Safe: Get new iterator
v = {1, 2, 3, 4, 5};
auto it = v.begin();
// Use 'it' immediately before modifications

// ✅ Safe: Erase returns next iterator
for(auto it = v.begin(); it != v.end(); ) {
    if(*it == 2) {
        it = v.erase(it);  // Safe
    } else {
        ++it;
    }
}

// ✅ Safer: Use list for frequent modifications
list<int> l = {1, 2, 3, 4, 5};
for(auto it = l.begin(); it != l.end(); ) {
    if(*it == 2) {
        it = l.erase(it);  // Still safe
    } else {
        ++it;
    }
}
```

### 3. String Optimization

```cpp
#include <string>
using namespace std;

// ❌ Inefficient: Multiple allocations
string result;
for(int i = 0; i < 10000; i++) {
    result += to_string(i);  // O(n) concatenation
}

// ✅ Efficient: Single pass
stringstream ss;
for(int i = 0; i < 10000; i++) {
    ss << i;
}
string result = ss.str();

// ✅ Or reserve
string result;
result.reserve(50000);  // Estimate needed size
for(int i = 0; i < 10000; i++) {
    result += to_string(i);
}
```

---

## Algorithm Best Practices

### 1. Use Algorithms Instead of Manual Loops

```cpp
// ❌ Manual loop
vector<int> v = {1, 2, 3, 4, 5};
int sum = 0;
for(int i = 0; i < v.size(); i++) {
    sum += v[i];
}

// ✅ STL algorithm
#include <numeric>
int sum = accumulate(v.begin(), v.end(), 0);

// ❌ Manual loop
vector<string> names = {"Alice", "Bob", "Charlie"};
vector<string> result;
for(const auto& name : names) {
    if(name.length() > 3) {
        result.push_back(name);
    }
}

// ✅ STL algorithm
vector<string> result;
copy_if(names.begin(), names.end(),
        back_inserter(result),
        [](const string& s) { return s.length() > 3; });
```

### 2. Correct Remove-Erase Pattern

```cpp
// ❌ Wrong: Only moves elements, doesn't erase
vector<int> v = {1, 2, 3, 2, 4};
remove(v.begin(), v.end(), 2);  // v is now {1, 3, 4, 2, 4}

// ✅ Correct: Remove-erase idiom
auto v = {1, 2, 3, 2, 4};
v.erase(remove(v.begin(), v.end(), 2), v.end());
// v is now {1, 3, 4}

// ✅ With conditions
v.erase(remove_if(v.begin(), v.end(),
                   [](int x) { return x < 2; }),
        v.end());
```

### 3. Use Appropriate Algorithms

```cpp
// For finding element
auto it = find(v.begin(), v.end(), value);
if(it != v.end()) { /*found*/ }

// For sorted ranges
if(binary_search(v.begin(), v.end(), value)) { /*found*/ }

// For custom condition
auto it = find_if(v.begin(), v.end(),
                  [](int x) { return x > 10; });

// For counting
int cnt = count(v.begin(), v.end(), value);
```

---

## Coding Style Best Practices

### 1. Use Range-based For Loops

```cpp
// ❌ Old style (C++98)
for(vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    cout << *it;
}

// ✅ C++11 range-based for
for(int val : v) {
    cout << val;
}

// ✅ With const reference for objects
for(const auto& obj : container) {
    process(obj);
}

// ✅ With structured bindings (C++17)
for(auto& [key, value] : map) {
    cout << key << ": " << value;
}
```

### 2. Use Auto Type Deduction

```cpp
// ❌ Verbose
map<string, vector<int>>::iterator it = myMap.begin();
vector<string>::const_iterator cit = vec.begin();

// ✅ Auto deduction
auto it = myMap.begin();
auto cit = vec.begin();

// ⚠️ Be careful with auto
auto value = someFunction();  // What type is this?
const auto& ref = largeObject;  // Better for objects
```

### 3. Use Lambdas Effectively

```cpp
// ✅ Simple comparator
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });

// ✅ Capture external state
int threshold = 10;
auto count = count_if(v.begin(), v.end(),
                     [threshold](int x) { return x > threshold; });

// ✅ Generic lambda (C++14)
auto process = [](const auto& item) { cout << item; };

// ⚠️ Avoid complex lambdas - use named functions
auto complex = [&data](const auto& a, const auto& b) {
    // Many lines of logic
    // Consider moving to a function instead
};
```

---

## Const Correctness

### 1. Use Const for Read-Only Access

```cpp
// ✅ Good: Clear that function doesn't modify
void process(const vector<int>& data) {
    for(const auto& val : data) {
        cout << val;
    }
}

// ✅ Const iterators
const auto& cit = v.cbegin();
cit = v.begin();  // Error: iterator is const

// ✅ Method const
class Container {
    vector<int> data;
public:
    int size() const { return data.size(); }  // Can't modify
    
    void modify() {  // Can modify
        data.push_back(1);
    }
};
```

---

## Exception Safety

### 1. Use RAII Principles

```cpp
#include <memory>
using namespace std;

// ❌ Manual memory management (risky)
vector<int>* ptr = new vector<int>();
// ...
delete ptr;  // What if exception occurs before this?

// ✅ RAII with unique_ptr
{
    unique_ptr<vector<int>> ptr(new vector<int>());
    // ...
}  // Automatically deleted

// ✅ RAII with vector directly
{
    vector<int> v;
    // ...
}  // Automatically cleaned up
```

### 2. Exception Safety in Algorithms

```cpp
// ✅ Safe: STL algorithms provide exception guarantees
try {
    sort(v.begin(), v.end());  // Strong guarantee
    copy_if(v.begin(), v.end(), back_inserter(result),
           [](int x) { return x > 10; });
} catch(...) {
    // Container state is still valid
}
```

---

## Common Mistakes to Avoid

```cpp
// ❌ Mistake 1: Comparing iterators from different containers
auto it1 = v1.begin();
auto it2 = v2.begin();
if(it1 == it2) { }  // Undefined behavior!

// ❌ Mistake 2: Dereferencing end() iterator
auto it = find(v.begin(), v.end(), value);
cout << *it;  // Dangerous if not found!

// ✅ Safe
if(it != v.end()) cout << *it;

// ❌ Mistake 3: Assuming container is not empty
cout << v.front();  // Crashes if v is empty

// ✅ Safe
if(!v.empty()) cout << v.front();

// ❌ Mistake 4: Using removed elements
auto new_end = remove(v.begin(), v.end(), value);
// Don't use [new_end, v.end()) - they're garbage

// ✅ Correct pattern
v.erase(remove(v.begin(), v.end(), value), v.end());
```

---

## Key Takeaways

✅ Choose container based on access patterns  
✅ Pre-allocate when size is known (reserve)  
✅ Use const references to avoid copies  
✅ Use range-based for loops (cleaner, safer)  
✅ Prefer STL algorithms over manual loops  
✅ Remember remove-erase idiom for removal  
✅ Use auto for type deduction (but be careful)  
✅ Follow const correctness  
✅ Use move semantics where appropriate  
✅ Prefer unique_ptr for RAII  

**Next**: [Real World Examples](12_Real_World_Examples.md)
