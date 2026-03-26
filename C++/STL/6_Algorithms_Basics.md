# 6. STL Algorithms - Basics

## What are Algorithms?

STL algorithms are generic functions that operate on containers through iterators. They provide common operations in an optimized, reusable way.

---

## Algorithm Categories

### 1. **Non-Modifying Algorithms** (Don't change container)

#### Find Operations
```cpp
#include <algorithm>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5, 2};

// Find single element
auto it = find(v.begin(), v.end(), 3);
if(it != v.end()) cout << "Found";

// Find first matching condition
auto it2 = find_if(v.begin(), v.end(), [](int x) { return x > 3; });
// Points to 4

// Find first NOT matching condition
auto it3 = find_if_not(v.begin(), v.end(), [](int x) { return x < 3; });
// Points to 3

// Count occurrences
int cnt = count(v.begin(), v.end(), 2);  // Returns 2

// Count matching condition
int cnt2 = count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
// Returns 2 (2 and 4)
```

#### Search Operations
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// Check if element exists
bool found = any_of(v.begin(), v.end(), [](int x) { return x > 4; });

// Check if all match condition
bool all_positive = all_of(v.begin(), v.end(), [](int x) { return x > 0; });

// Check if none match condition
bool no_negatives = none_of(v.begin(), v.end(), [](int x) { return x < 0; });

// Find first occurrence of value
auto it = search(v.begin(), v.end(), v.begin(), v.begin() + 2);

// Adjacent find (find adjacent equal elements)
vector<int> v2 = {1, 2, 2, 3, 4};
auto it2 = adjacent_find(v2.begin(), v2.end());  // Points to first 2
```

#### Min/Max Operations
```cpp
vector<int> v = {5, 2, 8, 1, 9};

// Find min and max elements
int minVal = *min_element(v.begin(), v.end());   // 1
int maxVal = *max_element(v.begin(), v.end());   // 9
auto minmax = minmax_element(v.begin(), v.end());
cout << minmax.first << " " << minmax.second;    // 1 9

// Min/max of two values
int m = min(5, 3);      // 3
int M = max(5, 3);      // 5

// Min/max of initializer list
int smallest = min({3, 1, 4, 1, 5, 9});
```

---

### 2. **Modifying Algorithms** (Change container content)

#### Copy Operations
```cpp
vector<int> src = {1, 2, 3, 4, 5};
vector<int> dst(5);

// Copy all
copy(src.begin(), src.end(), dst.begin());
// dst: {1, 2, 3, 4, 5}

// Copy with condition
vector<int> even;
copy_if(src.begin(), src.end(), 
        back_inserter(even), 
        [](int x) { return x % 2 == 0; });
// even: {2, 4}

// Copy backwards
vector<int> rev(5);
copy_backward(src.begin(), src.end(), rev.end());
// Handles overlapping ranges

// Copy n elements
vector<int> partial(3);
copy_n(src.begin(), 3, partial.begin());
// partial: {1, 2, 3}
```

#### Fill Operations
```cpp
vector<int> v(5);

// Fill all with value
fill(v.begin(), v.end(), 10);
// v: {10, 10, 10, 10, 10}

// Fill n elements
fill_n(v.begin(), 3, 5);
// v: {5, 5, 5, 10, 10}

// Generate with function/lambda
generate(v.begin(), v.end(), []() { return rand() % 10; });
```

#### Transform
```cpp
vector<int> v = {1, 2, 3, 4};
vector<int> result(4);

// Apply function to each element
transform(v.begin(), v.end(), 
          result.begin(), 
          [](int x) { return x * 2; });
// result: {2, 4, 6, 8}

// Transform two ranges
vector<int> v2 = {10, 20, 30, 40};
transform(v.begin(), v.end(), v2.begin(),
          result.begin(),
          [](int a, int b) { return a + b; });
// result: {11, 22, 33, 44}
```

#### Replace Operations
```cpp
vector<int> v = {1, 2, 3, 2, 4};

// Replace value
replace(v.begin(), v.end(), 2, 99);
// v: {1, 99, 3, 99, 4}

// Replace with condition
replace_if(v.begin(), v.end(), 
           [](int x) { return x > 2; }, 0);
// All > 2 become 0

// Replace and copy (original unchanged)
vector<int> dst(5);
replace_copy(v.begin(), v.end(), dst.begin(), 2, 99);
```

#### Remove Operations
```cpp
vector<int> v = {1, 2, 3, 2, 4, 2};

// Remove and erase (proper way!)
v.erase(remove(v.begin(), v.end(), 2), v.end());
// v: {1, 3, 4}

// Remove with condition
v.erase(remove_if(v.begin(), v.end(), 
                   [](int x) { return x < 2; }), 
        v.end());
```

#### Reverse and Rotate
```cpp
vector<int> v = {1, 2, 3, 4, 5};

// Reverse
reverse(v.begin(), v.end());
// v: {5, 4, 3, 2, 1}

// Rotate (like circular shift)
rotate(v.begin(), v.begin() + 2, v.end());
// v: {4, 3, 2, 1, 5} (moved first 2 to end)

