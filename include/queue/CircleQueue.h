#ifndef CIRCLEQUEUE_H
#define CIRCLEQUEUE_H

#include <iostream>
#include "MyCommon.h"

namespace test {

template <typename E>
class CircleQueue {
private:
  static const int DEFAULT_CAPACITY = 5;
  int _front;
  int _size;
  int _capacity;
  E* _elements;

private:
  int Index(int index, int capacity) const {
    // return (_front + index) % _capacity;

    index += _front;
    return index - ((index >= capacity) ? capacity : 0);
  }

public:
  CircleQueue() : _front(0),
                  _size(0),
                  _capacity(DEFAULT_CAPACITY),
                  _elements(new E[_capacity]) {
    std::cout << "CircleQueue::CircleQueue()\n";
  }

  ~CircleQueue() {
    if (_elements != nullptr) {
      delete[] _elements;
      _elements = nullptr;
    }
    std::cout << "CircleQueue::~CircleQueue()\n";
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

  void EnQueue(const E& element) {
    EnsureCapacity(_size + 1);

    // _elements[(_front + _size) % _capacity] = element;
    _elements[Index(_size, _capacity)] = element;
    _size++;
  }

  E DeQueue() {
    if (_size <= 0)
      throw std::out_of_range("Empty queue.");
    E frontElement = _elements[_front];
    // _front = (_front + 1) % _capacity;
    _front = Index(1, _capacity);
    _size--;
    return frontElement;
  }

  E Front() {
    return _elements[_front];
  }

  friend std::ostream& operator<<(std::ostream& out, const CircleQueue& rhs) {
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

#endif // CIRCLEQUEUE_H
