# 3. Lists and Deques - Sequential Containers

## Understanding Lists (Linked Lists)

A **list** is a doubly-linked list where each element stores data and pointers to previous/next elements.

### List vs Vector

| Feature | Vector | List |
|---------|--------|------|
| Access Time | O(1) | O(n) |
| Insert/Delete at end | O(1) | O(1) |
| Insert/Delete at middle | O(n) | O(1) |
| Insert/Delete at front | O(n) | O(1) |
| Memory | Contiguous | Scattered |

---

## Creating and Using Lists

```cpp
#include <list>
using namespace std;

// Create lists
list<int> l1;                        // Empty list
list<int> l2 = {1, 2, 3, 4, 5};     // With initialization
list<int> l3(5);                     // 5 elements with value 0
list<int> l4(5, 10);                 // 5 elements with value 10

// Size and capacity
l1.size();              // Number of elements
l1.empty();             // Boolean: is empty?

// Add elements
l1.push_back(10);       // Add at end
l1.push_front(5);       // Add at front

// Remove elements
l1.pop_back();          // Remove last
l1.pop_front();         // Remove first
l1.erase(l1.begin());   // Remove specific element
l1.clear();             // Remove all

// Access
l1.front();             // First element
l1.back();              // Last element
// NO RANDOM ACCESS - cannot use l1[0]

// Insert at position
auto it = l1.begin();
++it;  // Move iterator
l1.insert(it, 100);     // Insert before position

// Reverse
l1.reverse();

// Remove duplicate elements
l1.sort();
l1.unique();            // Removes consecutive duplicates

// Merge sorted lists
list<int> l5 = {1, 3, 5};
list<int> l6 = {2, 4, 6};
l5.merge(l6);  // l5 now contains merged list
```

---

## List Operations Example

```cpp
#include <iostream>
#include <list>
using namespace std;

int main() {
    list<int> myList = {10, 20, 30};
    
    // Add elements
    myList.push_back(40);      // [10, 20, 30, 40]
    myList.push_front(5);      // [5, 10, 20, 30, 40]
    
    // Iterate with iterator
    cout << "Elements: ";
    for(auto it = myList.begin(); it != myList.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;              // 5 10 20 30 40
    
    // Iterate with range-based for
    cout << "Reverse iteration: ";
    for(auto it = myList.rbegin(); it != myList.rend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;              // 40 30 20 10 5
    
    // Remove element
    myList.erase(myList.begin());  // Remove first
    
    // Get front and back
    cout << "First: " << myList.front() << endl;   // 10
    cout << "Last: " << myList.back() << endl;     // 40
    
    return 0;
}
```

---

## Deque - Double Ended Queue

A **deque** (double-ended queue) allows fast insertion/deletion at both ends while maintaining O(1) random access.

### Deque vs Vector vs List

| Operation | Vector | Deque | List |
|-----------|--------|-------|------|
| Front access | O(1) | O(1) | O(1) |
| Back access | O(1) | O(1) | O(1) |
| Random access | O(1) | O(1) | O(n) |
| Front insert | O(n) | O(1) | O(1) |
| Back insert | O(1) | O(1) | O(1) |
| Middle insert | O(n) | O(n) | O(1) |

---

## Creating and Using Deques

```cpp
#include <deque>
using namespace std;

// Create deques
deque<int> dq1;                     // Empty
deque<int> dq2 = {1, 2, 3, 4, 5};   // Initialized
deque<int> dq3(5, 10);              // 5 elements with value 10

// Add elements
dq1.push_back(10);      // Add at back
dq1.push_front(5);      // Add at front

// Remove elements
dq1.pop_back();         // Remove from back
dq1.pop_front();        // Remove from front

// Access
dq1[0];                 // Random access like vector
dq1.at(0);              // Safe access
dq1.front();            // First element
dq1.back();             // Last element

// Insert/Erase
auto it = dq1.begin();
dq1.insert(it, 100);    // Insert at position
dq1.erase(it);          // Erase at position

// Size
dq1.size();
dq1.empty();
dq1.clear();
```

---

## Deque Use Cases

```cpp
// Example 1: Sliding Window Maximum

#include <deque>
#include <vector>
using namespace std;

vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq;  // Stores indices
    vector<int> result;
    
    for(int i = 0; i < nums.size(); ++i) {
        // Remove elements outside the window
        while(!dq.empty() && dq.front() < i - k + 1) {
            dq.pop_front();
        }
        
        // Remove smaller elements from back
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
```

---

## List vs Deque vs Vector - When to Use?

### Use Vector when:
- ✅ You need fast random access
- ✅ Most insertions/deletions are at the end
- ✅ You need cache locality

### Use Deque when:
- ✅ You need fast operations at both ends
- ✅ You need random access
- ✅ Front insertions are frequent

### Use List when:
- ✅ You frequently insert/delete in the middle
- ✅ You don't need random access
- ✅ Memory is more important than speed

---

## Practical Examples

### Example 1: Palindrome Check with Deque
```cpp
#include <deque>
#include <string>
using namespace std;

bool isPalindrome(string s) {
    deque<char> dq(s.begin(), s.end());
    
    while(dq.size() > 1) {
        if(dq.front() != dq.back()) {
            return false;
        }
        dq.pop_front();
        dq.pop_back();
    }
    return true;
}
```

### Example 2: Queue Implementation with Deque
```cpp
template<typename T>
class Queue {
    deque<T> dq;
public:
    void enqueue(T val) { dq.push_back(val); }
    T dequeue() {
        T val = dq.front();
        dq.pop_front();
        return val;
    }
    bool isEmpty() { return dq.empty(); }
};
```

### Example 3: Remove Elements from List
```cpp
#include <list>
using namespace std;

list<int> removeEvens(list<int>& lst) {
    for(auto it = lst.begin(); it != lst.end(); ) {
        if(*it % 2 == 0) {
            it = lst.erase(it);  // erase returns next iterator
        } else {
            ++it;
        }
    }
    return lst;
}
```

---

## Time Complexity Summary

| Operation | Vector | Deque | List |
|-----------|--------|-------|------|
| Access | O(1) | O(1) | O(n) |
| Insert/Delete (end) | O(1)* | O(1) | O(1) |
| Insert/Delete (front) | O(n) | O(1) | O(1) |
| Insert/Delete (middle) | O(n) | O(n) | O(1) |
| Search | O(n) | O(n) | O(n) |

*Vector amortized O(1) for push_back

---

## Key Takeaways

✅ Use **vector** for random access and most general purposes  
✅ Use **list** when you need frequent middle insertions/deletions  
✅ Use **deque** when you need fast operations at both ends + random access  
✅ Lists don't support random access (no iterator arithmetic)  
✅ Deques combine benefits of vectors and lists with slight memory overhead  
✅ Always use appropriate container for your use case

**Next**: [Sets and Maps](4_Sets_and_Maps.md)
