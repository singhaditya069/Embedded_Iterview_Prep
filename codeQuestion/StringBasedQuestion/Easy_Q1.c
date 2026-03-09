#include<stdio.h>
#include<string.h>

void reverse(char *str, int len)
{
    char arr[len];
    for(int i = 0; i<len; i++)
    {
        arr[len-i-1] = str[i];
    }
    printf("%s",arr);
}

int main()
{
    char str[] = "hello";
    int len = strlen(str);
    reverse(str, len);
    return 0;
}