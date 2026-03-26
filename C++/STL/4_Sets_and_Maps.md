# 4. Sets and Maps - Associative Containers

## Sets - Unique Sorted Elements

A **set** is a container of unique elements automatically sorted by value.

### Set vs Vector/List

| Feature | Vector | List | Set |
|---------|--------|------|-----|
| Storage | Sequential | Linked | Sorted Binary Tree |
| Duplicates | Allowed | Allowed | Not allowed |
| Access | O(1) | O(n) | O(log n) |
| Insert | O(n) | O(1) | O(log n) |
| Search | O(n) | O(n) | O(log n) |
| Sorted | No | No | Yes |

---

## Creating and Using Sets

```cpp
#include <set>
using namespace std;

// Create sets
set<int> s1;                        // Empty set
set<int> s2 = {5, 2, 8, 1, 9, 2};   // Duplicates removed automatically
set<string> s3 = {"apple", "banana", "apple"};  // Only 2 unique strings

// Insert elements
s1.insert(5);       // Insert single element
s1.insert({1, 2, 3});  // Insert multiple elements

// Size and checks
s1.size();          // Number of unique elements
s1.empty();         // Boolean: is empty?
s1.count(5);        // Returns 0 or 1 (element exists?)

// Find
auto it = s1.find(5);
if(it != s1.end()) {
    cout << "Found: " << *it;
}

// Erase
s1.erase(5);                    // Erase by value
s1.erase(s1.find(5));           // Erase by iterator
s1.erase(s1.begin());           // Erase first element
s1.clear();                     // Remove all

// Access (iterators only, no random access)
for(int elem : s1) {
    cout << elem << " ";  // Automatically sorted
}

// Reverse iteration
for(auto it = s1.rbegin(); it != s1.rend(); ++it) {
    cout << *it << " ";
}
```

---

## Set Operations

```cpp
set<int> s1 = {1, 2, 3, 4, 5};
set<int> s2 = {3, 4, 5, 6, 7};

// Find first element >= given value
auto it1 = s1.lower_bound(3);  // Points to 3
auto it2 = s1.upper_bound(3);  // Points to 4

// Count occurrences (returns 0 or 1)
cout << s1.count(3);           // True (1)

// Range operations
for(auto it = s1.lower_bound(2); it != s1.upper_bound(4); ++it) {
    cout << *it << " ";        // Prints: 2 3 4
}
```

---

## Multiset - Allows Duplicates

```cpp
#include <set>
using namespace std;

multiset<int> ms = {1, 2, 2, 3, 3, 3};

ms.count(3);        // Returns 3 (count of 3s)
ms.erase(3);        // Removes ALL 3s
ms.erase(ms.find(3));  // Removes only ONE 3

for(int elem : ms) {
    cout << elem << " ";  // 1 2 2 3 3
}
```

---

## Maps - Key-Value Pairs

A **map** stores unique keys with associated values, sorted by key.

### Map Types

| Map Type | Key Unique | Sorted | Time |
|----------|-----------|--------|------|
| map | Yes | Yes (by key) | O(log n) |
| multimap | No | Yes | O(log n) |
| unordered_map | Yes | No | O(1) average |
| unordered_multimap | No | No | O(1) average |

---

## Creating and Using Maps

```cpp
#include <map>
using namespace std;

// Create maps
map<string, int> m1;                           // Empty map
map<string, int> m2 = {{"apple", 5}, {"banana", 3}};
map<int, string> m3 = {{1, "one"}, {2, "two"}};

// Insert elements
m1["key1"] = 100;           // Using operator[]
m1.insert({"key2", 200});   // Using insert()
m1.insert(make_pair("key3", 300));  // Using make_pair()

// Access elements
m1["key1"];                 // Returns 100
m1.at("key1");              // Safe access (throws if not found)
m1.count("key1");           // Returns 0 or 1

// Find
auto it = m1.find("key1");
if(it != m1.end()) {
    cout << "Key: " << it->first << ", Value: " << it->second;
}

// Erase
m1.erase("key1");           // Erase by key
m1.erase(m1.find("key1"));  // Erase by iterator
m1.clear();                 // Remove all

// Size
m1.size();
m1.empty();

// Iteration
for(auto& [key, value] : m1) {  // C++17 structured bindings
    cout << key << " -> " << value << endl;
}

// Traditional iteration
for(auto it = m1.begin(); it != m1.end(); ++it) {
    cout << it->first << " -> " << it->second << endl;
}
```

