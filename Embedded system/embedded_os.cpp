// Embedded OS: Task Scheduling Example
#include <iostream>
#include <queue>
using namespace std;

struct Task {
    int id, priority;
    Task(int i, int p) : id(i), priority(p) {}
    bool operator<(const Task& other) const { return priority < other.priority; }
};

int main() {
    priority_queue<Task> scheduler;
    scheduler.push(Task(1, 5));
    scheduler.push(Task(2, 2));
    scheduler.push(Task(3, 8));
    while(!scheduler.empty()) {
        Task t = scheduler.top(); scheduler.pop();
        cout << "Running Task " << t.id << " with priority " << t.priority << endl;
    }
    return 0;
}
