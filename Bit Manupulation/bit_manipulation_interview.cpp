// Bit Manipulation Interview Questions in C++
#include <iostream>
using namespace std;

// 1. Count set bits
int countSetBits(int n) {
    int count = 0;
    while(n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}
// 2. Check if power of two
bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n-1)) == 0;
}
// 3. Find single non-repeating element
int singleNumber(int arr[], int n) {
    int res = 0;
    for(int i=0; i<n; i++) res ^= arr[i];
    return res;
}
// 4. Swap two numbers without temp
void swap(int &a, int &b) {
    a ^= b; b ^= a; a ^= b;
}
// 5. Get ith bit
int getIthBit(int n, int i) {
    return (n >> i) & 1;
}
// 6. Find two non-repeating elements in array (all others repeat twice)
void twoNonRepeating(int arr[], int n, int &x, int &y) {
    int xor2 = 0;
    for(int i=0; i<n; i++) xor2 ^= arr[i];
    int set_bit = xor2 & ~(xor2-1);
    x = y = 0;
    for(int i=0; i<n; i++) {
        if(arr[i] & set_bit) x ^= arr[i];
        else y ^= arr[i];
    }
}
// 7. Find the only number occurring odd times
int oddOccurring(int arr[], int n) {
    int res = 0;
    for(int i=0; i<n; i++) res ^= arr[i];
    return res;
}
// 8. Reverse bits of a number (32-bit)
unsigned int reverseBits(unsigned int n) {
    unsigned int rev = 0;
    for(int i=0; i<32; i++) {
        rev <<= 1;
        rev |= (n & 1);
        n >>= 1;
    }
    return rev;
}
// 9. Next higher number with same set bits
int nextHigherSameSetBits(int n) {
    int c = n, c0 = 0, c1 = 0;
    while(((c & 1) == 0) && (c != 0)) { c0++; c >>= 1; }
    while((c & 1) == 1) { c1++; c >>= 1; }
    if(c0 + c1 == 31 || c0 + c1 == 0) return -1;
    int pos = c0 + c1;
    n |= (1 << pos);
    n &= ~((1 << pos) - 1);
    n |= (1 << (c1 - 1)) - 1;
    return n;
}
// 10. Count total set bits from 1 to n
int countTotalSetBits(int n) {
    int count = 0;
    for(int i=1; i<=n; i++) {
        int x = i;
        while(x) { x &= (x-1); count++; }
    }
    return count;
}
// 11. Position of rightmost set bit (1-based)
int rightmostSetBitPos(int n) {
    if(n == 0) return 0;
    return __builtin_ctz(n) + 1;
}

int main() {
    int n = 29;
    cout << "Set bits in " << n << ": " << countSetBits(n) << endl;
    cout << n << (isPowerOfTwo(n) ? " is " : " is not ") << "a power of two\n";
    int arr[] = {2, 3, 2, 4, 4};
    cout << "Single non-repeating: " << singleNumber(arr, 5) << endl;
    int a = 5, b = 7;
    swap(a, b);
    cout << "After swap: a=" << a << ", b=" << b << endl;
    cout << "3rd bit of " << n << ": " << getIthBit(n, 3) << endl;
    // 6. Two non-repeating elements
    int arr2[] = {2, 4, 7, 9, 2, 4};
    int x, y;
    twoNonRepeating(arr2, 6, x, y);
    cout << "Two non-repeating: " << x << ", " << y << endl;
    // 7. Odd occurring
    int arr3[] = {1, 2, 3, 2, 3, 1, 3};
    cout << "Odd occurring: " << oddOccurring(arr3, 7) << endl;
    // 8. Reverse bits
    unsigned int num = 11;
    cout << "Reverse bits of 11: " << reverseBits(num) << endl;
    // 9. Next higher with same set bits
    cout << "Next higher with same set bits as 5: " << nextHigherSameSetBits(5) << endl;
    // 10. Total set bits from 1 to 7: ";
    cout << countTotalSetBits(7) << endl;
    // 11. Rightmost set bit position of 18: ";
    cout << rightmostSetBitPos(18) << endl;
    return 0;
}
