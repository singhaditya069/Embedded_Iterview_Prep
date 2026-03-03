#include <iostream>
using namespace std;

void greet() {
    cout << "Hello from greet() function." << endl;
}

int add(int a, int b) {
    return a + b;
}

int main() {
    // pointer to function with no params
    void (*funcPtr)() = &greet;
    funcPtr();

    // pointer to function with params and return
    int (*addPtr)(int, int) = &add;
    cout << "Result of addPtr(3,4): " << addPtr(3, 4) << endl;

    return 0;
}
