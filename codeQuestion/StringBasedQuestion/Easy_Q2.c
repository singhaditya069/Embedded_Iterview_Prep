#include <stdio.h>
#include <string.h>

int isPalindrome(const char *str) {
    int left = 0;
    int right = strlen(str) - 1;

    while (left < right) {
        if (str[left] != str[right]) {
            return 0;  // not a palindrome
        }
        left++;
        right--;
    }
    return 1;  // palindrome
}

int main() {
    char str[] = "NITINS";
    if (isPalindrome(str)) {
        printf("Palindrome\n");
    } else {
        printf("Not Palindrome\n");
    }

    return 0;
}
