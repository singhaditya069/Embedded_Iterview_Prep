// OOPS Concepts in C++ for Competitive Interviews
#include <iostream>
using namespace std;

/********** 1. Class and Object **********/
class Person {
private: // Encapsulation
    string name;
    int age;
public:
    // Constructor
    Person(string n, int a) : name(n), age(a) {}
    // Destructor
    ~Person() { cout << "Person destroyed\n"; }
    // Getter (Abstraction)
    string getName() { return name; }
    int getAge() { return age; }
    // Setter
    void setAge(int a) { age = a; }
    // Operator Overloading
    bool operator==(const Person& other) {
        return name == other.name && age == other.age;
    }
    // Function Overloading
    void show() { cout << "Name: " << name << ", Age: " << age << endl; }
    void show(string prefix) { cout << prefix << name << ", Age: " << age << endl; }
};

/********** 2. Inheritance **********/
class Employee : public Person {
    int empId;
public:
    Employee(string n, int a, int id) : Person(n, a), empId(id) {}
    void show() { cout << "Employee ID: " << empId << ", Name: " << getName() << ", Age: " << getAge() << endl; }
};

/********** 3. Polymorphism **********/
class Shape {
public:
    virtual void area() { cout << "Shape area" << endl; }
};
class Circle : public Shape {
    int r;
public:
    Circle(int radius) : r(radius) {}
    void area() override { cout << "Circle area: " << 3.14 * r * r << endl; }
};
class Rectangle : public Shape {
    int w, h;
public:
    Rectangle(int width, int height) : w(width), h(height) {}
    void area() override { cout << "Rectangle area: " << w * h << endl; }
};

/********** 4. Abstraction & Encapsulation **********/
// Already shown in Person class

/********** 5. Competitive Interview Example **********/
// Find the sum of ages of all employees
int sumAges(Employee arr[], int n) {
    int sum = 0;
    for(int i=0; i<n; i++) sum += arr[i].getAge();
    return sum;
}

int main() {
    // Class/Object/Constructor/Destructor/Encapsulation/Abstraction/Operator Overloading
    Person p1("Alice", 30), p2("Bob", 25);
    p1.show();
    p2.show("Person: ");
    cout << "Are p1 and p2 same? " << (p1 == p2 ? "Yes" : "No") << endl;

    // Inheritance
    Employee e1("Charlie", 28, 101), e2("Diana", 32, 102);
    e1.show();
    e2.show();

    // Polymorphism
    Shape* s1 = new Circle(5);
    Shape* s2 = new Rectangle(4, 6);
    s1->area();
    s2->area();
    delete s1;
    delete s2;

    // Competitive Example
    Employee arr[2] = {e1, e2};
    cout << "Sum of ages: " << sumAges(arr, 2) << endl;

    return 0;
}
