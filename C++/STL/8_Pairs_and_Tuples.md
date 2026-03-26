# 8. Pairs and Tuples

## Pairs - Two Elements Together

A **pair** holds exactly two elements of possibly different types.

### Creating and Using Pairs

```cpp
#include <utility>
using namespace std;

// Create pairs
pair<int, string> p1;                           // Default
pair<int, string> p2 = {10, "hello"};           // Initialization
pair<int, string> p3 = make_pair(20, "world");  // make_pair
pair<int, string> p4(30, "!");                  // Constructor

// Access elements
p2.first;           // 10
p2.second;          // "hello"

// Assignment
p2 = {40, "new"};

// Comparison
pair<int, int> a = {1, 2};
pair<int, int> b = {1, 2};
if(a == b) { }      // Lexicographic comparison

// Copy pair
pair<int, string> p5 = p2;
```

### Functions with Pairs

```cpp
// Function returning pair
pair<int, bool> divideWithRemainder(int a, int b) {
    return {a / b, a % b != 0};
}

auto result = divideWithRemainder(7, 3);
cout << result.first << " " << result.second;  // 2 1

// Function with pair parameter
void processPair(const pair<string, int>& p) {
    cout << p.first << ": " << p.second;
}

processPair({"Alice", 25});
```

### Practical Examples

#### Example 1: Return Multiple Values
```cpp
#include <utility>
#include <string>
using namespace std;

pair<bool, string> validateEmail(string email) {
    if(email.find('@') == string::npos) {
        return {false, "Invalid: missing @"};
    }
    return {true, "Valid email"};
}

auto [valid, message] = validateEmail("test@example.com");
cout << message;
```

#### Example 2: Two Sum Problem
```cpp
#include <utility>
#include <vector>
#include <unordered_map>
using namespace std;

pair<int, int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> seen;
    
    for(int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        if(seen.count(complement)) {
            return {seen[complement], i};
        }
        seen[nums[i]] = i;
    }
    
    return {-1, -1};
}
```

---

## Tuples - Multiple Elements Together

A **tuple** can hold any number of elements of different types (C++11 onwards).

### Creating and Using Tuples

```cpp
#include <tuple>
using namespace std;

// Create tuples
tuple<int, string, double> t1;                          // Default
tuple<int, string, double> t2 = {10, "hello", 3.14};    // Initialize
tuple<int, string, double> t3 = make_tuple(20, "world", 2.71);

// Access elements
int num = get<0>(t2);               // 10
string str = get<1>(t2);            // "hello"
double val = get<2>(t2);            // 3.14

// Create tuple from values
auto t4 = make_tuple(100, "test", 1.5);

// Tuple size
tuple_size<decltype(t2)>::value;    // 3

// Create empty tuple
tuple<> empty_t;
```

### Structured Bindings (C++17)

```cpp
#include <tuple>
using namespace std;

tuple<int, string, bool> getInfo() {
    return {42, "Hello", true};
}

// Unpack tuple into individual variables
auto [num, str, flag] = getInfo();
cout << num << " " << str << " " << flag;

// Structured bindings with pairs too
pair<string, int> p = {"Alice", 25};
auto [name, age] = p;
```

### Tuple Operations

```cpp
#include <tuple>
using namespace std;

tuple<int, string, double> t = {10, "hello", 3.14};

// Concatenate tuples
auto t1 = make_tuple(1, 2);
auto t2 = make_tuple("a", "b");
auto combined = tuple_cat(t1, t2);  // (1, 2, "a", "b")

// Compare tuples
tuple<int, string> a = {1, "x"};
tuple<int, string> b = {1, "x"};
if(a == b) { }                      // Lexicographic comparison

// Tuple from array
int arr[] = {1, 2, 3};
// Can't directly, but can use:
auto t3 = make_tuple(arr[0], arr[1], arr[2]);
```

### Practical Examples

