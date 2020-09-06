#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include "MyCommon.h"
#include "BinarySearchTree.h"

namespace test {

template <typename E>
class AVLTree : public BinarySearchTree<E> {
protected:
  typedef typename BinarySearchTree<E>::Node Node;
public:
  class AVLNode : public Node {
  public:
    int _height;
    AVLNode(const E& element, Node* parent)
      : Node(element, parent), _height(1) {
    }

    int BalanceFactor() {
      int leftHeight = this->_left == nullptr ? 0 : static_cast<AVLNode*>(this->_left)->_height;
      int rightHeight = this->_right == nullptr ? 0 : static_cast<AVLNode*>(this->_right)->_height;
      return leftHeight - rightHeight;
    }

    void UpdateHeight() {
      int leftHeight = this->_left == nullptr ? 0 : static_cast<AVLNode*>(this->_left)->_height;
      int rightHeight = this->_right == nullptr ? 0 : static_cast<AVLNode*>(this->_right)->_height;
      _height = 1 + std::max(leftHeight, rightHeight);
    }

    Node* TallerChild() {
      int leftHeight = this->_left == nullptr ? 0 : static_cast<AVLNode*>(this->_left)->_height;
      int rightHeight = this->_right == nullptr ? 0 : static_cast<AVLNode*>(this->_right)->_height;
      if (leftHeight > rightHeight) return this->_left;
      if (leftHeight < rightHeight) return this->_right;
      return this->IsLeftChild() ? this->_left : this->_right;
    }

    friend std::ostream& operator<< (std::ostream& out, const AVLNode* rhs) {
      if (rhs == nullptr) {
        out << "null";
        return out;
      }
      if (rhs->_left != nullptr) {
        out << "L" << rhs->_left->_element;
      } else {
        out << "null";
      }

      out << "_(" << rhs->_element << "_" <<rhs->_height << ")";

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

public:
  Node* CreateNode(const E& element, Node* parent) {
    return new AVLNode(element, parent);
  }

  bool IsBalanced(Node* node) {
    return std::abs(static_cast<AVLNode*>(node)->BalanceFactor()) <= 1;
  }

  void UpdateHeight(Node* node) {
    static_cast<AVLNode*>(node)->UpdateHeight();
  }

  void Rebalance(Node* grand) {
    Node* parent = static_cast<AVLNode*>(grand)->TallerChild();
    Node* node = static_cast<AVLNode*>(parent)->TallerChild();
    if (parent->IsLeftChild()) { // L
      if (node->IsLeftChild()) { // LL
        RotateRight(grand);
      } else {                   // LR
        RotateLeft(parent);
        RotateRight(grand);
      }
    } else {                     // R
      if (node->IsLeftChild()) { // RL
        RotateRight(parent);
        RotateLeft(grand);
      } else {                   // RR
        RotateLeft(grand);
      }
    }
  }

  void RotateLeft(Node* grand) {
    Node* parent = grand->_right;
    Node* child = parent->_left;
    grand->_right = child;
    parent->_left = grand;

    AfterRotate(grand, parent, child);
  }

  void RotateRight(Node* grand) {
    Node* parent = grand->_left;
    Node* child = parent->_right;
    grand->_left = child;
    parent->_right = grand;

    AfterRotate(grand, parent, child);

  }

  void AfterRotate(Node* grand, Node* parent, Node* child) {
    parent->_parent = grand->_parent;
    if (grand->IsLeftChild()) {
      grand->_parent->_left = parent;
    } else if (grand->IsRightChild()) {
      grand->_parent->_right = parent;
    } else {
      this->_root = parent;
    }

    if (child != nullptr) {
      child->_parent = grand;
    }

    grand->_parent = parent;

    // 更新高度
    UpdateHeight(grand);
    UpdateHeight(parent);
  }

  void AfterAdd(Node* node) {
    // node 是新添加的节点，必然是叶子节点
    // 从这个节点向上找，找到失去平衡的节点
    while ((node = node->_parent) != nullptr) {
      if (IsBalanced(node)) {
        UpdateHeight(node);
      } else {
        Rebalance(node);
        break;
      }
    }
  }

  void AfterRemove(Node* node, Node* replaceNode) {

    if (replaceNode == nullptr) {

    }
    while ((node = node->_parent) != nullptr) {
      if (IsBalanced(node)) {
        UpdateHeight(node);
      } else {
        Rebalance(node);
        // break;
      }
    }
  }

  // 层序遍历检查是否为AVLTree
  bool IsAVLTree() const {
    if (this->_root == nullptr) {
      std::cout << "Empty Tree\n";
      return false;
    }

    std::queue<Node*> q;
    q.push(this->_root);
    while (!q.empty()) {
      AVLNode* node = static_cast<AVLNode*>(q.front());
      if (std::abs(node->BalanceFactor()) > 1)
        return false;

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
  friend std::ostream& operator<<(std::ostream& out, const AVLTree& rhs) {
    out << "Number of AVLTree Nodes Used = " << Node::use_count << "\n";
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
      out << static_cast<AVLNode*>(node) << " ";
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
    out << "AVLTree Height = " << height << "\n";

    return out;
  }

};


}

#endif // AVLTREE_H