// Shuffle
random_shuffle(v.begin(), v.end());  // Old way
shuffle(v.begin(), v.end(), mt19937(time(0)));  // Modern way
```

#### Unique
```cpp
vector<int> v = {1, 1, 2, 2, 2, 3, 3, 4};

// Must be sorted or adjacent duplicates
v.erase(unique(v.begin(), v.end()), v.end());
// v: {1, 2, 3, 4}

// Count unique elements
int uniqueCount = unique(v.begin(), v.end()) - v.begin();
```

---

### 3. **Partitioning Algorithms**

```cpp
vector<int> v = {1, 2, 3, 4, 5, 6};

// Partition: elements matching condition come first
partition(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
// v might be: {2, 4, 6, 1, 3, 5}

// Stable partition: preserves relative order
stable_partition(v.begin(), v.end(), [](int x) { return x < 4; });
// v: {1, 2, 3, 4, 5, 6} (1,2,3 come first, order preserved)

// Check if partitioned
bool isPartitioned = is_partitioned(v.begin(), v.end(), 
                                    [](int x) { return x < 3; });

// Partition point
auto split = partition_point(v.begin(), v.end(), 
                             [](int x) { return x < 4; });
```

---

### 4. **Sorting Algorithms**

```cpp
vector<int> v = {5, 2, 8, 1, 9, 3};

// Basic sort
sort(v.begin(), v.end());               // Ascending
sort(v.begin(), v.end(), greater<int>());  // Descending

// Stable sort (preserves order of equal elements)
stable_sort(v.begin(), v.end());

// Partial sort (sort first n elements)
partial_sort(v.begin(), v.begin() + 3, v.end());
// First 3 are sorted, rest don't matter

// Nth element (partition so nth element is correct)
nth_element(v.begin(), v.begin() + 3, v.end());
// Element at [3] is what would be there if sorted

// Check if sorted
bool sorted = is_sorted(v.begin(), v.end());
```

---

### 5. **Binary Search Algorithms**

```cpp
vector<int> v = {1, 2, 3, 4, 5};  // Must be sorted!

// Binary search (bool: exists?)
bool found = binary_search(v.begin(), v.end(), 3);  // true

// Lower bound (first >= value)
auto it1 = lower_bound(v.begin(), v.end(), 3);   // Points to 3

// Upper bound (first > value)
auto it2 = upper_bound(v.begin(), v.end(), 3);   // Points to 4

// Equal range (range of equal values)
auto range = equal_range(v.begin(), v.end(), 3);
// range.first = iterator to first 3
// range.second = iterator to first > 3

// Distance to element
int distance = lower_bound(v.begin(), v.end(), 3) - v.begin();  // 2
```

---

### 6. **Merging Algorithms**

```cpp
vector<int> v1 = {1, 3, 5};
vector<int> v2 = {2, 4, 6};
vector<int> result(6);

// Merge sorted ranges
merge(v1.begin(), v1.end(), v2.begin(), v2.end(), 
      result.begin());
// result: {1, 2, 3, 4, 5, 6}

// Check if merge would be stable
bool canMerge = includes(v1.begin(), v1.end(),
                        v2.begin(), v2.end());

// Inplace merge
vector<int> data = {1, 3, 5, 2, 4};
inplace_merge(data.begin(), data.begin() + 3, data.end());
```

---

### 7. **Numeric Operations**

```cpp
#include <numeric>

vector<int> v = {1, 2, 3, 4, 5};

// Sum
int sum = accumulate(v.begin(), v.end(), 0);  // 15

// Sum with custom operation
int product = accumulate(v.begin(), v.end(), 1, 
                         [](int a, int b) { return a * b; });  // 120

// Partial sum
vector<int> partial_sums(v.size());
partial_sum(v.begin(), v.end(), partial_sums.begin());
// partial_sums: {1, 3, 6, 10, 15}

// Inner product (dot product)
int dot = inner_product(v.begin(), v.end(), v.begin(), 0);
// 1*1 + 2*2 + 3*3 + 4*4 + 5*5 = 55
```

---

## Practical Examples

### Example 1: Count Vowels
```cpp
string s = "hello world";
int vowels = count_if(s.begin(), s.end(), 
                      [](char c) { return "aeiou"[c]; });
```

### Example 2: Convert to Uppercase
```cpp
string s = "hello";
transform(s.begin(), s.end(), s.begin(), ::toupper);
```

### Example 3: Remove Even Numbers
```cpp
vector<int> v = {1, 2, 3, 4, 5};
v.erase(remove_if(v.begin(), v.end(), 
                   [](int x) { return x % 2 == 0; }), 
        v.end());
// v: {1, 3, 5}
```

---

## Key Takeaways

✅ Non-modifying algorithms: find, search, count, min/max  
✅ Modifying algorithms: copy, transform, replace, remove  
✅ Sorting: sort, stable_sort, partial_sort, nth_element  
✅ Binary search (on sorted): binary_search, lower_bound, upper_bound  
✅ Always use algorithms instead of manual loops  
✅ Use lambdas for custom conditions  
✅ Remember: remove-erase idiom for safe removal  

**Next**: [Stack and Queue](7_Stack_Queue.md)
