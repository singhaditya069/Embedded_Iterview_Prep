#include <iostream>
using namespace std;

int main(void)
{
    int n , p;
    cout <<"star Pyramid"<< endl;
    cout <<"Enter the No.of stars: ";
    cin >> n;
    /********************Star Pyramid**********************/

    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < i+1; j++)
    //     {
    //         cout << "*" ;
    //     }
    //     cout << "\n";
    // } 
    
    /******************Star inverted pyramid ***************/
     p = n;
    for (int i = 0; i <= n; i++)
    {
        p--;
        for (int j = 0; j <= p; j++)
        {
            cout << "*" ;
        }
        cout << "\n";

    } 

    /******************Reverse Pyramid (centered)***************/
    for (int i = n; i >= 1; i--)
    {
        // Print leading spaces
        for (int j = 0; j < n - i; j++)
            cout << " ";
        // Print stars
        for (int j = 0; j < 2 * i - 1; j++)
            cout << "*";
        cout << "\n";
    }

    /******************Inverted Pyramid (centered)***************/
    for (int i = 0; i < n; i++)
    {
        // Print leading spaces
        for (int j = 0; j < i; j++)
            cout << " ";
        // Print stars
        for (int j = 0; j < 2 * (n - i) - 1; j++)
            cout << "*";
        cout << "\n";
    }

    /******************Palindrome Number Logic***************/
    int num, originalNum, reversedNum = 0, remainder;
    cout << "\nEnter a number to check palindrome: ";
    cin >> num;
    originalNum = num;
    while (num != 0)
    {
        remainder = num % 10;
        reversedNum = reversedNum * 10 + remainder;
        num /= 10;
    }
    if (originalNum == reversedNum)
        cout << originalNum << " is a palindrome number." << endl;
    else
        cout << originalNum << " is not a palindrome number." << endl;

    return 0;
}
