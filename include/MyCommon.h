#ifndef MYCOMMON_H
#define MYCOMMON_H

#include <iostream>

namespace test {

static const int ELEMENT_NOT_FOUND = -1;

class Person {
private:
  static int used_count;
  std::string _name;
  int _age;
public:
  Person() {
    used_count++;
    std::cout <<"Person::Person()\n";
  }
  Person(const std::string& name, int age)
      : _name(name), _age(age){
    used_count++;
    std::cout <<"Person::Person(const std::string& name, int age)\n";
  }

  Person(const Person& rhs) : _name(rhs._name), _age(rhs._age) {
    used_count++;
    std::cout <<"Person::Person(const Person& rhs)\n";
  }

  ~Person() {
    used_count--;
    std::cout <<"Person::~Person(), name = " << _name << " \n";
  }

  std::string GetName() const {
    return _name;
  }
  int GetAge() const {
    return _age;
  }
  void SetName(const std::string& name) {
    _name = name;
  }
  void SetAge(int age) {
    _age = age;
  }

  bool operator==(const Person& rhs) {
    std::cout << "invoke operator==\n";
    return _age == rhs._age && _name == rhs._name;
  }
  bool operator!=(const Person& rhs) {
    std::cout << "invoke operator!=\n";
    return _age != rhs._age || _name != rhs._name;
  }
  bool operator<(const Person& rhs) {
    std::cout << "invoke operator<\n";
    return _age < rhs._age;
  }
  bool operator>(const Person& rhs) {
    std::cout << "invoke operator>\n";
    return _age > rhs._age;
  }
  void operator=(const Person& rhs) {
    std::cout << "invoke operator=\n";
    _name = rhs._name;
    _age = rhs._age;
  }

  void operator=(const Person* rhs) {
    std::cout << "invoke operator= from Person*\n";
    _name = rhs->_name;
    _age = rhs->_age;
  }

  friend std::ostream& operator<<(std::ostream& out, const Person& rhs) {
    out << "Person(" << rhs._name << ", " << rhs._age << ")";
    return out;
  }

  friend std::ostream& operator<<(std::ostream& out, const Person* rhs) {
    out << "Person(" << rhs->_name << ", " << rhs->_age << ")";
    return out;
  }
};


template <typename E>
struct Point2D {
    static int used_count;
    E _x;
    E _y;
    Point2D() : _x(0), _y(0) {
        used_count++;
        std::cout << "Point2D(), used_count = " << used_count << "\n";
    }
    Point2D(const E& x, const E& y) : _x(x), _y(y) {
        used_count++;
        std::cout << "Point2D(const E& x, const E& y), used_count = " << used_count << "\n";
    }
    Point2D(E arr[]) : _x(arr[0]), _y(arr[1]) {
        used_count++;
        std::cout << "Point2D(int arr[]), used_count = " << used_count << "\n";
    }
    Point2D(const Point2D& other) {  // copy constructor
        used_count++;
        std::cout << "Point2D(const Point2D& other), used_count = " << used_count << "\n";
        _x = other._x;
        _y = other._y;
    }
    Point2D(Point2D& other) {  // copy constructor
        used_count++;
        std::cout << "Point2D(Point2D& other), used_count = " << used_count << "\n";
        _x = other._x;
        _y = other._y;
    }
    Point2D& operator=(const Point2D& other) { // assignment operator
        _x = other._x;
        _y = other._y;
        return *this;
    }
    Point2D& operator=(Point2D& other) { // assignment operator
        _x = other._x;
        _y = other._y;
        return *this;
    }
    Point2D(Point2D&& other) {  // move constructor
        if (this != &other) {
            used_count++;
            std::cout << "Point2D(Point2D&& other), used_count = " << used_count << "\n";
            _x = other._x;
            _y = other._y;
        }
    }

    virtual ~Point2D() {
        used_count--;
        std::cout << "~Point2D(), used_count = " << used_count << "\n";
    }

    Point2D operator-(const Point2D& other) {
        return Point2D(_x - other._x, _y - other._y);
    }
    Point2D operator-(Point2D& other) {
        return Point2D(_x - other._x, _y - other._y);
    }
    bool operator<(const Point2D& other) {
        return (_y < other._y) || (_y == other._y && _x < other._x);
    }
};


}

#endif // MYCOMMON_H
