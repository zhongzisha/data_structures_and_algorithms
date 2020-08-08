#ifndef FIBONACCIHEAP_H
#define FIBONACCIHEAP_H

#include <iostream>
#include <queue>
#include <cmath>
#include "MyCommon.h"

namespace test {

template <typename E>
class FibonacciHeap {
public:
  class Node {
  public:
    static int used_count;
    E _key;
    int _degree;
    bool _mark;
    Node* _left;
    Node* _child;
    Node* _right;
    Node* _parent;

    Node(const E& key = E())
        : _key(key),
          _degree(0),
          _mark(false),
          _left(this),
          _child(nullptr),
          _right(this),
          _parent(nullptr) {
      used_count++;
      std::cout << "Node::Node(const E& key = E()), key = " << _key << ", used_count = " << used_count << "\n";
    }

    ~Node() {
      used_count--;
      std::cout << "Node::~Node(), key = " << _key << ", used_count = " << used_count << "\n";
    }

    friend std::ostream& operator<<(std::ostream& out, Node* node) {
      if (node == nullptr) {
        out << "null";
        return out;
      }

      if (node->_left != nullptr) {
        out << "L(" << node->_left->_key << ")_";
      }

      out << "{" << node->_key << "," << node->_degree << "}";

      if (node->_parent != nullptr) {
        out << "_P(" << node->_parent->_key << ")";
      }

      if (node->_right != nullptr) {
        out << "_R(" << node->_right->_key << ")" ;
      }

      return out;
    }
  };

public:
  int _size;
  Node* _min;

  FibonacciHeap() : _size(0), _min(nullptr) {
    std::cout << "FibonacciHeap::FibonacciHeap()\n";
  }

  ~FibonacciHeap() {
    Clear();
    std::cout << "FibonacciHeap::~FibonacciHeap()\n";
  }

  void Add(const E& key) {
    if (_min == nullptr) {
      _min = new Node(key);
      _size++;
      return;
    }

    Node* x = new Node(key);
    // add x to the root list
    Node* last = _min->_left;
    last->_right = x;
    x->_left = last;
    x->_right = _min;
    _min->_left = x;

    if (_min == nullptr ||
        x->_key < _min->_key) {
      _min = x;
    }

    _size++;
  }


  // 查找key的节点，递归
  Node* FindNode(Node* node, const E& key) {
    if (node == nullptr) return nullptr;

    Node* currentNode = node;
    do {
      if (node->_key < key) {
        Node* node1 = FindNode(node->_child, key); //进入其子树递归查找
        if (node1 == nullptr) {
          node = node->_right; //进入其后面的树进行查找
        } else {
          return node1;  // 在子树中找到了该节点
        }
      } else if (node->_key > key) {
        node = node->_right; //进入其后面的树进行查找
      } else {
        return node; // 找到了该节点
      }
    } while (node != currentNode);

    return nullptr;
  }


  void Remove(const E& key, const E& mininum_key) {
    Node* node = FindNode(_min, key);
    if (node == nullptr) {
      std::cout << key << " is not found.\n";
      return;
    }
    DecreaseKey(node, mininum_key);

    //std::cout << "after decrease key\n";
    //std::cout << this;

    Node* minNode = ExtractMin(); // the ExtractMin will decrease _size by 1
    delete minNode;
  }

  void Union(const FibonacciHeap* h2) {
    Union(h2->_root, h2->_min, h2->_size);
  }

  void Union(Node* root2, Node* min2, int size2) {
    if (root2 == nullptr) return;

    Node* root2_last = root2->_left;
    Node* current_last = _min->_left;

    current_last->_right = root2;
    root2->_left = current_last;

    root2_last->_right = _min;
    _min->_left = root2_last;

    if (_min == nullptr ||
        (min2 != nullptr && min2->_key < _min->_key)) {
      _min = min2;
    }

    _size += size2;
  }

