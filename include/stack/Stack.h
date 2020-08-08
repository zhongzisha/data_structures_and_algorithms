#ifndef STACK_H
#define STACK_H

#include <iostream>
#include "MyCommon.h"
#include "list/ArrayList.h"

namespace test {

template <typename E>
class Stack {
private:
  ArrayList<E>* _list;
public:
  Stack() : _list(new ArrayList<E>()) {

  }

  ~Stack() {
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

  void Push(const E& element) {
    _list->Add(element);
  }

  E Pop() {
    return _list->Remove(_list->Size() - 1);
  }

  E Top() {
    return _list->Get(_list->Size() - 1);
  }

  void Clear() {
    _list->Clear();
  }

  friend std::ostream& operator<<(std::ostream& out, const Stack& rhs) {
    //std::cout << "Stack::operator<<\n";
    out << rhs._list;
    return out;
  }
};

}

#endif // STACK_H
