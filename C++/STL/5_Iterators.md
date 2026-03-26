# 5. Iterators - Navigating Containers

## What are Iterators?

An **iterator** is a generalized pointer that allows you to traverse and manipulate containers. It abstracts away the internal structure of containers.

---

## Iterator Categories

### 1. **Input Iterator** (Read-only, sequential)
- Only move forward
- Can read elements: `*it`
- Example: Input streams

```cpp
// Example: Reading from input
istream_iterator<int> input(cin);
istream_iterator<int> eof;
vector<int> v(input, eof);  // Read until EOF
```

### 2. **Output Iterator** (Write-only, sequential)
- Can only write elements: `*it = value`
- Move forward only
- Example: Output streams

```cpp
// Example: Writing to output
vector<int> v = {1, 2, 3};
copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
```

### 3. **Forward Iterator** (Read/Write, sequential)
- Read, write, and move forward
- No backward movement
- Example: Singly-linked lists

```cpp
forward_list<int> fl = {1, 2, 3};
auto it = fl.begin();
++it;           // Can move forward
*it = 20;       // Can read and write
```

### 4. **Bidirectional Iterator** (Read/Write, both directions)
- Read, write, move forward and backward
- Example: list, set, map

```cpp
list<int> l = {1, 2, 3};
auto it = l.begin();
++it;           // Move forward
--it;           // Move backward
*it = 20;       // Read/Write
```

### 5. **Random Access Iterator** (Read/Write, anywhere)
- Full pointer-like access
- Jump anywhere in O(1)
- Example: vector, deque, array

```cpp
vector<int> v = {1, 2, 3, 4, 5};
auto it = v.begin();
it += 3;        // Jump directly
it = v.begin() + 2;  // Arithmetic
cout << v[3];   // Random access
```

---

## Iterator Hierarchy

```
Input Iterator
    ↓
Forward Iterator
    ↓
Bidirectional Iterator
    ↓
Random Access Iterator
```

More restrictive → More capable (and more capabilities)

---

## Iterator Operations

```cpp
vector<int> v = {10, 20, 30, 40};

// Get iterators
auto begin_it = v.begin();      // First element
auto end_it = v.end();          // One past last (don't dereference!)
auto rbegin_it = v.rbegin();    // Last element (reverse)
auto rend_it = v.rend();        // One before first (reverse)

// Dereference
cout << *begin_it;              // 10

// Increment/Decrement
++begin_it;                     // Move forward
--begin_it;                     // Move backward

// Arithmetic (random access only)
auto it = v.begin();
it += 2;                        // Jump 2 positions
auto it2 = it + 1;              // Create new iterator
auto diff = it2 - it;           // Distance between iterators

// Comparison
if(it == v.end()) { }           // Check if at end
if(it != begin_it) { }          // Not equal
if(it < begin_it + 3) { }       // Less than (random access only)
```

---

## Working with Different Containers

### Vector (Random Access)
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// All operations work
for(auto it = v.begin(); it != v.end(); ++it) { }
for(auto it = v.begin() + 1; it <= v.end() - 1; ++it) { }
```

### List (Bidirectional)
```cpp
list<int> l = {1, 2, 3, 4, 5};

// OK: ++ and --
for(auto it = l.begin(); it != l.end(); ++it) { }

// NOT OK: arithmetic (l.begin() + 1 won't compile!)
// NOT OK: random access (l[0] doesn't exist)
```

### Set/Map (Bidirectional)
```cpp
set<int> s = {1, 2, 3};
map<string, int> m = {{"a", 1}};

// OK: ++ and --
for(auto it = s.begin(); it != s.end(); ++it) { }

// NOT OK: arithmetic or random access
// Iterator type depends on const-ness
const auto cit = s.cbegin();    // Const iterator
```

---

## Const and Reverse Iterators

```cpp
vector<int> v = {1, 2, 3};

// Regular iterators (mutable)
auto it = v.begin();
*it = 100;                  // OK: modify element

// Const iterators (immutable)
auto cit = v.cbegin();
*cit = 100;                 // Error: cannot modify

// Reverse iterators
for(auto rit = v.rbegin(); rit != v.rend(); ++rit) {
    cout << *rit;           // 3 2 1
}

// Const reverse iterators
auto crit = v.crbegin();
*crit = 100;                // Error
```

---

## Working with Iterator Functions

```cpp
vector<int> v = {10, 20, 30, 40, 50};

// Distance between iterators
auto it1 = v.begin();
auto it2 = v.begin() + 3;
int dist = distance(it1, it2);  // Returns 3

