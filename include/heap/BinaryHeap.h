#ifndef BINARYHEAP_H
#define BINARYHEAP_H

#include <iostream>
#include <queue>
#include "MyCommon.h"

namespace test {

template <typename E>
class BinaryHeap {
public:
  int _size;
  int _capacity;
  E* _elements;
  static const int DEFAULT_CAPACITY = 10;

  BinaryHeap(int capacity)
      : _size(0),
        _capacity((capacity < DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : capacity),
        _elements(new E[_capacity]) {
    std::cout << "BinaryHeap::BinaryHeap(int capacity)\n";
  }

  BinaryHeap() : BinaryHeap(DEFAULT_CAPACITY) {
    std::cout << "BinaryHeap::BinaryHeap()\n";
  }

  BinaryHeap(E elements[], int length) : BinaryHeap(length) {
    std::cout << "BinaryHeap::BinaryHeap(E elements[], int length)\n";
    _size = length;
    for (int i = 0; i < length; i++) {
      _elements[i] = elements[i];
    }
    Heapify();
  }

  void Clear() {
    delete[] _elements;
    _elements = nullptr;
  }

  ~BinaryHeap() {
    std::cout << "BinaryHeap::~BinaryHeap()\n";
    if (_elements != nullptr) {
      Clear();
    }
  }

  int Size() {
    return _size;
  }

  bool IsEmpty() {
    return _size == 0;
  }

  void Add(const E& element) {
    EnsureCapacity(_size + 1);

    _elements[_size++] = element;

    SiftUp(_size - 1);

  }

  E Pop() {
    int lastIndex = --_size;
    E root = _elements[0];
    _elements[0] = _elements[lastIndex];
    SiftDown(0);
    return root;
  }

  E Remove() {
    int lastIndex = --_size;
    E root = _elements[0];
    _elements[0] = _elements[lastIndex];
    SiftDown(0);
    return root;
  }

  E Replace(const E& element) {
    E root;
    if (_size == 0) {
      _elements[0] = element;
      _size++;
    } else {
      root = _elements[0];
      _elements[0] = element;
      SiftDown(0);
    }
    return root;
  }

  int Compare(E e1, E e2) {
    return e1 - e2;
  }

  void Heapify() {
//    // 上滤
//    for (int i = 1; i < _size; i++) {
//      SiftUp(i);
//    }

    // 下滤
    for (int i = (_size >> 1) - 1; i >= 0; i--) {
      SiftDown(i);
    }
  }

  void SiftUp(int index) {
    // version 1
//    E element = _elements[index];
//    while (index > 0) {
//      int pindex = (index - 1) >> 1; // 父节点位置
//      E parent = _elements[pindex];
//      if (Compare(element, parent) <= 0) {
//        return;
//      } else {
//        E tmp = _elements[index];
//        _elements[index] = _elements[pindex];
//        _elements[pindex] = tmp;

//        index = pindex;
//      }
//    }

    // version 2 (代码重构)
    E element = _elements[index];
    while (index > 0) {
      int pindex = (index - 1) >> 1;
      E parent = _elements[pindex];
      if (Compare(element, parent) <= 0) {
        break;
      } else {
        // 没必要交换
        _elements[index] = parent;
        index = pindex;
      }
    }
    _elements[index] = element;
  }

  void SiftDown(int index) {
    E element = _elements[index];
    int half = _size >> 1;
    while (index < half) {
      int childIndex = (index << 1) + 1;
      E child = _elements[childIndex];
      int rightIndex = childIndex + 1;
      if (rightIndex < _size &&
          Compare(_elements[rightIndex], child) > 0) {
        childIndex = rightIndex;
        child = _elements[childIndex];
        // 上面两句可以用下面一句替换
        // child = _elements[childIndex = rightIndex];
      }

      if (Compare(element, child) >= 0) break;

      _elements[index] = child;
      index = childIndex;
    }

    _elements[index] = element;
  }

  void EnsureCapacity(int capacity) {
    int oldCapacity = _capacity;
    if (oldCapacity >= capacity) return;

    std::cout << "enlarge the capacity\n";
    _capacity = oldCapacity + (oldCapacity >> 1);
    E* newElements = new E[_capacity];
    for (int i = 0; i < _size; i++) {
      newElements[i] = _elements[i];
    }
    delete[] _elements;
    _elements = newElements;
  }

  const E& operator[](int index) const {
    return _elements[index];
  }

  void Print() {
    for(int i = 0; i < _size; i++) {
      std::cout << _elements[i];
      if (i < _size - 1)
        std::cout << ", ";
    }
    std::cout << "\n";
  }

  friend std::ostream& operator<<(std::ostream& out, BinaryHeap* rhs) {
    out << "Size = " << rhs->_size
        << ", Capacity = " << rhs->_capacity << "\n";

    std::queue<int> q;
    q.push(0);
    int levelSize = 1;
    int height = 0;
    while (!q.empty()) {
      int index = q.front();
      int pindex = index > 0 ? ((index - 1) >> 1) : -1;

      out << rhs->operator[](index);
      if (pindex >= 0) {
        out << "_P(" << rhs->operator[](pindex) << ")";
      }
      out << " ";

      int lindex = (index << 1) + 1;
      int rindex = lindex + 1;
      if (lindex >= 0 && lindex < rhs->_size) {
        q.push(lindex);
      }
      if (rindex >=0 && rindex < rhs->_size) {
        q.push(rindex);
      }
      q.pop();
      levelSize--;

      if (levelSize == 0) {
        height++;
        levelSize = q.size();
        out << "\n";
      }
    }
    out << "Height = " << height << "\n";

    return out;
  }

};

}


#endif // BINARYHEAP_H
