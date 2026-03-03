// Bit Manipulation Examples
#include <iostream>
using namespace std;

int main() {
    unsigned int x = 0b10101010;
    cout << "Original: " << x << endl;
    // Set 3rd bit
    x |= (1 << 2);
    cout << "Set 3rd bit: " << x << endl;
    // Clear 2nd bit
    x &= ~(1 << 1);
    cout << "Clear 2nd bit: " << x << endl;
    // Toggle 1st bit
    x ^= (1 << 0);
    cout << "Toggle 1st bit: " << x << endl;
    // Count set bits
    int count = 0, y = x;
    while(y) { count += y & 1; y >>= 1; }
    cout << "Set bits: " << count << endl;
    return 0;
}
