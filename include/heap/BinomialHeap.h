#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H


#include <iostream>
#include <queue>
#include "MyCommon.h"

namespace test {


template <typename E>
class BinomialHeap {
public:
  class Node {
  public:
    static int used_count;
    E _key;   // 关键字
    int _degree;  // 子女个数
    Node* _left;  // 左孩子
    Node* _sibling; // 右兄弟
    Node* _parent; //父节点

    Node()
        : _key(E()),
          _degree(0),
          _left(nullptr),
          _sibling(nullptr),
          _parent(nullptr) {
      used_count++;
      std::cout << "Node::Node(), used_count = " << used_count << "\n";
    }

    Node(const E& key)
        : _key(key),
          _degree(0),
          _left(nullptr),
          _sibling(nullptr),
          _parent(nullptr) {
      used_count++;
      std::cout << "Node::Node(const E& key), used_count = " << used_count << "\n";
    }

    ~Node() {
      used_count--;
      std::cout << "Node::~Node(), key = " << _key << ", used_count = " << used_count << "\n";
    }

    friend std::ostream& operator<<(std::ostream& out, Node* node) {
      if (node == nullptr) {
        return out;
      }

      out << "{" << node->_key << "," << node->_degree << "}";

      if (node->_parent != nullptr) {
        out << "_P(" << node->_parent->_key << ")";
      }

      if (node->_sibling != nullptr) {
        out << "_N(" << node->_sibling->_key << ")";
      }

      return out;
    }

  };

public:
  int _size;
  Node* _root;

  BinomialHeap() : _size(0), _root(nullptr) {
    std::cout << "BinomialHeap::BinomialHeap()\n";
  }

  ~BinomialHeap() {

    Clear();

    std::cout << "BinomialHeap::~BinomialHeap()\n";
  }

  int Compare(E e1, E e2) {
    return e1 - e2;
  }

  void Add(const E& key) {
    if (_root == nullptr) {
      _root = new Node(key);
      _size++;
      return;
    }

    Node* newNode = new Node(key);
    _root = UnionNode(_root, newNode);
    _size++;
  }

  // 假设堆里面的关键字没有minimum_key, 比如INT_MIN
  void Remove(const E& key, const E& mininum_key) {
    Node* node = FindNode(_root, key);
    if (node == nullptr) return;
//    std::cout << "(1) Size = " << _size << "\n";
//    std::cout << this << "\n";
    DecreaseKey(node, mininum_key);
//    std::cout << "(2) Size = " << _size << "\n";
//    std::cout << "after DecreaseKey:\n";
//    std::cout << this << "\n";
    Node* minNode = ExtractMin();
//    std::cout << "(3) Size = " << _size << "\n";
//    std::cout << "after ExtractMin:\n";
//    std::cout << this << "\n";
    delete minNode;
    _size--;
  }

  // 查找key的节点，递归
  Node* FindNode(Node* node, const E& key) {
    if (node == nullptr) return nullptr;

    while (node != nullptr) {
      if (node->_key < key) {
        Node* node1 = FindNode(node->_left, key); //进入其子树递归查找
        if (node1 == nullptr) {
          node = node->_sibling; //进入其后面的树进行查找
        } else {
          return node1;  // 在子树中找到了该节点
        }
      } else if (node->_key > key) {
        node = node->_sibling; //进入其后面的树进行查找
      } else {
        return node; // 找到了该节点
      }
    }

    return nullptr;
  }

  // 检查是否一个二项堆
  bool Check(Node* node) {
    if (node == nullptr) return true;

    if (node->_left != nullptr && node->_left->_key < node->_key) {
      std::cout << "node = " << node << ", node->left = " << node->_left << "\n";
      return false;
    } else {
      std::cout << node << "\n";
    }

    return Check(node->_left) && Check(node->_sibling);
  }

  bool Check() {
    return Check(_root);
  }

  void Visit() {
    Visit(_root);
  }

