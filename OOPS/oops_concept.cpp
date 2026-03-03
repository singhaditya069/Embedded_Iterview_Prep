#include <iostream>

using namespace std;
class A 
{
    private: 
        int a;
        int b;
        int c;
    
    public:
    int scan_Record()
    {
        cout << "Input first Number: " << endl;
        cin >> a;
        cout << "Input Second Number: " << endl;
        cin >> b;
        c = a + b;
        return c ;
    }
    void print_Record()
    {
        cout << "Answer : " << c <<endl;
    }
}; 

int main (void)
{
    A a;
    a.scan_Record();
    cout << "Size : " << sizeof(a) <<endl;
    a.print_Record();
    return 0;
}