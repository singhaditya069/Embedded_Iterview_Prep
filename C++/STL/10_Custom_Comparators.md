# 10. Custom Comparators and Sorting

## Introduction to Comparators

A **comparator** defines how to compare two elements. STL uses comparators in containers (set, map) and algorithms (sort, heap).

---

## Function Comparators

### Simple Function Pointer

```cpp
#include <algorithm>
#include <vector>
using namespace std;

// Comparator function: returns true if a < b
bool ascending(int a, int b) {
    return a < b;
}

bool descending(int a, int b) {
    return a > b;
}

int main() {
    vector<int> v = {5, 2, 8, 1, 9};
    
    sort(v.begin(), v.end(), ascending);   // {1, 2, 5, 8, 9}
    sort(v.begin(), v.end(), descending);  // {9, 8, 5, 2, 1}
    
    return 0;
}
```

### Built-in Comparators

```cpp
#include <algorithm>
#include <functional>
#include <vector>
using namespace std;

vector<int> v = {5, 2, 8, 1, 9};

// Ascending (default)
sort(v.begin(), v.end());                      // {1, 2, 5, 8, 9}
sort(v.begin(), v.end(), less<int>());         // {1, 2, 5, 8, 9}

// Descending
sort(v.begin(), v.end(), greater<int>());      // {9, 8, 5, 2, 1}

// Other comparators
sort(v.begin(), v.end(), less_equal<int>());
sort(v.begin(), v.end(), greater_equal<int>());
```

---

## Comparator Functors (Structs)

A **functor** is a class/struct with `operator()`.

### Basic Functor

```cpp
#include <algorithm>
#include <vector>
using namespace std;

struct CompareDescending {
    bool operator()(int a, int b) const {
        return a > b;
    }
};

int main() {
    vector<int> v = {5, 2, 8, 1, 9};
    
    sort(v.begin(), v.end(), CompareDescending());
    // {9, 8, 5, 2, 1}
    
    return 0;
}
```

### Functor with State

```cpp
#include <algorithm>
#include <vector>
using namespace std;

struct CompareWithOffset {
    int offset;
    
    CompareWithOffset(int off) : offset(off) {}
    
    bool operator()(int a, int b) const {
        return (a + offset) < (b + offset);
    }
};

vector<int> v = {5, 2, 8, 1, 9};

// Sort with offset of 10
sort(v.begin(), v.end(), CompareWithOffset(10));
// Results in same order as normal sort
```

---

## Custom Comparators for Complex Objects

### Example 1: Sort by Member Variable

```cpp
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

struct Person {
    string name;
    int age;
    
    Person(string n, int a) : name(n), age(a) {}
};

// Comparator struct
struct CompareByAge {
    bool operator()(const Person& a, const Person& b) const {
        return a.age < b.age;
    }
};

int main() {
    vector<Person> people = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 22}
    };
    
    sort(people.begin(), people.end(), CompareByAge());
    
    for(const auto& p : people) {
        cout << p.name << ": " << p.age << endl;
    }
    // Output:
    // Charlie: 22
    // Alice: 25
    // Bob: 30
    
    return 0;
}
```

### Example 2: Sort by Name (String)

```cpp
struct ComparePerson {
    bool operator()(const Person& a, const Person& b) const {
        return a.name < b.name;  // Lexicographic order
    }
};

sort(people.begin(), people.end(), ComparePerson());
// Alice, Bob, Charlie
```

### Example 3: Sort by Multiple Criteria

```cpp
struct ComparePersonMulti {
    bool operator()(const Person& a, const Person& b) const {
        // Sort by age first, then by name if ages are equal
        if(a.age != b.age) {
            return a.age < b.age;
        }
        return a.name < b.name;
    }
};

sort(people.begin(), people.end(), ComparePersonMulti());
```

---

## Lambda Comparators (C++11)

Lambdas are concise, inline comparators.

### Basic Lambda

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> v = {5, 2, 8, 1, 9};

sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;  // Descending
});
// {9, 8, 5, 2, 1}
```

### Lambda for Complex Objects

```cpp
struct Person {
    string name;
    int age;
};

vector<Person> people = {
    {"Alice", 25},
    {"Bob", 30},
    {"Charlie", 22}
};

// Sort by age
sort(people.begin(), people.end(), 
     [](const Person& a, const Person& b) {
         return a.age < b.age;
     });

// Sort by name length, then by name
sort(people.begin(), people.end(),
     [](const Person& a, const Person& b) {
         if(a.name.length() != b.name.length()) {
             return a.name.length() < b.name.length();
         }
         return a.name < b.name;
     });
```

---

## Comparators with Maps and Sets

### Map with Custom Comparator

```cpp
#include <map>
#include <string>
using namespace std;

