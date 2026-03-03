#include <iostream>
using namespace std;

int main() {
    string s;
    cout << "Enter a string: ";
    cin >> s;
    int l = 0, r = s.length() - 1;
    bool isPalindrome = true;
    while(l < r) {
        if(s[l] != s[r]) {
            isPalindrome = false;
            break;
        }
        l++; r--;
    }
    if(isPalindrome)
        cout << s << " is a palindrome." << endl;
    else
        cout << s << " is not a palindrome." << endl;
    return 0;
}
