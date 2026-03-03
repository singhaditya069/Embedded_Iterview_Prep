// OOPS in Embedded
#include <iostream>
using namespace std;

class Sensor {
    int value;
public:
    Sensor() : value(0) {}
    void read() { value = 42; }
    int getValue() { return value; }
};

class TempSensor : public Sensor {
public:
    void calibrate() { cout << "Calibrated" << endl; }
};

int main() {
    TempSensor ts;
    ts.read();
    ts.calibrate();
    cout << "Value: " << ts.getValue() << endl;
    return 0;
}
