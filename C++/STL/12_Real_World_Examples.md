# 12. Real-World STL Applications

## Problem 1: LRU Cache Implementation

```cpp
#include <unordered_map>
#include <list>
using namespace std;

class LRUCache {
    unordered_map<int, pair<int, list<int>::iterator>> cache;
    list<int> lru_order;
    int capacity;
    
public:
    LRUCache(int cap) : capacity(cap) {}
    
    int get(int key) {
        if(cache.find(key) == cache.end()) return -1;
        
        // Move to front (most recently used)
        lru_order.erase(cache[key].second);
        lru_order.push_front(key);
        cache[key].second = lru_order.begin();
        
        return cache[key].first;
    }
    
    void put(int key, int value) {
        if(cache.find(key) != cache.end()) {
            // Update existing
            lru_order.erase(cache[key].second);
        } else if(cache.size() == capacity) {
            // Evict LRU
            int lru_key = lru_order.back();
            lru_order.pop_back();
            cache.erase(lru_key);
        }
        
        lru_order.push_front(key);
        cache[key] = {value, lru_order.begin()};
    }
};
```

---

## Problem 2: String Tokenization

```cpp
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

// Method 1: Using stringstream
vector<string> tokenize(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while(getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

// Method 2: Manual parsing
vector<string> tokenizeManual(const string& str, char delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while(end != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    
    tokens.push_back(str.substr(start));
    return tokens;
}

// Usage
string sentence = "Hello,World,from,STL";
auto words = tokenize(sentence, ',');
// words: ["Hello", "World", "from", "STL"]
```

---

## Problem 3: Counting Word Frequencies

```cpp
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

map<string, int> countFrequencies(vector<string>& words) {
    map<string, int> frequency;
    
    for(const auto& word : words) {
        frequency[word]++;
    }
    
    return frequency;
}

// Get top k frequent words
vector<pair<string, int>> topKFrequent(vector<string>& words, int k) {
    unordered_map<string, int> freq;
    for(const auto& word : words) {
        freq[word]++;
    }
    
    vector<pair<string, int>> sorted_freq(freq.begin(), freq.end());
    
    // Sort by frequency (descending), then alphabetically
    sort(sorted_freq.begin(), sorted_freq.end(),
         [](const auto& a, const auto& b) {
             if(a.second != b.second) return a.second > b.second;
             return a.first < b.first;
         });
    
    vector<pair<string, int>> result(sorted_freq.begin(), 
                                     sorted_freq.begin() + k);
    return result;
}
```

---

## Problem 4: Merge Sorted Lists

```cpp
#include <vector>
#include <algorithm>
using namespace std;

vector<int> mergeSortedArrays(const vector<int>& arr1, 
                              const vector<int>& arr2) {
    vector<int> result;
    result.reserve(arr1.size() + arr2.size());
    
    merge(arr1.begin(), arr1.end(),
          arr2.begin(), arr2.end(),
          back_inserter(result));
    
    return result;
}

// Usage
vector<int> a1 = {1, 3, 5, 7};
vector<int> a2 = {2, 4, 6, 8};
auto merged = mergeSortedArrays(a1, a2);
// merged: {1, 2, 3, 4, 5, 6, 7, 8}
```

---

## Problem 5: Anagram Grouping

```cpp
#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

vector<vector<string>> groupAnagrams(vector<string>& words) {
    map<string, vector<string>> anagram_map;
    
    for(const auto& word : words) {
        string sorted_word = word;
        sort(sorted_word.begin(), sorted_word.end());
        anagram_map[sorted_word].push_back(word);
    }
    
    vector<vector<string>> result;
    for(auto& [key, group] : anagram_map) {
        result.push_back(group);
    }
    
    return result;
}

// Usage
vector<string> words = {"eat", "tea", "ate", "tan", "nat", "ant"};
auto grouped = groupAnagrams(words);
// Result: {{"eat", "tea", "ate"}, {"tan", "nat", "ant"}}
```

---

## Problem 6: Sliding Window Maximum

```cpp
#include <deque>
#include <vector>
using namespace std;

vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq;  // Stores indices
    vector<int> result;
    
    for(int i = 0; i < nums.size(); i++) {
        // Remove indices outside window
        while(!dq.empty() && dq.front() < i - k + 1) {
            dq.pop_front();
        }
        
        // Remove smaller elements
        while(!dq.empty() && nums[dq.back()] < nums[i]) {
            dq.pop_back();
        }
        
        dq.push_back(i);
        
        if(i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    
    return result;
}

// Usage
vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
auto max_vals = maxSlidingWindow(nums, 3);
// max_vals: {3, 3, 5, 5, 6, 7}
```

---

## Problem 7: Group by Key-Value

```cpp
#include <map>
#include <vector>
#include <string>
using namespace std;

struct Record {
    string category;
    string item;
    int value;
};

map<string, vector<string>> groupByCategory(const vector<Record>& records) {
    map<string, vector<string>> grouped;
    
    for(const auto& record : records) {
        grouped[record.category].push_back(record.item);
    }
    
    return grouped;
}

// Usage
vector<Record> records = {
    {"fruit", "apple", 10},
    {"fruit", "banana", 15},
    {"vegetable", "carrot", 5},
    {"fruit", "orange", 12}
};

auto grouped = groupByCategory(records);
// grouped["fruit"]: {"apple", "banana", "orange"}
// grouped["vegetable"]: {"carrot"}
```

