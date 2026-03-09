#include<stdio.h>
#include<string.h>

checkAnagram(char *str1, char *str2)
{
    int count[26] = {0};
    for(int i = 0; str1[i]!='\0'; i++)
    {
        count[str1[i]-'a']++;
    }
    for(int i = 0; str2[i]!='\0'; i++)
    {
        count[str2[i]-'a']--;
    }
    for(int i = 0; i<26; i++)
    {
        if(count[i]!=0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    char str1[] = "silent";
    char str2[] = "listen";
    if(checkAnagram(str1, str2)==1)
    {
        printf("The strings are anagrams.");
    }
    else
    {
        printf("The strings are not anagrams.");
    }
    return 0;
}