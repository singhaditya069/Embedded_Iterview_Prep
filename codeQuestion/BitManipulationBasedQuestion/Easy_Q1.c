#include<stdio.h>
#include<stdint.h>

int main()
{
    uint8_t a = 0x03;
    if(a&1 == 1)
    {
        printf("Number is odd");
    }
    else
    {
        printf("Number is even");
    }
}