  void Link(Node* y, Node* x) {

    // remove y from root list
    if (y->_parent == nullptr) {   
      Node* left = y->_left;
      Node* right = y->_right;
      left->_right = right;
      right->_left = left;

      if (y == y->_right) {
        _min = nullptr;
      } else {
        _min = y->_right;
      }
    }

    // make y a child of x
    if (x->_child == nullptr) {
      x->_child = y;
      y->_left = y;
      y->_right = y;
      y->_parent = x;
    } else {
      // add x to the child list of x
      Node* childRoot = x->_child;
      Node* last = childRoot->_left; // the last child of x
      last->_right = y;
      y->_left = last;
      y->_right = childRoot;
      childRoot->_left = y;
      y->_parent = x;
    }
    x->_degree++;

    y->_mark = false;
  }

  Node* ExtractMin() {
    Node* z = _min;
    if (z != nullptr) {
      // add children of z into the root list
      Node* child = z->_child;
      Node* w = child;
      Node* x;
      if (w != nullptr) {
        do {
          x = w;
          w = w->_right;
          // add x to the root list
          Node* last = _min->_left;
          last->_right = x;
          x->_left = last;
          x->_right = _min;
          _min->_left = x;
          //
          x->_parent = nullptr;
        } while (w != child);
      }
      //PrintNodeList(_min);

      // remove z from the root list
      if (z->_parent == nullptr) {
        Node* left = z->_left;
        Node* right = z->_right;
        left->_right = right;
        right->_left = left;
      }

      if (z == z->_right) {
        // only one node, z is also the root
        _min = nullptr;
      } else {
        _min = z->_right;

        //PrintNodeList(_min);

        Consolidate();
      }

      _size--;
    }

    return z;
  }

  void Consolidate() {
//    std::cout << "begin consolidate ++++++++++++++++++++++++++++++++++++\n";
//    std::cout << this << "\n";


    int length = std::ceil(std::log2((float)_size)) + 1;
    std::vector<Node*> A(length);
    //Node** A = new Node*[length]{nullptr};
//    for (int i = 0; i < length; i++) {
//      std::cout << A[i] << "\n";
//    }

    Node* node = _min;
    Node* w = node;
    int rootCount = 0;
    do {
      rootCount++;
      w = w->_right;
    } while (w != node);

    w = node;
    Node* x = nullptr;
    Node* y = nullptr;
    Node* tmp = nullptr;
    int d = 0;
    for (int i = 0; i < rootCount; i++) {
      x = w;
      w = w->_right;
      d = x->_degree;
      // std::cout << i << ": " << x << ", d = " << d << " ===========\n";
      while (A[d] != nullptr) {
        y = A[d];
        if (x->_key > y->_key) {
          // let x is the min node
          tmp = x;
          x = y;
          y = tmp;
        }
        Link(y, x); // let y be a child of x and remove y from the root list
        A[d] = nullptr;
        d++;
      }
      A[d] = x; // now x is in root list
    }

    // std::cout << this << "\n";

    _min = nullptr;
    for (int i = 0; i < length; i++) {
      if (A[i] != nullptr) {
        // add A[i] to the root list
        x = A[i];
        if (_min == nullptr) {
          x->_left = x;
          x->_right = x;
          x->_parent = nullptr;
          _min = x;
        } else {
          Node* last = _min->_left;
          last->_right = x;
          x->_left = last;
          x->_right = _min;
          _min->_left = x;
        }

        if (_min == nullptr || x->_key < _min->_key) {
          _min = x;
        }
      }
    }

    //delete[] A;
    //std::cout << "end consolidate ++++++++++++++++++++++++++++++++++++\n";
  }

  void Consolidate_wrong() {
//        std::cout << "begin consolidate ++++++++++++++++++++++++++++++++++++\n";
//        std::cout << this << "\n";


    int length = std::ceil(std::log2((float)_size)) + 1;
    Node* A[length] = {nullptr};
    //Node** A = new Node*[length]{nullptr};
    //    for (int i = 0; i < length; i++) {
    //      std::cout << A[i] << "\n";
    //    }

    Node* x = _min;
    Node* y = nullptr;
    Node* tmp = nullptr;
    int d = 0;
    do {
      d = x->_degree;
      while (A[d] != nullptr) {
        y = A[d];
        if (x == y) break;
        if (x->_key > y->_key) {
          // let x is the min node
          tmp = x;
          x = y;
          y = tmp;
        }
        if (y == _min)
          _min = x;
        // let y be a child of x and remove y from the root list
        Link(y, x);
        if (x == x->_right)
          _min = x;
        A[d] = nullptr;
        d++;
      }
      A[d] = x; // now x is in root list
      //PrintNodeList(_min);
      x = x->_right;
    } while (x != _min);

    // std::cout << this << "\n";

    _min = nullptr;
    for (int i = 0; i < length; i++) {
      if (A[i] != nullptr) {
        // add A[i] to the root list
        x = A[i];
        if (_min == nullptr) {
          x->_left = x;
          x->_right = x;
          x->_parent = nullptr;
          _min = x;
        } else {
          Node* last = _min->_left;
          last->_right = x;
          x->_left = last;
          x->_right = _min;
          _min->_left = x;
        }

        if (_min == nullptr || x->_key < _min->_key) {
          _min = x;
        }
      }
    }

    //delete[] A;
    std::cout << "end consolidate ++++++++++++++++++++++++++++++++++++\n";
  }

