#include<stdio.h>
#include<string.h>


void removeduplicate(char *str) {
    int seen[256] = {0};   // Track ASCII characters
    int index = 0;         // Position to place unique characters

    for (int i = 0; str[i] !='\0'; i++) {
        unsigned char ch = str[i];
        if (!seen[ch]) {   // If character not seen before
            seen[ch] = 1;  // Mark as seen
            str[index++] = ch; // Keep it
        }
    }
    str[index] = '\0';     // Null-terminate the new string
}


int main()
{
    char str[] = "programming";
    removeduplicate(str);
    printf("String after removing duplicates: %s\n", str);
    return 0;
}