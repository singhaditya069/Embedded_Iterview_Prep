#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter array size: ";
    cin >> n;
    int arr[n], res = 0;
    cout << "Enter elements (all except one appear twice): ";
    for(int i=0; i<n; i++) {
        cin >> arr[i];
        res ^= arr[i];
    }
    cout << "Non-repeating element: " << res << endl;
    return 0;
}
