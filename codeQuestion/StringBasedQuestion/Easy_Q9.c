#include<stdio.h>
#include<string.h>

void reverse(char *str)
{
    int len = strlen(str);
    char * token =  strtok(str, " ");
    char srr[100][100];
    int cnt = 0;
    while(token != NULL)
    {
        strcpy(srr[cnt++], token);
        token = strtok(NULL, " ");
    }

    for(int i = cnt-1; i>=0; i--)
    {
        printf("%s ", srr[i]);
    }
}

int main()
{
    char str[] = "Hello World";
    reverse(str);
    return 0;
}