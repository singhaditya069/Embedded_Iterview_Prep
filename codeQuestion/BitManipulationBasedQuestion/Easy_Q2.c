#include<stdio.h>
#include<stdint.h>

int main()
{
    uint8_t a = 0x03;
    if(a>0 && (a&(a-1)) == 0)
    {
        printf("Number is power of 2");
    }
    else
    {
        printf("Number is not power of 2");
    }
}
