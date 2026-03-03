// System Design: UART Driver Simulation (C++)
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

class UART {
    queue<char> rx_buffer, tx_buffer;
    mutex mtx;
    condition_variable cv_rx, cv_tx;
public:
    void send(char c) {
        unique_lock<mutex> lock(mtx);
        tx_buffer.push(c);
        cout << "UART TX: " << c << endl;
        cv_tx.notify_one();
    }
    char receive() {
        unique_lock<mutex> lock(mtx);
        cv_rx.wait(lock, [&]{ return !rx_buffer.empty(); });
        char c = rx_buffer.front(); rx_buffer.pop();
        cout << "UART RX: " << c << endl;
        return c;
    }
    // Simulate hardware ISR for RX
    void isr_rx(char c) {
        unique_lock<mutex> lock(mtx);
        rx_buffer.push(c);
        cv_rx.notify_one();
    }
    // Simulate hardware ISR for TX
    void isr_tx() {
        unique_lock<mutex> lock(mtx);
        if(!tx_buffer.empty()) {
            char c = tx_buffer.front(); tx_buffer.pop();
            cout << "UART HW sent: " << c << endl;
        }
    }
};

int main() {
    UART uart;
    thread sender([&]{ uart.send('A'); uart.send('B'); });
    thread receiver([&]{ uart.isr_rx('X'); uart.receive(); });
    sender.join();
    receiver.join();
    uart.isr_tx();
    uart.isr_tx();
    return 0;
}