// Advance iterator
auto it = v.begin();
advance(it, 2);                 // Move 2 positions forward
cout << *it;                    // 30

// Next and prev (create new iterators)
auto next_it = next(v.begin(), 2);      // Points to v[2]
auto prev_it = prev(v.end(), 2);        // Points to v[3]
cout << *next_it << *prev_it;           // 30 40
```

---

## Insert Iterators

Insert iterators allow you to insert into containers instead of overwriting:

```cpp
vector<int> dest = {1, 2, 3};
vector<int> src = {10, 20, 30};

// back_inserter: Insert at end
copy(src.begin(), src.end(), back_inserter(dest));
// dest: {1, 2, 3, 10, 20, 30}

// front_inserter: Insert at front (list only)
list<int> l = {1, 2, 3};
copy(src.begin(), src.end(), front_inserter(l));
// l: {30, 20, 10, 1, 2, 3}

// inserter: Insert at specific position
vector<int> v = {1, 2, 3};
copy(src.begin(), src.end(), inserter(v, v.begin() + 1));
// v: {1, 10, 20, 30, 2, 3}
```

---

## Stream Iterators

```cpp
#include <iterator>
#include <iostream>
using namespace std;

// Read from input stream
istream_iterator<int> input(cin);
istream_iterator<int> eof;
vector<int> numbers(input, eof);

// Write to output stream
copy(numbers.begin(), numbers.end(), 
     ostream_iterator<int>(cout, " "));

// Read from file
ifstream file("numbers.txt");
istream_iterator<int> file_input(file);
istream_iterator<int> file_eof;
vector<int> file_numbers(file_input, file_eof);
```

---

## Common Iterator Patterns

### Pattern 1: Erase While Iterating
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// WRONG: Iterator invalidated after erase
for(auto it = v.begin(); it != v.end(); ++it) {
    if(*it == 3) v.erase(it);  // BUG!
}

// CORRECT: erase returns next iterator
for(auto it = v.begin(); it != v.end(); ) {
    if(*it == 3) {
        it = v.erase(it);  // Safe
    } else {
        ++it;
    }
}
```

### Pattern 2: Reverse Iteration
```cpp
vector<int> v = {1, 2, 3};

// Method 1: Reverse iterators
for(auto it = v.rbegin(); it != v.rend(); ++it) {
    cout << *it;  // 3 2 1
}

// Method 2: Index-based (vector/deque only)
for(int i = v.size() - 1; i >= 0; --i) {
    cout << v[i];  // 3 2 1
}
```

### Pattern 3: Find and Replace
```cpp
vector<int> v = {1, 2, 3, 2, 4};

auto it = find(v.begin(), v.end(), 2);
if(it != v.end()) {
    *it = 99;  // Replace first occurrence
}
```

---

## Iterator Invalidation

Different containers invalidate iterators differently:

```cpp
vector<int> v = {1, 2, 3};
auto it = v.begin();

// INVALIDATES all iterators
v.clear();                  // All iterators invalid
v.erase(v.begin());         // All at/after position invalid
v.insert(v.begin(), 0);     // All iterators invalid
v.push_back(4);             // Possibly invalid if reallocation

// SAFE
for(auto it = v.begin(); it != v.end(); ) {
    it = v.erase(it);       // erase returns valid iterator
}

// List: SAFER
list<int> l = {1, 2, 3};
auto lit = l.begin();
l.erase(lit++);             // Only erased iterator invalid
```

---

## Practical Examples

### Example 1: Print Container
```cpp
template<typename Container>
void printContainer(const Container& c) {
    for(auto it = c.begin(); it != c.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}

vector<int> v = {1, 2, 3};
printContainer(v);  // Works with any container
```

### Example 2: Sum Elements Using Iterator
```cpp
vector<int> v = {1, 2, 3, 4, 5};
int sum = 0;
for(auto it = v.begin(); it != v.end(); ++it) {
    sum += *it;
}
cout << sum;  // 15
```

### Example 3: Find Maximum
```cpp
vector<int> v = {5, 2, 8, 1};
auto max_it = max_element(v.begin(), v.end());
cout << *max_it;  // 8
cout << distance(v.begin(), max_it);  // Position: 2
```

---

## Key Takeaways

✅ Iterators abstract container internals  
✅ Different containers support different iterator types  
✅ vector/deque → random access (most capable)  
✅ list → bidirectional (no random access)  
✅ set/map → bidirectional (sorted)  
✅ Use range-based for loops when possible (simpler)  
✅ Use algorithms with iterators for complex operations  

**Next**: [Algorithms - Basics](6_Algorithms_Basics.md)
