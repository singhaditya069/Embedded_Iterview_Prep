// Device Driver: Register Read/Write & ISR Simulation
#include <iostream>
using namespace std;

volatile uint32_t DEVICE_REG = 0;

void writeRegister(uint32_t val) {
    DEVICE_REG = val;
    cout << "Register written: " << DEVICE_REG << endl;
}
uint32_t readRegister() {
    cout << "Register read: " << DEVICE_REG << endl;
    return DEVICE_REG;
}
void ISR() {
    cout << "Interrupt Service Routine triggered!" << endl;
    DEVICE_REG++;
}

int main() {
    writeRegister(100);
    readRegister();
    ISR();
    readRegister();
    return 0;
}
