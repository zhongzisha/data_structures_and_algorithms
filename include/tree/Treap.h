#ifndef TREAP_H
#define TREAP_H

#include <iostream>
#include <queue>
#include <cstdlib>    // for rand
#include <climits>
#include "MyCommon.h"

namespace test {

template <typename E>
class Treap {
public:
  class Node {
  public:
    static int used_count;
    E _element;
    Node* _left;
    Node* _right;
    int _priority;

    Node() : _left(nullptr), _right(nullptr), _priority(rand()) {
      used_count++;
      std::cout << "Node::Node(), used_count = " << used_count << "\n";
    }

    Node(const E& element)
      : _element(element),
        _left(nullptr),
        _right(nullptr),
        _priority(rand()){
      used_count++;
      std::cout << "Node::Node(), used_count = " << used_count << "\n";
    }

    Node(const E& element, Node* left, Node* right)
      : _element(element),
        _left(left),
        _right(right),
        _priority(rand()){
      used_count++;
      std::cout << "Node::Node(), used_count = " << used_count << "\n";
    }


    ~Node() {
      used_count--;
      std::cout << "Node::~Node(), used_count = " << used_count << "\n";
    }

    friend std::ostream& operator<<(std::ostream& out, const Node* rhs) {
      if (rhs == nullptr) {
        out << "null";
        return out;
      }
      if (rhs->_left != nullptr) {
        out << "L" << rhs->_left->_element;
      } else {
        out << "null";
      }

      out << "_(" << rhs->_element << ")_P(" << rhs->_priority << ")";

      if (rhs->_right != nullptr) {
        out << "_R" << rhs->_right->_element;
      } else {
        out << "_null";
      }
      return out;
    }
  };


public:
  int _size;
  Node* _root;
  Node* _nullNode;

  Treap() {
    std::cout << "Treap::Treap()\n";

    _size = 0;
    _nullNode = new Node();
    _nullNode->_left = _nullNode;
    _nullNode->_right = _nullNode;
    _nullNode->_priority = INT_MAX;
    _root = _nullNode;

  }

  virtual ~Treap() {
    DeleteTreeIteratively(_root);
    delete _nullNode;
    std::cout << "Treap::~Treap()\n";
  }

  int Size() {
    return _size;
  }

  bool IsEmpty() {
    return _size == 0;
  }

  int Compare(const E& e1, const E& e2) {
    return e1 - e2;
  }

  // 迭代删除一棵树的所有节点, O(n) time, O(1) space
  void DeleteTreeIteratively(Node* root) {
    while (root != _nullNode) {
      if (root->_left != _nullNode) {
        Node* left = root->_left;
        root->_left = root->_left->_right;
        left->_right = root;
        root = left;
      } else {
        Node* right = root->_right;
        delete root; root = _nullNode;
        root = right;
      }
    }
  }

  bool Contains(const E& element) {
    if (_size == 0) {
      return false;
    }

    Node* node = _root;
    int cmp;
    while (true) {
      cmp = Compare(element, node->_element);
      if (cmp > 0) {
        node = node->_right;
      } else if (cmp < 0) {
        node = node->_left;
      } else {
        return node != _nullNode;
      }
    }
  }

  void Add(const E& element) {
    _root = Insert(element, _root);
  }

  Node* Insert(const E& element, Node* node) {
    if (node == _nullNode) {
      _size++;
      return new Node(element, _nullNode, _nullNode);
    }

    int cmp = Compare(element, node->_element);
    if (cmp > 0) {
      node->_right = Insert(element, node->_right);
      if (node->_right->_priority < node->_priority) {
        node = RotateRight(node);
      }
    } else if (cmp < 0) {
      node->_left = Insert(element, node->_left);
      if (node->_left->_priority < node->_priority) {
        node = RotateLeft(node);
      }
    } else {

    }

    return node;
  }

  void Remove(const E& element) {
    _root = Delete(element, _root);
  }

  Node* Delete(const E& element, Node* node) {
    if (node != _nullNode) {
      int cmp = Compare(element, node->_element);

      if (cmp > 0) {
        node->_right = Delete(element, node->_right);
      } else if (cmp < 0) {
        node->_left = Delete(element, node->_left);
      } else {
        if (node->_left->_priority < node->_right->_priority) {
          node = RotateLeft(node);
        } else {
          node = RotateRight(node);
        }

        if (node != _nullNode) {
          node = Delete(element, node);
        } else {
          if (node->_left != _nullNode) {
            delete node->_left;
          }
          node->_left = _nullNode;
          _size--;
        }
      }
    }

    return node;
  }

  E FindMin() {
    if (_size == 0) {
      throw "Empty Tree\n";
    }

    Node* node = _root;
    while (node->_left != _nullNode) {
      node = node->_left;
    }
    return node->_element;
  }

  E FindMax() {
    if (_size == 0) {
      throw "Empty Tree\n";
    }

    Node* node = _root;
    while (node->_right != _nullNode) {
      node = node->_right;
    }
    return node->_element;
  }

  // 层序遍历输出二叉树
  friend std::ostream& operator<<(std::ostream& out, const Treap* rhs) {
    out << "Number of Tree Nodes Used = " << Node::used_count << "\n";
    if (rhs->_root == rhs->_nullNode) {
      out << "Empty Tree.\n";
      return out;
    }
    std::queue<Node*> q;
    q.push(rhs->_root);
    int levelSize = 1;
    int height = 0;
    while (!q.empty()) {
      Node* node = q.front();
      out << node << " ";
      levelSize--;

      if (node->_left != rhs->_nullNode) {
        q.push(node->_left);
      }
      if (node->_right != rhs->_nullNode) {
        q.push(node->_right);
      }
      q.pop();

      if (levelSize == 0) {
        levelSize = q.size();
        height++;
        out << "\n";
      }
    }
    out << "Tree Height = " << height << "\n";
    out << "Tree Size = " << rhs->_size << "\n";

    return out;
  }

  Node* RotateLeft(Node* node) {
    Node* left = node->_left;
    node->_left = left->_right;
    left->_right = node;
    return left;
  }

  Node* RotateRight(Node* node) {
    Node* right = node->_right;
    node->_right = right->_left;
    right->_left = node;
    return right;
  }


};

}

#endif // TREAP_H
