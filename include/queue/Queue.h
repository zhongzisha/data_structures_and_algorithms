#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "MyCommon.h"
#include "list/LinkedList.h"

namespace test {

template <typename E>
class Queue {
private:
  LinkedList<E>* _list;
public:
  Queue() : _list(new LinkedList<E>()) {

  }

  ~Queue() {
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

  void EnQueue(const E& element) {
    _list->Add(element);
  }

  E DeQueue() {
    return _list->Remove(0);
  }

  E Front() {
    return _list->Get(0);
  }

  void Clear() {
    _list->Clear();
  }

  friend std::ostream& operator<<(std::ostream& out, const Queue& rhs) {
    //std::cout << "Queue::operator<<\n";
    out << rhs._list;
    return out;
  }
};
}
#endif // QUEUE_H
