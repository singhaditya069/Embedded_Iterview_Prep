#include <iostream>
#include <vector>
#include <list>
#include <stack>
using namespace std;

int main(void)
{
    /***************Stack*****************/
    stack<string> cars;
    cars.push("Audi");
    cars.push("MSD");
    cars.push("alto");
    cars.pop();
    cars.push("thar");
    cout<< cars.top() << endl;
    cout<< cars.size() << endl;

    /***************List******************/ 
    // list<string> cars  = {"Volvo", "Msd", "Alto" , "Jeep"};
    // cars.push_back("Mersedeis");
    // cars.pop_back();
    // cars.push_front("Renult");
    // cars.pop_front();
    // //cars.empty();
    // ///cars.size();
    // cout << cars.size() <<endl;
    // for(string car: cars)
    // {
    //     cout<< car << "\n";
    // }

    /********** Vector **********/
    // vector<string> cars = {"Volvo", "Msd", "Alto" , "Jeep"};
    // cout << cars.at(1) <<endl;
    // cout << cars.at(2) <<endl;
    // cars.clear();
    // cars.push_back("Audi");
    // cars.pop_back();
    // cars[2] = {"Thor"};

    return 0; 
}