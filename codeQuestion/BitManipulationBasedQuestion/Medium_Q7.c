#include<stdio.h>
#include<stdint.h>

// int main()
// {
//     uint32_t num = 0x12345678;
//     // uint32_t Byte1, Byte2, Byte3, Byte4;
//     // Byte1 = (num>>24) & 0xFF;
//     // Byte2 = (num>>16) & 0xFF;
//     // Byte3 = (num>>8) & 0xFF;
//     // Byte4 = num & 0xFF;

//     uint32_t reversedNum = (num>>24)& 0x000000FF |
//                             (num>>8) & 0x0000FF00 |
//                             (num<<8) & 0x00FF0000 |
//                             (num<<24) & 0xFF000000;
                            
//     printf("Original Number: 0x%X\n", num);
//     printf("Reversed Number: 0x%X\n", reversedNum);
// }

// int main()
// {
//     uint8_t num = 0x36; // 0011 0110
//     uint8_t bit1,bit2,bit3,bit4,bit5,bit6,bit7,bit8;
//     bit1 = (num>>7) & 0x01; // 0
//     bit2 = (num>>6) & 0x01; // 0
//     bit3 = (num>>5) & 0x01; // 1
//     bit4 = (num>>4) & 0x01; // 0
//     bit5 = (num>>3) & 0x01; // 0
//     bit6 = (num>>2) & 0x01; // 1    
//     bit7 = (num>>1) & 0x01; // 0
//     bit8 = num & 0x01;      // 0

//     uint8_t reversedNum = (bit1<<0) | (bit2<<1) | (bit3<<2) | (bit4<<3) |
//                           (bit5<<4) | (bit6<<5) | (bit7<<6) | (bit8<<7);
//     printf("Original Number: 0x%X\n", num);
//     printf("Reversed Number: 0x%X\n", reversedNum);
//     return 0;
// }

int main()
{
    uint32_t num = 0x12345678;
    uint32_t reversedNum = 0;
    for(int i=0; i<32; i++){
        uint32_t bit = (num>>i) & 0x01; // Extract the i-th bit
        reversedNum |= (bit << (31 - i)); // Set the corresponding bit in the reversed number
    }
    printf("Original Number: 0x%X\n", num);
    printf("Reversed Number: 0x%X\n", reversedNum);
    return 0;
}


