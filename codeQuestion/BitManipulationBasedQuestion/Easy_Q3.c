#include<stdio.h>
#include<stdint.h>

int main()
{
    uint8_t n = 0x02;
    uint32_t cnt = 0;
    while(n)
    {
        n=n&(n-1);
        cnt++;
    }
    printf("set bits = %d",cnt);
}
