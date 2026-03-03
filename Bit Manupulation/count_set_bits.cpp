#include <iostream>
using namespace std;

int main() {
    int n, count = 0;
    cout << "Enter a number: ";
    cin >> n;
    while(n) {
        count += n & 1;
        n >>= 1;
    }
    cout << "Set bits: " << count << endl;
    return 0;
}
