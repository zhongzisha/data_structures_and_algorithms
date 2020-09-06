#ifndef BinarySearchTree_H
#define BinarySearchTree_H

#include <iostream>
#include <queue>
#include "MyCommon.h"

namespace test {

template <typename E>
class BinarySearchTree {
public:
  class Node {
  public:
    E _element;
    Node* _left;
    Node* _right;
    Node* _parent;
    static int use_count;

    Node(const E& element, Node* parent) : _element(element),
      _left(nullptr),
      _right(nullptr),
      _parent(parent) {
      use_count++;
    }
    ~Node() {
      std::cout << "invode Node::~Node() " << _element << "\n";
      use_count--;
    }

    bool IsLeaf() const {
      return _left == nullptr && _right == nullptr;
    }

    bool HasTwoChildren() const {
      return _left != nullptr && _right != nullptr;
    }

    bool IsLeftChild() const {
      return _parent != nullptr && this == _parent->_left;
    }

    bool IsRightChild() const {
      return _parent != nullptr && this == _parent->_right;
    }

    bool operator==(const Node& rhs) {
      return _element == rhs._element &&
          _left == rhs._left &&
          _right == rhs._right &&
          _parent == rhs._parent;
    }

    bool operator!=(const Node& rhs) {
      return !operator==(rhs);
    }

    Node* sibling() const {
      if (IsLeftChild()) {
        return _parent->_right;
      }
      if (IsRightChild()) {
        return _parent->_left;
      }
      return nullptr;
    }

    friend std::ostream& operator<< (std::ostream& out, const Node* rhs) {
      if (rhs == nullptr) {
        out << "null";
        return out;
      }
      if (rhs->_left != nullptr) {
        out << "L" << rhs->_left->_element;
      } else {
        out << "null";
      }

      out << "_(" << rhs->_element << ")";

      if (rhs->_parent != nullptr) {
        out << "P" << rhs->_parent->_element;
      }

      if (rhs->_right != nullptr) {
        out << "_R" << rhs->_right->_element;
      } else {
        out << "_null";
      }
      return out;
    }
  };

protected:
  virtual Node* CreateNode(const E& element, Node* parent) {
    return new Node(element, parent);
  }

protected:
  int _size;
  Node* _root;
public:
  BinarySearchTree() : _size(0), _root(nullptr) {
    std::cout << "BinarySearchTree()\n";
  }
  virtual ~BinarySearchTree() {
    std::cout << "~BinarySearchTree()\n";
    Clear();
  }

  int Size() {
    return _size;
  }
  bool IsEmpty() {
    return _size == 0;
  }
  void Clear() {
    if (_root == nullptr) return;

    // 层序遍历，依次删除访问到的节点
    std::queue<Node*> q;
    q.push(_root);
    while (!q.empty()) {
      Node* node = q.front();
      if (node->_left != nullptr)
        q.push(node->_left);
      if (node->_right != nullptr)
        q.push(node->_right);
      q.pop();
      delete node;
    }
    _size = 0;
    _root = nullptr;
  }

  bool Contains(const E& element) {
    return GetNode(element) != nullptr;
  }

  int Compare(const E& e1, const E& e2) {
    return e1 - e2;
  }

  void Add(const E& element) {
    std::cout << "Add " << element << "\n";

    if (_root == nullptr) { // 如果添加的是第一个节点，则创建根节点
      _root = CreateNode(element, nullptr);
      _size++;

      AfterAdd(_root);
      return;
    }

    //如果不是第一个节点，则沿着树进行比较，左边小，右边大
    Node* parent = nullptr;
    Node* node = _root;
    int cmp = 0;
    while (node != nullptr) {
      cmp = Compare(element, node->_element);
      parent = node;
      if (cmp > 0) {
        node = node->_right;
      } else if (cmp < 0) {
        node = node->_left;
      } else {
        node->_element = element;
        return;
      }
    }

    // 这时，需要创建一个新节点，看是插入到父节点的左边还是右边
    Node* newNode = CreateNode(element, parent);
    if (cmp > 0) {
      parent->_right = newNode;
    } else {
      parent->_left = newNode;
    }
    _size++;

    // 新添加节点的处理
    AfterAdd(newNode);
  }

  void Remove(const E& element) {
    Node* elementNode = GetNode(element);
    //RemoveBySuccessor(elementNode);
    RemoveByPredecessor(elementNode);
  }

  void RemoveBySuccessor(Node* node) {
    if (node == nullptr) return;

    _size--;

    // 删除的是度为2的节点
    if (node->HasTwoChildren()) {
      Node* s = Successor(node); // 找到后继节点
      node->_element = s->_element; // 将后继节点的值覆盖需要删除的节点
      node = s; // 删除那个后继节点(后继节点肯定是度为0或者度为1)
    }

    // 删除度为0或1的节点node
    Node* replaceNode = (node->_left == nullptr) ? node->_right : node->_left;
    if (replaceNode != nullptr) { // 度为1的节点
      replaceNode->_parent = node->_parent;
      if (node->_parent == nullptr) { // 度为1，且是根节点
        _root = replaceNode;
      } else if (node == node->_parent->_left) {
        node->_parent->_left = replaceNode;
      } else if( node == node->_parent->_right) {
        node->_parent->_right = replaceNode;
      }

      // 删除节点后进行处理
      AfterRemove(node, replaceNode);
    } else if (node->_parent == nullptr) { // 度为0的节点，根节点
      _root = nullptr;

      AfterRemove(node, nullptr);
    } else { // 度为0的节点，叶子节点
      if (node == node->_parent->_left) {
        node->_parent->_left = nullptr;
      } else {
        node->_parent->_right = nullptr;
      }

      AfterRemove(node, nullptr);
    }

    delete node;
  }


