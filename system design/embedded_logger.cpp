// System Design: Embedded Logger with Circular Buffer (C++)
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Logger {
    vector<string> buffer;
    int size, head;
public:
    Logger(int n) : buffer(n), size(n), head(0) {}
    void log(const string& msg) {
        buffer[head] = msg;
        head = (head + 1) % size;
    }
    void printLogs() {
        cout << "Logs (oldest to newest):\n";
        for(int i=0; i<size; i++) {
            int idx = (head + i) % size;
            if(!buffer[idx].empty())
                cout << buffer[idx] << endl;
        }
    }
};

int main() {
    Logger logger(3);
    logger.log("Init");
    logger.log("Sensor read");
    logger.log("Error: Timeout");
    logger.log("Recovered");
    logger.printLogs();
    return 0;
}
