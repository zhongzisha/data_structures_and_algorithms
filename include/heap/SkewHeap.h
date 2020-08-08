#ifndef SKEWHEAP_H
#define SKEWHEAP_H

#include <iostream>
#include <queue>
#include "MyCommon.h"

namespace test {

template <typename E>
class SkewHeap {
public:
  class Node {
  public:
    static int used_count;
    E _element;
    Node* _left;
    Node* _right;

    Node(const E& element)
        : _element(element),
          _left(nullptr),
          _right(nullptr){
      used_count++;
      std::cout << "Node::Node(), element = " << _element
                << ", used_count = " << used_count << "\n";
    }

    ~Node() {
      used_count--;
      std::cout << "Node::~Node(), element = " << _element
                << ", used_count = " << used_count << "\n";
    }

    friend std::ostream& operator<<(std::ostream& out, Node* node) {
      if (node == nullptr) {
        out << "Empty Node.\n";
        return out;
      }

      if (node->_left != nullptr) {
        out << "L(" << node->_left->_element << ")_";
      }

      out << "{" << node->_element << "}";

      if (node->_right != nullptr) {
        out << "_R(" << node->_right->_element << ")";
      }

      return out;
    }

  };

public:
  int _size;
  Node* _root;

  SkewHeap() : _size(0), _root(nullptr) {
    std::cout << "SkewHeap::SkewHeap()\n";
  }

  ~SkewHeap() {
    DeleteTreeIteratively(_root);
    std::cout << "SkewHeap::~SkewHeap()\n";
  }

  bool IsEmpty() {
    return _size == 0;
  }

  void SwapNode(Node* &x, Node* &y) {
    Node* tmp = x;
    x = y;
    y = tmp;
  }

  Node* Merge(Node* &x, Node* &y) {
    if (x == nullptr) return y;
    if (y == nullptr) return x;

    if (x->_element > y->_element) {
      SwapNode(x, y);
    }

    Node* tmp = Merge(x->_right, y);
    x->_right = x->_left;
    x->_left = tmp;

    return x;
  }

  void Push(const E& element) {
    Node* newNode = new Node(element);
    if (newNode == nullptr) {
      throw "bad allocation";
      return;
    }

    _root = Merge(_root, newNode);
    _size++;
  }

  E Pop() {

    if (_root == nullptr) return E();

    Node* left = _root->_left;
    Node* right = _root->_right;

    E element = _root->_element;

    delete _root;
    _size--;

    _root = Merge(left, right);

    return element;

  }


  // 迭代删除一棵树的所有节点, O(n) time, O(1) space
  void DeleteTreeIteratively(Node* root) {
    while (root != nullptr) {
      if (root->_left != nullptr) {
        Node* left = root->_left;
        root->_left = root->_left->_right;
        left->_right = root;
        root = left;
      } else {
        Node* right = root->_right;
        delete root; root = nullptr;
        root = right;
      }
    }
  }

  friend std::ostream& operator<<(std::ostream& out, SkewHeap* h) {
    if (h == nullptr || h->IsEmpty()) {
      out << "Empty SkewHeap.\n";
      return out;
    }

    Node* node = h->_root;
    std::queue<Node*> q;
    q.push(node);
    int levelSize = q.size();
    int height = 0;
    while (!q.empty()) {
      node = q.front();

      std::cout << node << " ";

      if (node->_left != nullptr) {
        q.push(node->_left);
      }
      if (node->_right != nullptr) {
        q.push(node->_right);
      }

      q.pop();
      levelSize--;
      if (levelSize == 0) {
        levelSize = q.size();
        height++;
        out << "\n";
      }
    }

    out << "Size = " << h->_size << "\n"
        << "Height = " << height << "\n";

    return out;
  }

};

}


#endif // SKEWHEAP_H
