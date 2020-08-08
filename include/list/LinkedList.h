#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include "MyCommon.h"

namespace test {

template <typename E>
class LinkedList {
private:
  struct Node {
    Node* _prev;
    E _element;
    Node* _next;

    Node(Node* prev, E element, Node* next)
        : _prev(prev), _element(element), _next(next) {
    }

    Node() : Node(nullptr, E(), nullptr) {

    }

    friend std::ostream& operator<< (std::ostream& out, const Node& rhs) {
      if (rhs._prev != nullptr) {
        out << rhs._prev->_element;
      } else {
        out << "null";
      }

      out << "_" << rhs._element << "_";

      if (rhs._next != nullptr) {
        out << rhs._next->_element;
      } else {
        out << "null";
      }
      return out;
    }

    friend std::ostream& operator<< (std::ostream& out, const Node* rhs) {
      if (rhs->_prev != nullptr) {
        out << rhs->_prev->_element;
      } else {
        out << "null";
      }

      out << "_" << rhs->_element << "_";

      if (rhs->_next != nullptr) {
        out << rhs->_next->_element;
      } else {
        out << "null";
      }
      return out;
    }
  };

private:
  int _size;
  Node* _first;
  Node* _last;

public:
  LinkedList() : _size(0), _first(nullptr), _last(nullptr) {
    std::cout << "LinkedList()\n";
  }

  ~LinkedList() {
    std::cout << "~LinkedList()\n";
    Clear();
  }

  void Clear() {
    Node* tmpNode = _last;
    Node* prevNode = nullptr;
    for (int i = 0; i < _size; i++) {
      prevNode = tmpNode->_prev;
      delete tmpNode;
      tmpNode = prevNode;
    }
    _size = 0;
    _first = nullptr;
    _last = nullptr;
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
    std::cout << "Add\n";
    RangeCheckForAdd(index);

    if (index == _size) {
      Node* oldLast = _last;
      _last = new Node(_last, element, nullptr);
      if (oldLast == nullptr) {
        _first = _last;
      } else {
        oldLast->_next = _last;
      }
    } else {
      Node* nextNode = GetNode(index);
      Node* prevNode = nextNode->_prev;
      Node* currNode = new Node(prevNode, element, nextNode);
      nextNode->_prev = currNode;
      if (prevNode == nullptr) {
        _first = currNode;
      } else {
        prevNode->_next = currNode;
      }
    }
    _size++;
  }

  E Remove(int index) {
    std::cout << "Remove\n";
    RangeCheck(index);

    Node* currNode = GetNode(index);
    Node* prevNode = currNode->_prev;
    Node* nextNode = currNode->_next;
    if (prevNode == nullptr) {
      _first = nextNode;
    } else {
      prevNode->_next = nextNode;
    }

    if (nextNode == nullptr) {
      _last = prevNode;
    } else {
      nextNode->_prev = prevNode;
    }

    _size--;
    return currNode->_element;
  }

  E Get(int index) {
    RangeCheck(index);
    return GetNode(index)->_element;
  }

  E Set(int index, const E& element) {
    RangeCheck(index);

    Node* currNode = GetNode(index);
    E oldElement = currNode->_element;
    currNode->_element = element;
    return oldElement;
  }

  int IndexOf(const E& element) {
    Node* node = _first;
    for (int i = 0; i < _size; i++) {
      if (node->_element == element) {  // need operator==
        return i;
      }
      node = node->_next;
    }

    return ELEMENT_NOT_FOUND;
  }

  Node* GetNode(int index) {
    RangeCheck(index);

    if (index < (_size >> 1)) {
      Node* tmpNode = _first;
      for (int i = 0; i < index; i++) {
        tmpNode = tmpNode->_next;
      }
      return tmpNode;
    } else {
      Node* tmpNode = _last;
      for (int i = _size - 1; i > index; i--) {
        tmpNode = tmpNode->_prev;
      }
      return tmpNode;
    }
  }

  friend std::ostream& operator<<(std::ostream& out, const LinkedList& rhs) {
    Node* tmpNode = rhs._first;
    for (int i = 0; i < rhs._size; i++) {
      out << tmpNode << " ";
      tmpNode = tmpNode->_next;
    }
    return out;
  }

  friend std::ostream& operator<<(std::ostream& out, const LinkedList* rhs) {
    Node* tmpNode = rhs->_first;
    for (int i = 0; i < rhs->_size; i++) {
      out << tmpNode << " ";
      tmpNode = tmpNode->_next;
    }
    return out;
  }
};

}


#endif // LINKEDLIST_H
