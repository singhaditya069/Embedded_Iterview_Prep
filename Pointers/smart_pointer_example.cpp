#include <iostream>
#include <memory>
using namespace std;

int main() {
    // unique_ptr example
    unique_ptr<int> uptr = make_unique<int>(100);
    cout << "unique_ptr value: " << *uptr << endl;

    // shared_ptr example
    shared_ptr<int> sptr1 = make_shared<int>(200);
    shared_ptr<int> sptr2 = sptr1; // share ownership
    cout << "shared_ptr use_count: " << sptr1.use_count() << endl;

    // weak_ptr example
    weak_ptr<int> wptr = sptr1;
    if (auto locked = wptr.lock()) {
        cout << "weak_ptr locked value: " << *locked << endl;
    }

    return 0;
}
