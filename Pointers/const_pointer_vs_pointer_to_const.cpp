#include <iostream>
using namespace std;

int main() {
    int x = 10;
    int y = 20;

    const int *ptrToConst = &x;   // pointer to const int (value cannot be changed through pointer)
    int *const constPtr = &x;     // const pointer to int (pointer cannot change)
    const int *const both = &x;   // const pointer to const int

    cout << "ptrToConst points to: " << *ptrToConst << endl;
    // *ptrToConst = 15; // error: cannot modify value
    ptrToConst = &y; // ok, pointer can change

    cout << "constPtr points to: " << *constPtr << endl;
    *constPtr = 25; // ok, value can change
    // constPtr = &y; // error: pointer cannot change

    cout << "both points to: " << *both << endl;
    // *both = 30; // error
    // both = &y; // error

    return 0;
}
