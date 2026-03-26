# 9. Advanced STL Algorithms

## Advanced Sorting and Partitioning

### Sorting Strategies

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {5, 2, 8, 1, 9, 3};

// 1. Quick Sort (average O(n log n), worst O(n²))
sort(v.begin(), v.end());

// 2. Stable Sort (preserves order of equal elements)
vector<int> scores = {80, 90, 85, 90, 75};
stable_sort(scores.begin(), scores.end());

// 3. Partial Sort (only sort first k elements)
partial_sort(v.begin(), v.begin() + 3, v.end());
// After: {1, 2, 3, ...} (only first 3 guaranteed sorted)

// 4. Heap Sort (O(n log n), guarantees worst case)
make_heap(v.begin(), v.end());
sort_heap(v.begin(), v.end());

// 5. Nth Element (partition point, O(n) average)
nth_element(v.begin(), v.begin() + 2, v.end());
// Element at index 2 is what would be there if sorted
```

### Advanced Partitioning

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5, 6};

// Partition by condition
auto it = partition(v.begin(), v.end(), 
                   [](int x) { return x < 4; });
// Elements < 4 are before 'it', >= 4 after 'it'

// Stable partition (preserves relative order)
stable_partition(v.begin(), v.end(), 
                [](int x) { return x % 2 == 0; });
// Even numbers first, order preserved

// Partition point
auto split = partition_point(v.begin(), v.end(),
                            [](int x) { return x < 5; });
// Returns iterator where condition changes

// Check if partitioned
bool is_part = is_partitioned(v.begin(), v.end(),
                              [](int x) { return x < 4; });
```

---

## Advanced Search and Find

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5, 1, 2, 3};

// Find all occurrences
vector<int> positions;
for(auto it = v.begin(); it != v.end(); 
    it = find(it, v.end(), 2)) {
    positions.push_back(distance(v.begin(), it));
    ++it;
}

// Find any of multiple values
vector<int> targets = {3, 5, 7};
auto it = find_first_of(v.begin(), v.end(),
                        targets.begin(), targets.end());

// Find a subsequence
vector<int> pattern = {2, 3, 4};
auto seq = search(v.begin(), v.end(),
                 pattern.begin(), pattern.end());

// Find end of subsequence
auto found = find_end(v.begin(), v.end(),
                     pattern.begin(), pattern.end());

// Adjacent elements satisfying condition
auto adj = adjacent_find(v.begin(), v.end(),
                        [](int a, int b) { return a == b; });
```

---

## Set Operations on Sorted Ranges

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> a = {1, 3, 5, 7};
vector<int> b = {2, 3, 5, 8};
vector<int> result;

// Set union (all unique elements)
set_union(a.begin(), a.end(), b.begin(), b.end(),
         back_inserter(result));
// result: {1, 2, 3, 5, 7, 8}

// Set intersection (common elements)
result.clear();
set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                back_inserter(result));
// result: {3, 5}

// Set difference (in a but not in b)
result.clear();
set_difference(a.begin(), a.end(), b.begin(), b.end(),
              back_inserter(result));
// result: {1, 7}

// Set symmetric difference (in a or b but not both)
result.clear();
set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                        back_inserter(result));
// result: {1, 2, 7, 8}

// Includes (is b subset of a?)
bool included = includes(a.begin(), a.end(),
                        {3, 5}.begin(), {3, 5}.end());  // true

// Merge (merge two sorted ranges)
vector<int> merged(a.size() + b.size());
merge(a.begin(), a.end(), b.begin(), b.end(),
     merged.begin());
// merged: {1, 2, 3, 3, 5, 5, 7, 8}

// Inplace merge
vector<int> combined = {1, 3, 5, 2, 4, 6};
inplace_merge(combined.begin(), combined.begin() + 3, combined.end());
```

---

## Transform and Accumulate

```cpp
#include <algorithm>
#include <numeric>
#include <vector>
using namespace std;

vector<int> a = {1, 2, 3};
vector<int> b = {10, 20, 30};
vector<int> result(3);

// Simple transform
transform(a.begin(), a.end(), result.begin(),
         [](int x) { return x * 2; });
// result: {2, 4, 6}

// Transform with two inputs
transform(a.begin(), a.end(), b.begin(), result.begin(),
         [](int x, int y) { return x + y; });
// result: {11, 22, 33}

// For each (apply function, no return)
for_each(a.begin(), a.end(), [](int x) {
    cout << x << " ";
});

// Accumulate (sum with custom operation)
int sum = accumulate(a.begin(), a.end(), 0);  // 6
int product = accumulate(a.begin(), a.end(), 1,
                        [](int acc, int x) { return acc * x; });  // 6

// Partial sum
vector<int> partial(a.size());
partial_sum(a.begin(), a.end(), partial.begin());
// partial: {1, 3, 6}

// Adjacent difference
vector<int> diff(a.size());
adjacent_difference(a.begin(), a.end(), diff.begin());
// diff: {1, 1, 1}

// Inner product (dot product)
int dot = inner_product(a.begin(), a.end(), b.begin(), 0);
// 1*10 + 2*20 + 3*30 = 140
```

