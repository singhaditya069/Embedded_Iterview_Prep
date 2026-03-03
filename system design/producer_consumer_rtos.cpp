// System Design: Producer-Consumer Problem using RTOS Concepts (C++)
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

queue<int> buffer;
const unsigned int MAX_SIZE = 5;
mutex mtx;
condition_variable cv_full, cv_empty;
bool done = false;

void producer() {
    for(int i=1; i<=10; i++) {
        unique_lock<mutex> lock(mtx);
        cv_full.wait(lock, []{ return buffer.size() < MAX_SIZE; });
        buffer.push(i);
        cout << "Produced: " << i << endl;
        lock.unlock();
        cv_empty.notify_one();
    }
    done = true;
    cv_empty.notify_all();
}

void consumer() {
    while(true) {
        unique_lock<mutex> lock(mtx);
        cv_empty.wait(lock, []{ return !buffer.empty() || done; });
        if(buffer.empty() && done) break;
        int val = buffer.front(); buffer.pop();
        cout << "Consumed: " << val << endl;
        lock.unlock();
        cv_full.notify_one();
    }
}

int main() {
    thread prod(producer);
    thread cons(consumer);
    prod.join();
    cons.join();
    return 0;
}
