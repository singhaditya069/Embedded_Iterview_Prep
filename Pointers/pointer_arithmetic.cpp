#include <iostream>
using namespace std;

int main() {
    int arr[5] = {10, 20, 30, 40, 50};
    int *p = arr; // points to first element

    cout << "Array elements using pointer arithmetic:" << endl;
    for (int i = 0; i < 5; ++i) {
        cout << "*(p + " << i << ") = " << *(p + i) << endl;
    }

    // increment pointer
    p++;
    cout << "After increment, *p = " << *p << " (should be 20)" << endl;

    return 0;
}
