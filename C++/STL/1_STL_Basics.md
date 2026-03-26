# 1. STL Basics - Introduction to Standard Template Library

## What is STL?

The **Standard Template Library (STL)** is a library of template classes and functions that supports:
- **Generic Programming**: Write once, use with any data type
- **Data Structures**: Containers to organize data
- **Algorithms**: Pre-built functions like sort, search, transform
- **Iterators**: Pointer-like objects to traverse containers

---

## Core Components of STL

### 1. **Containers** - Data Storage
Containers hold data in various structures:

| Container | Type | Best For | Time Complexity |
|-----------|------|----------|-----------------|
| vector | Sequential | Dynamic arrays | O(1) access, O(n) insert/delete |
| list | Sequential | Frequent insertions/deletions | O(1) insert/delete, O(n) access |
| deque | Sequential | Double-ended queue | O(1) at both ends |
| set | Associative | Unique sorted elements | O(log n) all operations |
| map | Associative | Key-value pairs (sorted) | O(log n) all operations |
| unordered_set | Hash | Unique elements (fast lookup) | O(1) average |
| unordered_map | Hash | Key-value pairs (fast lookup) | O(1) average |

### 2. **Iterators** - Traverse Containers
Iterators are like pointers that help navigate containers.

```cpp
vector<int> vec = {1, 2, 3};
for(auto it = vec.begin(); it != vec.end(); ++it) {
    cout << *it << " ";  // Dereference iterator
}
```

### 3. **Algorithms** - Operations on Data
Pre-built functions like `sort()`, `find()`, `transform()`, etc.

```cpp
// Sort a vector
sort(vec.begin(), vec.end());

// Find an element
auto it = find(vec.begin(), vec.end(), 5);

// Transform elements
transform(vec.begin(), vec.end(), vec.begin(), [](int x) { return x * 2; });
```

### 4. **Functors** - Function Objects
Objects that act like functions for custom operations.

```cpp
struct Compare {
    bool operator()(int a, int b) {
        return a > b;  // Descending order
    }
};

sort(vec.begin(), vec.end(), Compare());
```

---

## STL Header Files

```cpp
// Containers
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>

// Algorithms
#include <algorithm>

// Utilities
#include <iterator>
#include <functional>
#include <numeric>

// Include all STL
#include <bits/stdc++.h>  // Non-standard, not recommended for production
```

---

## Why Use STL?

1. **Saves Development Time**: Pre-built, tested data structures
2. **Reliability**: Written by experts, thoroughly tested
3. **Performance**: Optimized implementations
4. **Flexibility**: Works with any data type (templates)
5. **Maintainability**: Standard conventions known by all C++ developers

---

## First STL Program

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    // Create a vector
    vector<int> numbers = {5, 2, 8, 1, 9};
    
    // Sort using STL algorithm
    sort(numbers.begin(), numbers.end());
    
    // Print using range-based for loop
    for(int num : numbers) {
        cout << num << " ";
    }
    // Output: 1 2 5 8 9
    
    return 0;
}
```

---

## Common STL Operations

| Operation | Syntax | Returns |
|-----------|--------|---------|
| Create | `vector<int> v;` | - |
| Add | `v.push_back(5);` | - |
| Access | `v[0]` or `v.at(0)` | Element |
| Size | `v.size()` | int |
| Iterate | `for(auto x : v)` | Element |
| Find | `find(v.begin(), v.end(), 5)` | Iterator |
| Sort | `sort(v.begin(), v.end());` | - |

---

## Key Takeaways

✅ STL provides containers, iterators, algorithms, and functors  
✅ Use vectors for most sequential data needs  
✅ Use maps for key-value associations  
✅ Always use STL algorithms instead of manual loops where possible  
✅ Learn iterators to work with all container types uniformly  

**Next**: [Vectors and Arrays](2_Vectors_and_Arrays.md)