  void Visit(Node* node) {
    if (node == nullptr) return;

//    std::cout << node << "\n";
    Visit(node->_left);
//    std::cout << node << "\n";
    Visit(node->_sibling);
    std::cout << node << "\n";
  }

  // 寻找最小key值的节点
  Node* FindMin() {
    if (_root == nullptr) return nullptr;

    // at first, point to the root node
    // the root is the mininum node
    Node* node = _root;
    Node* minNode = node;
    E minKey = minNode->_key;
    while (node != nullptr) {
      if (node->_key < minKey) {
        minKey = node->_key;
        minNode = node;
      }
      node = node->_sibling;
    }

    return minNode;
  }

  // 寻找最小key值的节点并删除该节点
  Node* ExtractMin() {
    if (_root == nullptr) return nullptr;

    // find the root x with the mininum key in the root list
    Node* prev = nullptr;
    Node* node = _root;
    Node* next = nullptr;
    Node* minNode = node;
    Node* minPrev = nullptr;
    E minKey = minNode->_key;
    while (node != nullptr) {
      next = node->_sibling;
      if (node->_key < minKey) {
        minNode = node;
        minKey = minNode->_key;
        minPrev = prev;   // 这种要存储前一个节点，便于后面删除
      }
      prev = node;
      node = next;
    }

    // remove x from the root list
    if (minPrev != nullptr)
      minPrev->_sibling = minNode->_sibling;
    else
      // 这里要特别注意，如果最小节点是头节点，需要将root指向后一个节点
      _root = minNode->_sibling;

    // set the parent of each child to nullptr
    node = minNode->_left;
    while (node != nullptr) {
      node->_parent = nullptr;
      node = node->_sibling;
    }

    // reverse the children node list
    PrintNodeList(minNode->_left);
    Node* childrenNodeList = ReverseNodeList(minNode->_left);
    PrintNodeList(childrenNodeList);

    // union the original tree and the children tree
    _root = UnionNode(_root, childrenNodeList);

    std::cout << this << "\n";

    // need to delete manually
    return minNode;

  }

  // 将某个节点的key减小为一个新的key
  // 如果新的key大于当前的key，将引发错误
  void DecreaseKey(Node* x, const E& key) {
    if (key > x->_key) {
      throw "New key is greater than current key.\n";
    }

    x->_key = key;
    Node* y = x;
    Node* z = y->_parent;
    while (z != nullptr && y->_key < z->_key) {
      // exchange the key between parent and child
      E tmpKey = y->_key;
      y->_key = z->_key;
      z->_key = tmpKey;

      y = z;
      z = y->_parent;
    }
  }

  void LinkNode(Node* y, Node* z) {
    /*
     *       root
     *     /
     *   child
     */
    y->_parent = z;
    y->_sibling = z->_left;
    z->_left = y;
    z->_degree++;   //z的孩子数加1
  }

  // 递归合并两个有序链表
  Node* MergeNode_Recursively(Node* node1, Node* node2) {
    Node* first = nullptr;
    if (node1 == nullptr)
      return node2;
    else if (node2 == nullptr)
      return node1;
    else {
      if (node1->_degree <= node2->_degree) {
        first = node1;
        first->_sibling = MergeNode_Recursively(node1->_sibling, node2);
      } else {
        first = node2;
        first->_sibling = MergeNode_Recursively(node1, node2->_sibling);
      }
    }
  }

  Node* MergeNode(Node* node1, Node* node2) {
    //合并成一个按degree单调递增的链表
    if (node1 == nullptr) return node2;
    if (node2 == nullptr) return node1;

    Node* p1 = node1;
    Node* p2 = node2;
    Node* tmpNode = new Node();
    Node* lastNode = tmpNode;
    while (p1 != nullptr && p2 != nullptr) {
      if (p1->_degree <= p2->_degree) {
        lastNode->_sibling = p1;
        p1 = p1->_sibling;
      } else {
        lastNode->_sibling = p2;
        p2 = p2->_sibling;
      }
      lastNode = lastNode->_sibling;
    }
    if (p1 != nullptr) {
      lastNode->_sibling = p1;
    }
    if (p2 != nullptr) {
      lastNode->_sibling = p2;
    }
    lastNode = tmpNode->_sibling;
    delete tmpNode;
    return lastNode;
  }

