#include <iostream>
#include <stack>
using namespace std;

bool isBalanced(string s) {
    stack<char> st;
    for(char c : s) {
        if(c == '(') st.push(c);
        else if(c == ')') {
            if(st.empty()) return false;
            st.pop();
        }
    }
    return st.empty();
}

int main() {
    string s;
    cout << "Enter parentheses string: ";
    cin >> s;
    if(isBalanced(s))
        cout << "Balanced" << endl;
    else
        cout << "Not Balanced" << endl;
    return 0;
}
