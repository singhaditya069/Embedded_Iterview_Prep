//Find position of rightmost set bit
#include<stdio.h>
#include<stdint.h>

int main()
{
    uint8_t num = 0x24;
    for(int i=0;i<8;i++)
    {
        if((num>>i)&1 ==1)
        {
            printf("%dth bit is set", (8-i));
            break;
        }
    }
    return 0;    
}
