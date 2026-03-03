// Bit Manipulation Interview Questions in C
#include <stdio.h>

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
int isPowerOfTwo(int n) {
    return n > 0 && (n & (n-1)) == 0;
}
// 3. Find single non-repeating element
int singleNumber(int arr[], int n) {
    int res = 0;
    for(int i=0; i<n; i++) res ^= arr[i];
    return res;
}
// 4. Swap two numbers without temp
void swap(int *a, int *b) {
    *a ^= *b; *b ^= *a; *a ^= *b;
}
// 5. Get ith bit
int getIthBit(int n, int i) {
    return (n >> i) & 1;
}

int main() {
    int n = 29;
    printf("Set bits in %d: %d\n", n, countSetBits(n));
    printf("%d %s a power of two\n", n, isPowerOfTwo(n) ? "is" : "is not");
    int arr[] = {2, 3, 2, 4, 4};
    printf("Single non-repeating: %d\n", singleNumber(arr, 5));
    int a = 5, b = 7;
    swap(&a, &b);
    printf("After swap: a=%d, b=%d\n", a, b);
    printf("3rd bit of %d: %d\n", n, getIthBit(n, 3));
    return 0;
}