  void RemoveByPredecessor(Node* node) {
    if (node == nullptr) return;

    _size--;

    // 删除的是度为2的节点
    if (node->HasTwoChildren()) {
      Node* s = Predecessor(node); // 找到后继节点
      node->_element = s->_element; // 将后继节点的值覆盖需要删除的节点
      node = s; // 删除那个后继节点(后继节点肯定是度为0或者度为1)
    }

    // 删除度为0或1的节点node
    Node* replaceNode = (node->_left == nullptr) ? node->_right : node->_left;
    if (replaceNode != nullptr) { // 度为1的节点
      replaceNode->_parent = node->_parent;
      if (node->_parent == nullptr) { // 度为1，且是根节点
        _root = replaceNode;
      } else if (node == node->_parent->_left) {
        node->_parent->_left = replaceNode;
      } else if( node == node->_parent->_right) {
        node->_parent->_right = replaceNode;
      }

      // 删除节点后进行处理
      AfterRemove(node, replaceNode);
    } else if (node->_parent == nullptr) { // 度为0的节点，根节点
      _root = nullptr;

      AfterRemove(node, nullptr);
    } else { // 度为0的节点，叶子节点
      if (node == node->_parent->_left) {
        node->_parent->_left = nullptr;
      } else {
        node->_parent->_right = nullptr;
      }

      AfterRemove(node, nullptr);
    }

    delete node;
  }

  // 找node节点的后继节点
  Node* Successor(Node* node) {
    if (node == nullptr) return nullptr;

    //如果节点的右子树不为空，则从右子树一路向左找
    Node* p = node->_right;
    if (p != nullptr) {
      while (p->_left != nullptr) {
        p = p->_left;
      }
      return p;
    }

    // 如果节点的右子树为空，则沿着父亲节点向上找
    while (node->_parent != nullptr && node == node->_parent->_right) {
      node = node->_parent;
    }
    return node->_parent;
  }

  // 找node节点的前驱节点
  Node* Predecessor(Node* node) {
    if (node == nullptr) return nullptr;

    Node* p = node->_left;
    if (p != nullptr) {
      while (p->_right != nullptr) {
        p = p->_right;
      }
      return p;
    }

    while (node->_parent != nullptr && node == node->_parent->_left) {
      node = node->_parent;
    }
    return node->_parent;
  }

  //
  Node* GetNode(const E& element) {
    Node* node = _root;
    while (node != nullptr) {
      int cmp = Compare(element, node->_element);
      if (cmp == 0) return node;
      if (cmp > 0) {
        node = node->_right;
      } else {
        node = node->_left;
      }
    }
    return nullptr;
  }

  void PreOrder() {

  }

  void InOrder() {

  }

  void PostOrder() {

  }

  void LevelOrder() {
    if (_root == nullptr) {
      std::cout << "Empty tree.\n";
      return;
    }
    std::queue<Node*> q;
    q.push(_root);
    while (!q.empty()) {
      Node* node = q.front();
      std::cout << node << "\n";

      if (node->_left != nullptr) {
        q.push(node->_left);
      }
      if (node->_right != nullptr) {
        q.push(node->_right);
      }

      q.pop();
    }
  }

  // 翻转二叉树
  void Invert() {

  }

  // 层序遍历检查是否为BST
  bool IsBST() const {
    if (_root == nullptr) {
      std::cout << "Empty Tree\n";
      return false;
    }
    std::queue<Node*> q;
    q.push(_root);
    while (!q.empty()) {
      Node* node = q.front();

      if (node->_left != nullptr) {
        if (node->_left->_element > node->_element)
          return false;
        q.push(node->_left);
      }
      if (node->_right != nullptr) {
        if (node->_right->_element < node->_element)
          return false;
        q.push(node->_right);
      }

      q.pop();
    }
    return true;
  }

  // 层序遍历输出二叉树
  friend std::ostream& operator<<(std::ostream& out, const BinarySearchTree& rhs) {
    out << "Number of Tree Nodes Used = " << Node::use_count << "\n";
    if (rhs._root == nullptr) {
      out << "Empty Tree.\n";
      return out;
    }
    std::queue<Node*> q;
    q.push(rhs._root);
    int levelSize = 1;
    int height = 0;
    while (!q.empty()) {
      Node* node = q.front();
      out << node << " ";
      levelSize--;

      if (node->_left != nullptr) {
        q.push(node->_left);
      }
      if (node->_right != nullptr) {
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

    return out;
  }

  virtual void AfterAdd(Node* node) {
    if (node == nullptr) return;
  }

  virtual void AfterRemove(Node* node, Node* replaceNode) {
    if (node == nullptr) return;
    if (replaceNode == nullptr) {

    }
  }

};

}

#endif // BinarySearchTree_H
