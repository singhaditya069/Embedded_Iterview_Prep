#include<stdio.h>
#include<string.h>

void consicutive(char *str)
{
    int len = strlen(str);
    int cnt = 1;
    for(int i =1; i<=len; i++)
    {
        if(str[i]==str[i-1])
        {
            cnt++;
        }
        else
        {
            printf("%c%d",str[i-1],cnt);
            cnt = 1;
        }
    }
}

int main()
{
    char str[]="aaabbbbcc";
    consicutive(str);
    return 0;
}