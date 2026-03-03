// C++ Pointers and Memory Management
#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int* p = &a;
    cout << "Value: " << *p << ", Address: " << p << endl;
    int* arr = new int[5];
    for(int i=0; i<5; i++) arr[i] = i;
    cout << "Array: ";
    for(int i=0; i<5; i++) cout << arr[i] << " ";
    cout << endl;
    delete[] arr;
    return 0;
}
