#ifndef CARTESIANTREE_H
#define CARTESIANTREE_H

#include <iostream>
#include <queue>
#include "MyCommon.h"

namespace test {

template <typename E>
class CartesianTree {
public:
  class Node {
  public:
    static int used_count;
    E _key;
    int _priority;
    Node* _left;
    Node* _right;

    Node(const E& key, int priority)
      : _key(key), _priority(priority), _left(nullptr), _right(nullptr) {
      used_count++;
      std::cout << "Node:Node(const E& key, int priority), used_count = "
                << used_count << "\n";
    }

    ~Node() {
      used_count--;
      std::cout << "Node:~Node(), used_count = " << used_count << "\n";
    }
  };

public:
  int _size;
  Node* _root;

  CartesianTree() : _size(0), _root(nullptr) {
    std::cout << "CartesianTree::CartesianTree()\n";
  }
  ~CartesianTree() {
    std::cout << "CartesianTree::~CartesianTree()\n";
  }


};

}

#endif // CARTESIANTREE_H
