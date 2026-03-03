#include <iostream>
using namespace std;

int main() {
    int x = 5;
    int *p = &x;      // single pointer
    int **pp = &p;    // double pointer

    cout << "x = " << x << endl;
    cout << "*p = " << *p << endl;
    cout << "**pp = " << **pp << endl;

    // modify value through double pointer
    **pp = 10;
    cout << "After modification, x = " << x << endl;

    return 0;
}
