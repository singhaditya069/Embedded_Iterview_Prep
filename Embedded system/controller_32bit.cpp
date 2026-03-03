// 32-bit Controller Register Simulation
#include <iostream>
using namespace std;

struct Register {
    uint32_t value;
    void setBit(int pos) { value |= (1 << pos); }
    void clearBit(int pos) { value &= ~(1 << pos); }
    void toggleBit(int pos) { value ^= (1 << pos); }
    bool readBit(int pos) { return (value >> pos) & 1; }
};

int main() {
    Register reg = {0};
    reg.setBit(5);
    reg.toggleBit(2);
    cout << "Register: " << reg.value << endl;
    cout << "Bit 5: " << reg.readBit(5) << endl;
    reg.clearBit(5);
    cout << "Register after clear: " << reg.value << endl;
    return 0;
}
