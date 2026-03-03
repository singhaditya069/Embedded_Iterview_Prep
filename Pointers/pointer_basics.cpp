#include <iostream>
using namespace std;

int main() {
    int x = 10;
    int *p = &x; // pointer holds address of x

    cout << "Value of x: " << x << endl;
    cout << "Pointer p pointing to x, *p: " << *p << endl;
    cout << "Address stored in p: " << p << endl;
    cout << "Address of x: " << &x << endl;

    return 0;
}
