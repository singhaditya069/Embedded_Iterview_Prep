#include <stdio.h>

int add(int a, int b) {
    while (b != 0) {
        int sum = a ^ b;          // sum without carry
        int carry = (a & b) << 1; // carry
        a = sum;
        b = carry;
    }
    return a;
}

int main() 
{
    int x = 15, y = 27;
    printf("Sum = %d\n", add(x, y));
    return 0;
}

// a = 4 0100
// b = 5 0101
// a ^ b = 1 0001 (sum without carry)
// a & b = 4 0100 (carry)
// carry << 1 = 8 1000
// a = 1 0001
// b = 8 1000
// a ^ b = 9 1001 (sum without carry)
// a & b = 0 0000 (carry)
// carry << 1 = 0 0000
// a = 9 1001
// b = 0 0000
// a ^ b = 9 1001 (sum without carry)
// a & b = 0 0000 (carry)
// carry << 1 = 0 0000
// a = 9 1001
// b = 0 0000
// Sum = 9 1001 = 9 in decimal
