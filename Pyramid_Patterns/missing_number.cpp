#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter n (array size should be n-1): ";
    cin >> n;
    int arr[n-1], sum = 0;
    cout << "Enter elements (from 1 to n, one missing): ";
    for(int i=0; i<n-1; i++) {
        cin >> arr[i];
        sum += arr[i];
    }
    int total = n*(n+1)/2;
    cout << "Missing number: " << total - sum << endl;
    return 0;
}
