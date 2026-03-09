#include<stdio.h>
#include<string.h>

void occurence(char *str, int len)
{
    char count[10];
    int cnt = 0;
    for(int i = 0; i<len; i++)
    {
        for(int j = 0; j<len; j++)
        {
            if(str[i]==str[j])
            {
                cnt++;
            }          
        }
        count[i] = cnt;
        printf("%d",count[i]);
        cnt = 0;
    }
}

int main()
{
    char str[] = "NITINS";
    int len = strlen(str);
    occurence(str, len);
    return 0;
}