struct Person {
    string name;
    int id;
};

// Comparator for map (must be strict weak ordering)
struct ComparePerson {
    bool operator()(const Person& a, const Person& b) const {
        return a.id < b.id;
    }
};

// Map with custom comparator
map<Person, string, ComparePerson> personMap;

Person p1{"Alice", 1};
Person p2{"Bob", 2};

personMap[p1] = "Engineer";
personMap[p2] = "Manager";

// Iterate (ordered by id)
for(auto& [person, job] : personMap) {
    cout << person.name << ": " << job << endl;
}
```

### Set with Custom Comparator

```cpp
#include <set>
#include <string>
using namespace std;

struct ComparePerson {
    bool operator()(const Person& a, const Person& b) const {
        if(a.age != b.age) return a.age < b.age;
        return a.name < b.name;
    }
};

// Set automatically keeps elements sorted by comparator
set<Person, ComparePerson> personSet;

personSet.insert({"Alice", 25});
personSet.insert({"Bob", 30});
personSet.insert({"Charlie", 25});

// Iterate in sorted order
for(const auto& p : personSet) {
    cout << p.name << ": " << p.age << endl;
}
```

---

## Comparator Traits and Best Practices

### Strict Weak Ordering Requirements

A valid comparator must satisfy:
1. **Irreflexive**: `comp(a, a)` returns false
2. **Antisymmetric**: If `comp(a, b)` then `!comp(b, a)`
3. **Transitive**: If `comp(a, b)` and `comp(b, c)` then `comp(a, c)`

```cpp
// ✅ Valid comparator
struct Valid {
    bool operator()(int a, int b) const {
        return a < b;  // Proper comparison
    }
};

// ❌ Invalid comparator
struct Invalid {
    bool operator()(int a, int b) const {
        return abs(a - b) > 5;  // Not transitive!
    }
};
```

---

## Practical Examples

### Example 1: Sort by Absolute Value

```cpp
vector<int> v = {-5, 3, -8, 2, -1};

sort(v.begin(), v.end(), 
     [](int a, int b) {
         return abs(a) < abs(b);
     });
// {-1, 2, 3, -5, -8}
```

### Example 2: Sort By Frequency

```cpp
#include <unordered_map>

vector<int> v = {1, 1, 1, 2, 2, 3};
unordered_map<int, int> freq;
for(int num : v) freq[num]++;

sort(v.begin(), v.end(),
     [&freq](int a, int b) {
         if(freq[a] != freq[b]) {
             return freq[a] > freq[b];  // Higher frequency first
         }
         return a < b;  // Then by value
     });
// Removes duplicates first: {1, 2, 3}
// {1, 1, 1, 2, 2, 3}
```

### Example 3: Case-Insensitive String Sort

```cpp
vector<string> words = {"Apple", "banana", "Cherry"};

sort(words.begin(), words.end(),
     [](const string& a, const string& b) {
         string lower_a = a, lower_b = b;
         transform(lower_a.begin(), lower_a.end(), 
                   lower_a.begin(), ::tolower);
         transform(lower_b.begin(), lower_b.end(),
                   lower_b.begin(), ::tolower);
         return lower_a < lower_b;
     });
// {"Apple", "banana", "Cherry"}
```

### Example 4: Sort Pairs by Second Element

```cpp
vector<pair<int, int>> pairs = {{3, 1}, {1, 3}, {2, 2}};

sort(pairs.begin(), pairs.end(),
     [](const pair<int, int>& a, const pair<int, int>& b) {
         return a.second < b.second;
     });
// {{3, 1}, {2, 2}, {1, 3}}
```

### Example 5: Custom Sort for LinkedList

```cpp
struct Node {
    int val;
    Node* next;
};

// Sort by value
auto cmp = [](Node* a, Node* b) {
    return a->val < b->val;
};

// Use with custom comparator in algorithms
// or manually implement merge sort with cmp
```

---

## Performance Notes

| Method | Speed | Clarity | Reusability |
|--------|-------|---------|-------------|
| Function pointer | Fast | Good | Good |
| Functor | Fast | Good | Good |
| Lambda | Fast | Excellent | Poor (inline) |
| Stateful functor | Fast | Good | Good |

---

## Key Takeaways

✅ Comparators define ordering for sort and containers  
✅ Use lambdas for simple, one-off comparators  
✅ Use functors for reusable, complex comparators  
✅ Must satisfy strict weak ordering  
✅ Multi-criteria sorting: compare in order  
✅ Can access external state with stateful functors/lambdas  
✅ Maps and sets use comparators for ordering  

**Next**: [STL Best Practices](11_STL_Best_Practices.md)
