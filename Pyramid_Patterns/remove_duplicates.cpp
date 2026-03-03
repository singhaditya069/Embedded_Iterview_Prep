#include <iostream>
#include <set>
using namespace std;

int main() {
    int n;
    cout << "Enter array size: ";
    cin >> n;
    int arr[n];
    cout << "Enter elements: ";
    for(int i=0; i<n; i++) cin >> arr[i];
    set<int> s(arr, arr+n);
    cout << "Array after removing duplicates: ";
    for(auto x : s) cout << x << " ";
    cout << endl;
    return 0;
}
