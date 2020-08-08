#ifndef ARRAYLISTOBJECT_H
#define ARRAYLISTOBJECT_H


#include <iostream>
#include "MyCommon.h"

namespace test {

template <typename E>
class ArrayListObject {
private:
  int _size;
  int _capacity;
  E* _elements;
  static const int DEFAULT_CAPACITY = 10;
public:
  ArrayListObject(int capacity)
      : _size(0),
        _capacity((capacity < DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : capacity),
        _elements(new E[_capacity]) {
    std::cout << "ArrayList(int capacity)\n";
  }
  ArrayListObject() : ArrayListObject(DEFAULT_CAPACITY) {
    std::cout << "ArrayList()\n";
  }

  void Clear() {
    for (int i = 0; i < _size; i++) {
      delete _elements[i];
      _elements[i] = nullptr;
    }
    delete[] _elements;
    _elements = nullptr;
  }
  ~ArrayListObject() {
    std::cout << "~ArrayList()\n";
    Clear();
  }

  int Size() {
    return _size;
  }
  bool IsEmpty() {
    return _size == 0;
  }
  bool Contains(const E& element) {
    return IndexOf(element) == ELEMENT_NOT_FOUND;
  }

  void Add(const E& element) {
    Add(_size, element);
  }

  void RangeCheck(int index) {
    if (index < 0 || index >= _size) {
      throw std::out_of_range("Index: "  + std::to_string(index) + ", Size: " + std::to_string(_size));
    }
  }

  void RangeCheckForAdd(int index) {
    if (index < 0 || index > _size) {
      throw std::out_of_range("Index: "  + std::to_string(index) + ", Size: " + std::to_string(_size));
    }
  }

  void Add(int index, const E& element) {
    RangeCheckForAdd(index);

    EnsureCapacity(_size + 1);
    std::cout << "Add\n";

    for (int i = _size; i > index; i--) {
      _elements[i] = _elements[i - 1];
    }
    _elements[_size++] = element;
  }

  E Remove(int index) {
    std::cout << "Before remove (size = " << _size << ")\n";
    RangeCheck(index);

    E old = _elements[index];
    for (int i = index + 1; i < _size; i++) {
      _elements[i - 1] = _elements[i];
    }
    // delete _elements[--_size];
    _size--;
    return old;
  }

  E Get(int index) {
    RangeCheck(index);

    return _elements[index];
  }

  E Set(int index, const E& element) {
    RangeCheck(index);

    E old = _elements[index];
    _elements[index] = element;
    return old;
  }

  int IndexOf(const E& element) {
    for (int i = 0; i < _size; i++) {
      if (_elements[i] == element) {  // need operator==
        return i;
      }
    }

    return ELEMENT_NOT_FOUND;
  }

  friend std::ostream& operator<<(std::ostream& out, const ArrayListObject& rhs) {
    out << "size = " << rhs._size << "\n";
    for(int i = 0; i < rhs._size; i++) {
      out << rhs._elements[i] << " ";
    }
    out << "\n";
    return out;
  }

private:
  void EnsureCapacity(int capacity) {
    int oldCapacity = _capacity;
    if (oldCapacity >= capacity) return;

    std::cout << "enlarge the capacity \n";
    _capacity = oldCapacity + (oldCapacity >> 1);
    E* newElements = new E[_capacity];
    for (int i = 0; i < _size; i++) {
      newElements[i] = _elements[i];
    }
    delete[] _elements;
    _elements = newElements;
  }
};

}

#endif // ARRAYLISTOBJECT_H
