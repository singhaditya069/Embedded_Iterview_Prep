#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter a number: ";
    cin >> n;
    if(n > 0 && (n & (n-1)) == 0)
        cout << n << " is a power of two." << endl;
    else
        cout << n << " is not a power of two." << endl;
    return 0;
}
