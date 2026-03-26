# 2. Vectors and Dynamic Arrays

## What is a Vector?

A **vector** is a dynamic array that can grow or shrink at runtime. It's the most commonly used STL container.

### Vector vs Array

| Feature | Array | Vector |
|---------|-------|--------|
| Size | Fixed | Dynamic |
| Memory | Stack | Heap |
| Bounds | No checks | Optional checks |
| Flexibility | Low | High |
| Performance | Fast access | Fast access + dynamic sizing |

---

## Creating and Initializing Vectors

```cpp
#include <vector>
using namespace std;

// Empty vector
vector<int> v1;

// Vector with size
vector<int> v2(5);  // 5 elements, value 0

// Vector with size and initial value
vector<int> v3(5, 10);  // [10, 10, 10, 10, 10]

// Vector with initialization list
vector<int> v4 = {1, 2, 3, 4, 5};

// Copy constructor
vector<int> v5 = v4;

// 2D vector
vector<vector<int>> matrix(3, vector<int>(4, 0));  // 3x4 matrix filled with 0
```

---

## Basic Vector Operations

```cpp
vector<int> v = {10, 20, 30};

// Add elements
v.push_back(40);        // Add at end: [10, 20, 30, 40]
v.insert(v.begin() + 1, 15);  // Insert at position: [10, 15, 20, 30, 40]

// Remove elements
v.pop_back();           // Remove last: [10, 15, 20, 30]
v.erase(v.begin());     // Remove first: [15, 20, 30]
v.clear();              // Remove all

// Access elements
v[0];                   // Direct access (no bounds check)
v.at(0);                // Safe access (bounds check)
v.front();              // First element
v.back();               // Last element

// Size and capacity
v.size();               // Number of elements
v.capacity();           // Allocated space
v.empty();              // Boolean: is empty?

// Iterators
v.begin();              // Iterator to first element
v.end();                // Iterator past last element
v.rbegin();             // Reverse iterator to last element
v.rend();               // Reverse iterator before first element

// Modify size
v.resize(5);            // Resize to 5 elements
v.shrink_to_fit();      // Release unused memory
```

---

## Vector Capacity vs Size

```cpp
vector<int> v;

// Capacity grows exponentially (usually doubles)
cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << endl;  // 0, 0

v.push_back(1);
cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << endl;  // 1, 1

v.push_back(2);
cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << endl;  // 2, 2

v.push_back(3);
cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << endl;  // 3, 4

v.push_back(4);
v.push_back(5);
cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << endl;  // 5, 8

// Pre-allocate space
v.reserve(100);  // Allocate space for 100 elements without creating them
```

---

## Iterating Through Vectors

```cpp
vector<int> v = {1, 2, 3, 4, 5};

// 1. Traditional for loop with index
for(int i = 0; i < v.size(); ++i) {
    cout << v[i] << " ";
}

// 2. Iterator-based loop
for(auto it = v.begin(); it != v.end(); ++it) {
    cout << *it << " ";  // Dereference iterator
}

// 3. Range-based for loop (C++11)
for(int num : v) {
    cout << num << " ";
}

// 4. Reverse iteration
for(auto it = v.rbegin(); it != v.rend(); ++it) {
    cout << *it << " ";
}
```

---

## Common Vector Algorithms

```cpp
vector<int> v = {5, 2, 8, 1, 9, 3};

// Sort
sort(v.begin(), v.end());  // Ascending: [1, 2, 3, 5, 8, 9]
sort(v.begin(), v.end(), greater<int>());  // Descending: [9, 8, 5, 3, 2, 1]

// Find
auto it = find(v.begin(), v.end(), 5);
if(it != v.end()) cout << "Found at position: " << (it - v.begin());

// Count
int count = count(v.begin(), v.end(), 5);

// Min/Max
int minVal = *min_element(v.begin(), v.end());
int maxVal = *max_element(v.begin(), v.end());

// Sum
int sum = accumulate(v.begin(), v.end(), 0);

// Reverse
reverse(v.begin(), v.end());

// Transform (apply function to each element)
transform(v.begin(), v.end(), v.begin(), [](int x) { return x * 2; });

// Fill
fill(v.begin(), v.end(), 0);  // Set all elements to 0

// Check if sorted
bool isSorted = is_sorted(v.begin(), v.end());
```

---

## Practical Examples

### Example 1: Finding Maximum Element
```cpp
vector<int> scores = {85, 92, 78, 95, 88};
int maxScore = *max_element(scores.begin(), scores.end());
cout << "Highest score: " << maxScore;  // 95
```

### Example 2: Removing Duplicates
```cpp
vector<int> v = {1, 2, 2, 3, 3, 3, 4};
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());
// Result: [1, 2, 3, 4]
```

### Example 3: Sum of Elements
```cpp
vector<int> v = {1, 2, 3, 4, 5};
int sum = accumulate(v.begin(), v.end(), 0);
cout << "Sum: " << sum;  // 15
```

### Example 4: Print Elements
```cpp
vector<string> fruits = {"apple", "banana", "orange"};
for(const auto& fruit : fruits) {
    cout << fruit << " ";
}
```

---

## Vector vs Other Containers

Choose vector when:
- ✅ You need fast random access
- ✅ You primarily add/remove at the end
- ✅ Memory is contiguous

Don't use vector when:
- ❌ You frequently insert/delete at the beginning or middle
- ❌ You need a linked structure
- ❌ You need associative access (use map instead)

---

## Performance Considerations

```cpp
// ❌ Inefficient: Creates copies
vector<int> result;
for(int i = 0; i < 1000; ++i) {
    result.push_back(i);  // May allocate multiple times
}

// ✅ Efficient: Pre-allocate
vector<int> result;
result.reserve(1000);  // Allocate once
for(int i = 0; i < 1000; ++i) {
    result.push_back(i);
}
```

---

## Key Takeaways

✅ Vectors are dynamic arrays with automatic memory management  
✅ Use `push_back()` for adding, `pop_back()` for removing  
✅ Access with `[]` (no checks) or `.at()` (with bounds check)  
✅ Use range-based for loops for clarity  
✅ Reserve space if you know the size in advance  
✅ STL algorithms like `sort()`, `find()`, `transform()` work on vectors

**Next**: [Lists and Deques](3_Lists_and_Deques.md)