---

## Problem 8: Priority Queue for Task Scheduling

```cpp
#include <queue>
#include <vector>
#include <string>
using namespace std;

struct Task {
    string name;
    int priority;
    
    bool operator<(const Task& other) const {
        return priority < other.priority;  // Max-heap
    }
};

vector<Task> processTasksByPriority(vector<Task>& tasks) {
    priority_queue<Task> pq(tasks.begin(), tasks.end());
    vector<Task> processed;
    
    while(!pq.empty()) {
        processed.push_back(pq.top());
        pq.pop();
    }
    
    return processed;
}

// Usage
vector<Task> tasks = {
    {"Task A", 2},
    {"Task B", 5},
    {"Task C", 1}
};

auto ordered = processTasksByPriority(tasks);
// ordered: {"Task B" (5), "Task A" (2), "Task C" (1)}
```

---

## Problem 9: Find Duplicate Elements

```cpp
#include <unordered_set>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> findDuplicates(const vector<int>& nums) {
    unordered_set<int> seen;
    unordered_set<int> duplicates;
    
    for(int num : nums) {
        if(seen.count(num)) {
            duplicates.insert(num);
        } else {
            seen.insert(num);
        }
    }
    
    return vector<int>(duplicates.begin(), duplicates.end());
}

// Alternative using set
vector<int> findDuplicatesAlt(const vector<int>& nums) {
    set<int> unique_seen;
    set<int> duplicates;
    
    for(int num : nums) {
        if(unique_seen.count(num)) {
            duplicates.insert(num);
        } else {
            unique_seen.insert(num);
        }
    }
    
    return vector<int>(duplicates.begin(), duplicates.end());
}
```

---

## Problem 10: Two Sum Problem

```cpp
#include <unordered_map>
#include <vector>
using namespace std;

vector<pair<int, int>> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> seen;
    vector<pair<int, int>> result;
    
    for(int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        
        if(seen.count(complement)) {
            result.push_back({seen[complement], i});
        }
        
        seen[nums[i]] = i;
    }
    
    return result;
}

// Usage
vector<int> nums = {2, 7, 11, 15, 3, 6};
auto pairs = twoSum(nums, 9);
// pairs: {(0, 1)} since nums[0] + nums[1] = 2 + 7 = 9
```

---

## Problem 11: Distinct Elements Only

```cpp
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> getUnique(vector<int> nums) {
    set<int> unique(nums.begin(), nums.end());
    return vector<int>(unique.begin(), unique.end());
}

// One-liner with algorithm
vector<int> getUniqueAlgo(vector<int> nums) {
    sort(nums.begin(), nums.end());
    nums.erase(unique(nums.begin(), nums.end()), nums.end());
    return nums;
}

// Usage
vector<int> nums = {1, 2, 2, 3, 3, 3};
auto unique_nums = getUnique(nums);
// unique_nums: {1, 2, 3}
```

---

## Problem 12: Reverse and Rotate

```cpp
#include <algorithm>
#include <vector>
using namespace std;

// Reverse
vector<int> reverseVector(vector<int> v) {
    reverse(v.begin(), v.end());
    return v;
}

// Rotate left by k positions
vector<int> rotateLeft(vector<int> v, int k) {
    k %= v.size();
    rotate(v.begin(), v.begin() + k, v.end());
    return v;
}

// Rotate right by k positions
vector<int> rotateRight(vector<int> v, int k) {
    k %= v.size();
    rotate(v.begin(), v.end() - k, v.end());
    return v;
}

// Usage
vector<int> v = {1, 2, 3, 4, 5};

auto rev = reverseVector(v);      // {5, 4, 3, 2, 1}
auto left = rotateLeft(v, 2);     // {3, 4, 5, 1, 2}
auto right = rotateRight(v, 2);   // {4, 5, 1, 2, 3}
```

---

## Performance Comparison Example

```cpp
#include <vector>
#include <algorithm>
#include <ctime>
#include <iostream>
using namespace std;

void demonstratePerformance() {
    vector<int> v;
    v.reserve(10000);
    
    // Method 1: push_back (fast due to reserve)
    auto start = clock();
    for(int i = 0; i < 10000; i++) {
        v.push_back(i);
    }
    cout << "Time with reserve: " << clock() - start << "ms" << endl;
    
    // Method 2: Without reserve
    vector<int> v2;
    start = clock();
    for(int i = 0; i < 10000; i++) {
        v2.push_back(i);
    }
    cout << "Time without reserve: " << clock() - start << "ms" << endl;
    
    // Method 3: Sorting
    start = clock();
    sort(v.begin(), v.end());
    cout << "Time to sort: " << clock() - start << "ms" << endl;
}
```

---

## Key Takeaways

✅ STL provides pre-built solutions for common problems  
✅ Understand time/space tradeoffs (hashmap vs sorted)  
✅ Use appropriate containers for each problem  
✅ Leverage algorithms to reduce code complexity  
✅ Test performance with actual data sizes  
✅ Prefer STL over manual implementation  

**Congratulations!** You've completed the STL learning journey from basics to advanced.
