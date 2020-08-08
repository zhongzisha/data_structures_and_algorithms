#ifndef LEFTISTHEAP_H
#define LEFTISTHEAP_H


#include <iostream>
#include <queue>
#include "MyCommon.h"

namespace test {

template <typename E>
class LeftistHeap {
public:
  class Node {
  public:
    static int used_count;
    E _element;
    Node* _left;
    Node* _right;
    int _npl;  // null path length

    Node(const E& element, Node* left, Node* right)
        : _element(element),
          _left(left),
          _right(right),
          _npl(0){
      used_count++;
      std::cout << "Node::Node(const E& element, Node* left, Node* right), element = " << _element
                << ", used_count = " << used_count << "\n";
    }

    Node(const E& element) : Node(element, nullptr, nullptr) {
      std::cout << "Node::Node(const E& element), element = " << _element
                << ", used_count = " << used_count << "\n";
    }

    ~Node() {
      used_count--;
      std::cout << "Node::~Node(), element = " << _element
                << ", used_count = " << used_count << "\n";
    }

    friend std::ostream& operator<<(std::ostream& out, Node* node) {
      if (node == nullptr) {
        out << "Empty Node.";
        return out;
      }

      if (node->_left != nullptr) {
        out << "L(" << node->_left->_element << "," << node->_left->_npl << ")_";
      }

      out << "{" << node->_element << "," << node->_npl << "}";

      if (node->_right != nullptr) {
        out << "_R(" << node->_right->_element << "," << node->_right->_npl << ")";
      }

      return out;
    }
  };
public:
  int _size;
  Node* _root;

  LeftistHeap() : _size(0), _root(nullptr) {
    std::cout << "LeftistHeap::LeftistHeap()\n";
  }

  ~LeftistHeap() {
    Clear();
    std::cout << "LeftistHeap::~LeftistHeap()\n";
  }

  int Compare(const E& e1, const E& e2) {
    return e1 - e2;
  }

  void Push(const E& element) {

    Node* newNode = new Node(element);
    if (newNode == nullptr) {
      throw "Bad allocation";
      return;
    }
    _size++;

    if (_root == nullptr) {
      _root = newNode;
    } else {
      _root = Merge(_root, newNode);
    }
  }

  void SwapNode(Node* &x, Node* &y) {
    Node* tmp = x;
    x = y;
    y = tmp;
  }

  Node* Merge(Node* x, Node* y) {
    if (x == nullptr) return y;
    if (y == nullptr) return x;

    if (x->_element > y->_element) {
      SwapNode(x, y);
    }

    x->_right = Merge(x->_right, y);

    if (x->_left == nullptr
        || x->_left->_npl < x->_right->_npl) {
      Node* tmp = x->_left;
      x->_left = x->_right;
      x->_right = tmp;
    }

    if (x->_right == nullptr || x->_left == nullptr) {
      x->_npl = 0;
    } else {
      x->_npl = (x->_left->_npl > x->_right->_npl)
                    ? (x->_right->_npl + 1) : (x->_left->_npl + 1);
    }

    return x;
  }

  void Merge(LeftistHeap<E>* other) {
    _root = Merge(_root, other->_root);
    _size += other->_size;
  }

  E Top() {
    if (_root != nullptr) {
      return _root->_element;
    } else {
      return E();
    }
  }

  E Pop() {
    if (_root == nullptr) {
      return E();
    }

    Node* left = _root->_left;
    Node* right = _root->_right;
    E element = _root->_element;
    delete _root;
    _size--;
    _root = Merge(left, right);
    return element;
  }

  void Clear() {
    if (_root == nullptr) return;

    DeleteTreeIteratively(_root);

  }

  bool IsEmpty() {
    return _size == 0;
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

  friend std::ostream& operator<<(std::ostream& out, LeftistHeap* h) {
    if (h == nullptr || h->IsEmpty()) {
      out << "Empty LeftistHeap.\n";
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

#endif // LEFTISTHEAP_H
