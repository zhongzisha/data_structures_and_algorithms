#ifndef DEQUE_H
#define DEQUE_H


#include <iostream>
#include "MyCommon.h"
#include "list/LinkedList.h"

namespace test {

template <typename E>
class Deque {
private:
  LinkedList<E>* _list;
public:
  Deque() : _list(new LinkedList<E>()) {

  }

  ~Deque() {
    _list->Clear();

    delete _list;
    _list = nullptr;
  }

  int Size() {
    return _list->Size();
  }

  bool IsEmpty() {
    return _list->IsEmpty();
  }

  void EnQueueFront(const E& element) {
    _list->Add(0, element);
  }

  E DeQueueFront() {
    return _list->Remove(0);
  }

  void EnQueueRear(const E& element) {
    _list->Add(element);
  }

  E DeQueueRear() {
    return _list->Remove(_list->Size() - 1);
  }

  E Front() {
    return _list->Get(0);
  }

  E Rear() {
    return _list->Get(_list->Size() - 1);
  }

  void Clear() {
    _list->Clear();
  }

  friend std::ostream& operator<<(std::ostream& out, const Deque& rhs) {
    //std::cout << "Queue::operator<<\n";
    out << rhs._list;
    return out;
  }
};
}

#endif // DEQUE_H