#### Example 1: Return Multiple Values with Tuple
```cpp
#include <tuple>
using namespace std;

tuple<int, int, int> minMaxAverage(vector<int>& nums) {
    int minVal = *min_element(nums.begin(), nums.end());
    int maxVal = *max_element(nums.begin(), nums.end());
    int avg = accumulate(nums.begin(), nums.end(), 0) / nums.size();
    
    return make_tuple(minVal, maxVal, avg);
}

auto [min, max, avg] = minMaxAverage({1, 5, 3, 9, 2});
cout << "Min: " << min << ", Max: " << max << ", Avg: " << avg;
```

#### Example 2: 3D Point Structure
```cpp
#include <tuple>
#include <cmath>
using namespace std;

using Point3D = tuple<double, double, double>;

double distance(const Point3D& p1, const Point3D& p2) {
    auto [x1, y1, z1] = p1;
    auto [x2, y2, z2] = p2;
    
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    
    return sqrt(dx*dx + dy*dy + dz*dz);
}

Point3D p1 = {0, 0, 0};
Point3D p2 = {1, 1, 1};
cout << distance(p1, p2);  // 1.732...
```

#### Example 3: Key-Value-Flag Storage
```cpp
#include <tuple>
#include <vector>
#include <string>
using namespace std;

struct Config {
    vector<tuple<string, string, bool>> settings;
    
    void addSetting(string key, string value, bool enabled) {
        settings.push_back(make_tuple(key, value, enabled));
    }
    
    void printSettings() {
        for(auto& [key, value, enabled] : settings) {
            cout << key << ": " << value << " (" 
                 << (enabled ? "Enabled" : "Disabled") << ")" << endl;
        }
    }
};
```

---

## Tuple Helper Functions

```cpp
#include <tuple>
#include <iostream>
using namespace std;

// Print tuple elements (example implementation)
template<typename Tuple, size_t... I>
void printTupleImpl(const Tuple& t, index_sequence<I...>) {
    ((cout << (I == 0 ? "" : ", ") << get<I>(t)), ...);
}

template<typename... Args>
void printTuple(const tuple<Args...>& t) {
    cout << "(";
    printTupleImpl(t, index_sequence_for<Args...>{});
    cout << ")" << endl;
}

tuple<int, string, double> t = {1, "hello", 3.14};
printTuple(t);  // (1, hello, 3.14)
```

---

## When to Use Pairs vs Tuples

| Use Case | Pair | Tuple |
|----------|------|-------|
| Two values | ✅ | ✅ |
| Multiple values | ❌ | ✅ |
| Clarity | Good | Better |
| Performance | Best | Good |
| Standard maps/sets | ✅ | ❌ |

---

## Common Pair/Tuple Patterns

### Pattern 1: Map with Pair Values
```cpp
map<string, pair<int, int>> scores;
scores["Player1"] = {100, 50};
scores["Player2"] = {80, 60};

for(auto& [name, score] : scores) {
    cout << name << ": Wins=" << score.first << ", Loss=" << score.second;
}
```

### Pattern 2: Vector of Tuples for Data
```cpp
vector<tuple<string, int, double>> people = {
    {"Alice", 25, 5.8},
    {"Bob", 30, 6.1},
    {"Charlie", 22, 5.9}
};

for(auto& [name, age, height] : people) {
    cout << name << " (" << age << "): " << height << "ft" << endl;
}
```

### Pattern 3: Nested Pairs/Tuples
```cpp
// Dictionary of dictionaries
map<string, map<string, int>> data;
data["Section1"]["Item1"] = 10;
data["Section1"]["Item2"] = 20;

// Or with tuple:
vector<tuple<string, string, int>> data2 = {
    {"Section1", "Item1", 10},
    {"Section1", "Item2", 20}
};
```

---

## Key Takeaways

✅ Pair: Two elements of possibly different types  
✅ Tuple: Multiple elements of different types  
✅ Use pairs for simple two-value scenarios  
✅ Use tuples for complex multi-value returns  
✅ C++17 structured bindings make unpacking easy  
✅ make_pair() and make_tuple() for convenience  
✅ Tuples are lexicographically comparable  
✅ get<index>() for accessing tuple elements  

**Next**: [Advanced Algorithms](9_Algorithms_Advanced.md)