  Node* UnionNode(Node* node1, Node* node2) {

//    std::cout << "node1: ";
//    PrintNodeList(node1);
//    std::cout << "node2: ";
//    PrintNodeList(node2);

    Node* mergedNode = MergeNode(node1, node2);
    if (mergedNode == nullptr) return nullptr;

    std::cout << "mergedNode: ";
    PrintNodeList(mergedNode);

    Node* firstNode = mergedNode;

    Node* prev = nullptr;
    Node* x = mergedNode;
    Node* next = x->_sibling;
    while (next != nullptr) {
      if ((x->_degree != next->_degree) ||
          (next->_sibling != nullptr && next->_sibling->_degree == x->_degree)) {
        prev = x;
        x = next;
      } else if (x->_key <= next->_key) {
        x->_sibling = next->_sibling;
        LinkNode(next, x);
      } else {
        if (prev == nullptr) {
          firstNode = next;
        } else {
          prev->_sibling = next;
        }
        LinkNode(x, next);
        x = next;
      }
      next = x->_sibling;
    }

    return firstNode;
  }

  Node* GenerateNodeList(int arr[], int length) {
    Node* node = new Node();
    Node* firstNode = node;
    node->_degree = arr[0];
    for (int i = 1; i < length; i++) {
      node->_sibling = new Node();
      node->_sibling->_degree = arr[i];
      node = node->_sibling;
    }

    return firstNode;
  }

  // 反转链表
  Node* ReverseNodeList(Node* node) {
    if (node == nullptr) return nullptr;

    Node* prev = nullptr;
    Node* current = node;
    Node* next = nullptr;
    while (current != nullptr) {
      next = current->_sibling;
      current->_sibling = prev;
      prev = current;
      current = next;
    }

    return prev;
  }

  void PrintNodeList(Node* node) {
    std::cout << "NodeList: ";
    Node* p = node;
    while (p != nullptr) {
      std::cout << p->_key << "_" << p->_degree << ", ";
      p = p->_sibling;
    }
    std::cout << "\n";
  }

  void DeleteNodeList(Node* node) {
    Node* p;
    while (node != nullptr) {
      p = node;
      node = node->_sibling;
      delete p;
    }
  }

  friend std::ostream& operator<<(std::ostream& out, BinomialHeap* rhs) {
    if (rhs->_root == nullptr) {
      out << "Empty BinomialHeap.\n";
      return out;
    }

    out << "BinomialHeap Size = " << rhs->_size << "\n";

    std::queue<Node*> q;

    // add the root nodes to the queue
    Node* node = rhs->_root;
    while (node != nullptr) {
      q.push(node);
      node = node->_sibling;
    }

    int levelSize = q.size();

    while (!q.empty()) {
      node = q.front();

      std::cout << node << " ";

      // add the children to the queue
      Node* child = node->_left;
      while (child != nullptr) {
        q.push(child);
        child = child->_sibling;
      }

      q.pop();
      levelSize--;

      if (levelSize == 0) {
        levelSize = q.size();
        std::cout << "\n";
      }
    }

    return out;
  }

  void Clear() {
    if (_root == nullptr) {
      return;
    }

    std::queue<Node*> q;

    // add the root nodes to the queue
    Node* node = _root;
    while (node != nullptr) {
      q.push(node);
      node = node->_sibling;
    }

    int levelSize = q.size();

    while (!q.empty()) {
      node = q.front();

      // std::cout << node->_key << " ";

      // add the children to the queue
      Node* child = node->_left;
      while (child != nullptr) {
        q.push(child);
        child = child->_sibling;
      }

      delete node;

      q.pop();
      levelSize--;

      if (levelSize == 0) {
        levelSize = q.size();
        std::cout << "\n";
      }

    }

  }

};

}


#endif // BINOMIALHEAP_H
