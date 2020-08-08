#ifndef CIRCLEDEQUE_H
#define CIRCLEDEQUE_H


#include <iostream>
#include "MyCommon.h"

namespace test {

template <typename E>
class CircleDeque {
private:
  static const int DEFAULT_CAPACITY = 3;
  int _front;
  int _size;
  int _capacity;
  E* _elements;

private:
  int Index(int index, int capacity) const {
    // return (_front + index) % _capacity;

    index += _front;
    if (index < 0) {
      return index + capacity;
    }
    return index - ((index >= capacity) ? capacity : 0);
  }

public:
  CircleDeque() : _front(0),
                  _size(0),
                  _capacity(DEFAULT_CAPACITY),
                  _elements(new E[_capacity]) {
    std::cout << "CircleDeque::CircleDeque()\n";
  }

  ~CircleDeque() {
    if (_elements != nullptr) {
      delete[] _elements;
      _elements = nullptr;
    }
    std::cout << "CircleDeque::~CircleDeque()\n";
  }



  int Size() {
    return _size;
  }

  void Clear() {
    if (_elements != nullptr) {
      delete[] _elements;
      _elements = nullptr;
    }
    _front = 0;
    _size = 0;
    _capacity = 0;
  }

  bool IsEmpty() {
    return _size == 0;
  }

  void EnsureCapacity(int capacity) {
    if (_capacity >= capacity) return;

    std::cout << "enlarge the capacity\n";
    int _old_capacity = _capacity;
    _capacity += (_capacity >> 1);
    E* newElements = new E[_capacity];
    for (int i = 0; i < _size; i++) {
      // newElements[i] = _elements[(i + _front) % _old_capacity];
      newElements[i] = _elements[Index(i, _old_capacity)];
    }
    delete[] _elements;
    _elements = newElements;
    _front = 0;
  }

  void EnQueueRear(const E& element) {
    EnsureCapacity(_size + 1);

    // _elements[(_front + _size) % _capacity] = element;
    _elements[Index(_size, _capacity)] = element;
    _size++;
  }

  void EnQueueFront(const E& element) {
    EnsureCapacity(_size + 1);

    // _elements[(_front + _size) % _capacity] = element;
    _front = Index(-1, _capacity);
    _elements[_front] = element;
    _size++;
  }

  E DeQueueFront() {
    if (_size <= 0)
      throw std::out_of_range("Empty queue.");
    E frontElement = _elements[_front];
    // _front = (_front + 1) % _capacity;
    _front = Index(1, _capacity);
    _size--;
    return frontElement;
  }

  E DeQueueRear() {
    if (_size <= 0)
      throw std::out_of_range("Empty queue.");
    int rearIndex = Index(_size - 1, _capacity);
    E rearElement = _elements[rearIndex];
    _size--;
    return rearElement;
  }

  E Front() {
    return _elements[_front];
  }

  E Rear() {
    return _elements[Index(_size - 1, _capacity)];
  }

  friend std::ostream& operator<<(std::ostream& out, const CircleDeque& rhs) {
    out << "Capacity = " << rhs._capacity << ", Size = " << rhs._size << ", Front = " << rhs._front << ", [";
    //    for (int i = 0; i < rhs._capacity; i++) {
    //      if (i != 0) {
    //        out << ", ";
    //      }
    //      out << rhs._elements[i];
    //    }
    for (int i = 0; i < rhs._size; i++) {
      if (i != 0) {
        out << ", ";
      }
      out << rhs._elements[rhs.Index(i, rhs._capacity)];
    }
    out << "]";
    return out;
  }

};
}

#endif // CIRCLEDEQUE_H