---

## Map vs Unordered_Map

```cpp
#include <map>
#include <unordered_map>

// map: Sorted by key, slower insertion but consistent O(log n)
map<string, int> sorted_map;
sorted_map["zebra"] = 1;
sorted_map["apple"] = 2;
sorted_map["banana"] = 3;
// Iteration order: apple, banana, zebra (alphabetically sorted)

// unordered_map: Faster on average, no guaranteed order
unordered_map<string, int> hash_map;
hash_map["zebra"] = 1;
hash_map["apple"] = 2;
hash_map["banana"] = 3;
// Iteration order: undefined (hash-based)

// unordered_map is faster for:
// - Large datasets with random access
// - When order doesn't matter

// map is better for:
// - Iterating in sorted order
// - Range queries
// - When consistency matters
```

---

## Practical Examples

### Example 1: Word Frequency Counter
```cpp
#include <map>
#include <string>
#include <iostream>
using namespace std;

int main() {
    map<string, int> wordCount;
    string word;
    
    while(cin >> word) {
        wordCount[word]++;  // Auto-creates if not exists
    }
    
    for(auto& [word, count] : wordCount) {
        cout << word << ": " << count << endl;
    }
    return 0;
}
```

### Example 2: Unique Elements using Set
```cpp
#include <set>
#include <vector>
using namespace std;

vector<int> removeNotUnique(vector<int>& nums) {
    set<int> unique(nums.begin(), nums.end());
    return vector<int>(unique.begin(), unique.end());
}
```

### Example 3: Character Frequency with Map
```cpp
#include <map>
#include <string>
using namespace std;

map<char, int> charFrequency(string s) {
    map<char, int> freq;
    for(char c : s) {
        freq[c]++;
    }
    return freq;
}
```

### Example 4: Find Pairs with Target Sum using Map
```cpp
#include <unordered_map>
#include <vector>
using namespace std;

vector<pair<int, int>> findPairs(vector<int>& nums, int target) {
    unordered_map<int, int> seen;
    vector<pair<int, int>> result;
    
    for(int num : nums) {
        int complement = target - num;
        if(seen.count(complement)) {
            result.push_back({complement, num});
            seen.erase(complement);
        } else {
            seen[num]++;
        }
    }
    
    return result;
}
```

### Example 5: Anagram Grouping
```cpp
#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

map<string, vector<string>> groupAnagrams(vector<string>& words) {
    map<string, vector<string>> groups;
    
    for(auto& word : words) {
        string sorted_word = word;
        sort(sorted_word.begin(), sorted_word.end());
        groups[sorted_word].push_back(word);
    }
    
    return groups;  // Anagrams grouped together
}
```

---

## Set Operations for Problem Solving

```cpp
#include <set>
#include <algorithm>

set<int> s1 = {1, 2, 3, 4, 5};
set<int> s2 = {3, 4, 5, 6, 7};

// Intersection
vector<int> intersection;
set_intersection(s1.begin(), s1.end(), 
                 s2.begin(), s2.end(),
                 back_inserter(intersection));
// Result: {3, 4, 5}

// Union
vector<int> unionSet;
set_union(s1.begin(), s1.end(),
          s2.begin(), s2.end(),
          back_inserter(unionSet));
// Result: {1, 2, 3, 4, 5, 6, 7}

// Difference
vector<int> difference;
set_difference(s1.begin(), s1.end(),
               s2.begin(), s2.end(),
               back_inserter(difference));
// Result: {1, 2}
```

---

## Performance Comparison

| Operation | set | map | unordered_set | unordered_map |
|-----------|-----|-----|----------------|----------------|
| Insert | O(log n) | O(log n) | O(1) avg | O(1) avg |
| Find | O(log n) | O(log n) | O(1) avg | O(1) avg |
| Erase | O(log n) | O(log n) | O(1) avg | O(1) avg |
| Sorted | Yes | Yes | No | No |
| Space | O(n) | O(n) | O(n) | O(n) |

---

## Key Takeaways

✅ Use **set** for unique sorted elements  
✅ Use **map** for key-value associations with sorted keys  
✅ Use **unordered_map** for fast key-value lookup without sorting  
✅ Use **multiset/multimap** to allow duplicate keys  
✅ Map and set use binary search trees internally (log n operations)  
✅ Unordered_map/set use hash tables (O(1) average)  

**Next**: [Iterators](5_Iterators.md)