---

## Heap Operations

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {3, 1, 4, 1, 5, 9};

// Make heap
make_heap(v.begin(), v.end());
// Now v is a max-heap

// Add to heap
v.push_back(2);
push_heap(v.begin(), v.end());

// Remove from heap
pop_heap(v.begin(), v.end());
int max_elem = v.back();
v.pop_back();

// Check if heap
bool is_h = is_heap(v.begin(), v.end());

// Heap sort
vector<int> to_sort = {5, 2, 8, 1, 9};
make_heap(to_sort.begin(), to_sort.end());
sort_heap(to_sort.begin(), to_sort.end());
// to_sort: {1, 2, 5, 8, 9}

// Largest k elements using partial sort
vector<int> data = {5, 2, 8, 1, 9, 3, 7};
partial_sort(data.begin(), data.begin() + 3, data.end(),
            greater<int>());
// data: {9, 8, 7, ...}
```

---

## Advanced Search with Predicates

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

// Find first element > 5
auto it1 = find_if(v.begin(), v.end(), 
                   [](int x) { return x > 5; });

// Find first element NOT > 5
auto it2 = find_if_not(v.begin(), v.end(),
                      [](int x) { return x > 5; });

// Count elements > 5
int cnt = count_if(v.begin(), v.end(),
                  [](int x) { return x > 5; });

// Check all positive
bool all_pos = all_of(v.begin(), v.end(),
                      [](int x) { return x > 0; });

// Check any even
bool any_even = any_of(v.begin(), v.end(),
                       [](int x) { return x % 2 == 0; });

// Check none negative
bool none_neg = none_of(v.begin(), v.end(),
                        [](int x) { return x < 0; });

// Find longest sequence > 5
auto it3 = find_if(v.begin(), v.end(), 
                  [](int x) { return x > 5; });
auto it4 = find_if_not(it3, v.end(),
                       [](int x) { return x > 5; });
// Range [it3, it4) contains all > 5
```

---

## Practical Complex Examples

### Example 1: K Most Frequent Elements
```cpp
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>
using namespace std;

vector<int> topKFrequent(vector<int>& nums, int k) {
    // Count frequencies
    unordered_map<int, int> freq;
    for(int num : nums) freq[num]++;
    
    // Min heap
    auto cmp = [&](int a, int b) { return freq[a] > freq[b]; };
    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
    
    // Keep top k
    for(auto& [num, count] : freq) {
        pq.push(num);
        if(pq.size() > k) pq.pop();
    }
    
    // Extract result
    vector<int> result;
    while(!pq.empty()) {
        result.push_back(pq.top());
        pq.pop();
    }
    
    return result;
}
```

### Example 2: Largest Subarray Sum
```cpp
#include <algorithm>
#include <vector>
using namespace std;

int maxSubarraySum(const vector<int>& nums) {
    int maxSum = nums[0];
    int maxCurrent = nums[0];
    
    for(int i = 1; i < nums.size(); i++) {
        maxCurrent = max(nums[i], maxCurrent + nums[i]);
        maxSum = max(maxSum, maxCurrent);
    }
    
    return maxSum;
}
```

### Example 3: Merge Overlapping Intervals
```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<pair<int, int>> mergeIntervals(vector<pair<int, int>>& intervals) {
    if(intervals.empty()) return {};
    
    // Sort by start point
    sort(intervals.begin(), intervals.end());
    
    vector<pair<int, int>> merged;
    merged.push_back(intervals[0]);
    
    for(int i = 1; i < intervals.size(); i++) {
        if(intervals[i].first <= merged.back().second) {
            // Overlapping: merge
            merged.back().second = max(merged.back().second, intervals[i].second);
        } else {
            // Non-overlapping: add new interval
            merged.push_back(intervals[i]);
        }
    }
    
    return merged;
}
```

---

## Performance Tips

```cpp
// ❌ Inefficient
vector<int> result;
for(int i = 0; i < src.size(); i++) {
    if(src[i] > 5) {
        result.push_back(src[i]);  // Multiple reallocations
    }
}

// ✅ Efficient
vector<int> result;
result.reserve(src.size());  // Pre-allocate
copy_if(src.begin(), src.end(), back_inserter(result),
       [](int x) { return x > 5; });

// ✅ More efficient for sorted data
vector<int> result;
result.reserve(count_if(src.begin(), src.end(), 
                       [](int x) { return x > 5; }));
copy_if(src.begin(), src.end(), back_inserter(result),
       [](int x) { return x > 5; });
```

---

## Key Takeaways

✅ Use partial_sort for k largest/smallest elements  
✅ Use nth_element for median-like operations  
✅ Heap operations for priority queue needs  
✅ Set operations on sorted ranges for efficiency  
✅ Transform and accumulate for functional-style code  
✅ Partitioning for separating elements by criteria  
✅ Always consider pre-allocation for vectors  

**Next**: [Custom Comparators](10_Custom_Comparators.md)
