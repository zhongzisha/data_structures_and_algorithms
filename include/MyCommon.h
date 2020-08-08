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

}

#endif // MYCOMMON_H
