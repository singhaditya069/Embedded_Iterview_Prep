#include<stdio.h>
#include<string.h>

char nonrepeating(char *str,int len)
{
    for(int i=0;i<len;i++)
    {
        int cnt = 0;
        for(int j=0;j<len;j++)
        {
            if(str[i] == str[j])
            {
                cnt++;
            }
        }
        if(cnt==1)
        {
            return str[i];
        }
    }
    return '\0';
}

int main()
{
    char str[] = "leetcode";
    int len;
    len = strlen(str);
    char w = nonrepeating(str,len);
    printf("%c",w);
    return 0;
}
