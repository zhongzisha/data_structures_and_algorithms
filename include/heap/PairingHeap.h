#ifndef PAIRINGHEAP_H
#define PAIRINGHEAP_H

#include <iostream>
#include <queue>
#include "MyCommon.h"

namespace test {

template <typename E>
class PairingHeap {
public:
  class Node {
  public:
    static int used_count;
    E _element;
    Node* _leftChild;
    Node* _nextSib;

    Node(const E& element)
      : _element(element),
      _leftChild(nullptr),
      _nextSib(nullptr){
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

      if (node->_leftChild != nullptr) {
        out << "L(" << node->_leftChild->_element << ")_";
      }

      out << "{" << node->_element << "}";

      if (node->_nextSib != nullptr) {
        out << "_R(" << node->_nextSib->_element << ")";
      }

      return out;
    }

  };

public:
  int _size;
  Node* _root;

  PairingHeap() : _size(0), _root(nullptr) {
    std::cout << "PairingHeap::PairingHeap()\n";
  }

  ~PairingHeap() {
    DeleteTreeIteratively(_root);
    std::cout << "PairingHeap::~PairingHeap()\n";
  }

  bool IsEmpty() {
    return _size == 0;
  }

  void SwapNode(Node* &x, Node* &y) {
    Node* tmp = x;
    x = y;
    y = tmp;
  }

  void AddChild(Node* &x, Node* &y) {
    if (x->_leftChild == nullptr) {
      x->_leftChild = y;
    } else {
      y->_nextSib = x->_leftChild;
      x->_leftChild = y;
    }
  }

  Node* Merge(Node* x, Node* y) {
    if (x == nullptr) return y;
    if (y == nullptr) return x;

    if (x->_element < y->_element) {
      AddChild(x, y);
      return x;
    } else {
      AddChild(y, x);
      return y;
    }

    return nullptr;
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

  Node* TwoPassMerge(Node *node) {
    if (node == nullptr || node->_nextSib == nullptr) {
      return node;
    } else {
      Node *A, *B, *newNode;
      A = node;
      B = node->_nextSib;
      newNode = node->_nextSib->_nextSib;
      A->_nextSib = nullptr;
      B->_nextSib = nullptr;
      return Merge(Merge(A, B), TwoPassMerge(newNode));
    }
  }

  E Pop() {

    if (_root == nullptr) return E();

    E element = _root->_element;
    Node* left = _root->_leftChild;

    delete _root;
    _size--;

    _root = TwoPassMerge(left);
    return element;

  }


  // 迭代删除一棵树的所有节点, O(n) time, O(1) space
  void DeleteTreeIteratively(Node* root) {
    while (root != nullptr) {
      if (root->_leftChild != nullptr) {
        Node* left = root->_leftChild;
        root->_leftChild = root->_leftChild->_nextSib;
        left->_nextSib = root;
        root = left;
      } else {
        Node* right = root->_nextSib;
        delete root; root = nullptr;
        root = right;
      }
    }
  }

  friend std::ostream& operator<<(std::ostream& out, PairingHeap* h) {
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

      if (node->_leftChild != nullptr) {
        q.push(node->_leftChild);
      }
      if (node->_nextSib != nullptr) {
        q.push(node->_nextSib);
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

#endif // PAIRINGHEAP_H
