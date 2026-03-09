#include<stdio.h>
#include<stdint.h>

int main()
{
    uint32_t num = 0x12345678;
    // uint32_t Byte1, Byte2, Byte3, Byte4;
    // Byte1 = (num>>24) & 0xFF;
    // Byte2 = (num>>16) & 0xFF;
    // Byte3 = (num>>8) & 0xFF;
    // Byte4 = num & 0xFF;

    uint32_t reversedNum = (num>>24)& 0x000000FF |
                            (num>>8) & 0x0000FF00 |
                            (num<<8) & 0x00FF0000 |
                            (num<<24) & 0xFF000000;
                            
    printf("Original Number: 0x%X\n", num);
    printf("Reversed Number: 0x%X\n", reversedNum);
}