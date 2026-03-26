#include <stdio.h>
#include <stdbool.h>

bool oppositeSigns(int a, int b) {
    return (a ^ b) < 0;
}

int main() 
{
    int x = 5, y = -10;
    if (oppositeSigns(x, y))
        printf("%d and %d have opposite signs\n", x, y);
    else
        printf("%d and %d have same sign\n", x, y);
    return 0;
}
