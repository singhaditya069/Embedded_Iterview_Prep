#include<stdio.h>

int multiplyBy7(int n){
    return (n<<3) - n; // n*8 - n = n*7
}

int multiplyBy9(int n){
    return (n<<3) + n; // n*8 + n = n*9
}

int multiplyBy8(int n){
    return n<<3; // n*8
}

int main(){
    int a = 5;
    printf("%d",multiplyBy7(a));
    printf("\n%d",multiplyBy9(a));
    printf("\n%d",multiplyBy8(a));
    return 0;    
}