  void PrintNodeList(Node* node) {
    Node* x = node;
    do {
      std::cout << x->_key << "(" << x->_degree << ")" << ", ";
      x = x->_right;
    } while (x != node);
    std::cout << "\n";
  }

  void DecreaseKey(Node *x, const E& key) {
    if (key > x->_key) {
      throw "new key is greater than current key";
    }

    x->_key = key;
    Node* y = x->_parent;
    if (y != nullptr && x->_key < y->_key) {
      Cut(x, y);
      CascadingCut(y);
    }

    if (x->_key < _min->_key) {
      _min = x;
    }
  }

  void Cut(Node* x, Node* y) {
    // remove x from the child list of y, decrementing degree of y
    if (x->_parent == y) {
      // remove y from the child of x
      if (x == x->_right) {
        y->_child = nullptr; // only one child
        y->_degree = 0;
      } else {
        Node* left = x->_left;
        Node* right = x->_right;
        left->_right = right;
        right->_left = left;
        y->_degree--;
        if (y->_child == x) {
          y->_child = right;
        }
      }
    }

    // add x to the root list
    if (_min == nullptr) {
      x->_left = x;
      x->_right = x;
      _min = x;
    } else {
      Node* last = _min->_left;
      last->_right = x;
      x->_left = last;
      x->_right = _min;
      _min->_left = x;
      if (x->_key < _min->_key) {
        _min = x;
      }
    }
    x->_parent = nullptr;
    x->_mark = false;
  }

  void CascadingCut(Node* y) {
    Node* z = y->_parent;
    if (z != nullptr) {
      if (y->_mark == false) {
        y->_mark = true;
      } else {
        Cut(y, z);
        CascadingCut(z);
      }
    }
  }

  void Clear() {
    if (_min == nullptr) {
      return;
    }

    std::queue<Node*> q;
    Node* node = _min;
    Node* x = node;
    do {
      q.push(x);
      x = x->_right;
    } while (x != node);

    while (!q.empty()) {
      node = q.front();

      Node* child = node->_child;
      x = child;
      if (x != nullptr) {
        do {
          q.push(x);
          x = x->_right;
        } while(x != child);
      }
      q.pop();

      delete node;
    }
  }

  friend std::ostream& operator<<(std::ostream& out, FibonacciHeap* h) {
    if (h == nullptr) {
      out << "Empty FibonacciHeap.\n";
      return out;
    }
    std::queue<Node*> q;
    Node* node = h->_min;
    Node* x = node;
    if (x != nullptr) {
      do {
        q.push(x);
        x = x->_right;
      } while (x != node);
    }

    int levelSize = q.size();
    int height = 0;
    while (!q.empty()) {
      node = q.front();

      std::cout << node << "\n";

      Node* child = node->_child;
      x = child;
      if (x != nullptr) {
        do {
          q.push(x);
          x = x->_right;
        } while(x != child);
      }

      q.pop();
      levelSize--;
      if (levelSize == 0) {
        levelSize = q.size();
        height++;
        std::cout << "Level " << height
                  << " END ===========================================\n";
      }
    }

    out << "FibonacciHeap Size = " << h->_size << "\n";
    out << "FibonacciHeap Height = " << height << "\n";
    out << "MinNode = " << h->_min << "\n";
    return out;
  }

};

}

#endif // FIBONACCIHEAP_H
