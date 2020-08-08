#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>
#include <queue>
#include "MyCommon.h"

namespace test {

template <typename E, int K = 2>
class KDTree {
public:
  class Point {
  public:
    E _data[K];

    Point() {
      _data = {0};
    }

    Point(const Point& point) {
      for (int i = 0; i < K; i++)
        _data[i] = point._data[i];
    }

    Point(const E data_array[]) {
      for (int i = 0; i < K; i++)
        _data[i] = data_array[i];
    }

    Point& operator=(const Point& point) {
      for (int i = 0; i < K; i++)
        _data[i] = point._data[i];
      return *this;
    }

    friend bool operator==(const Point& p1, const Point& p2) {
      for (int i = 0; i < K; i++) {
        if (p1._data[i] != p2._data[i])
          return false;
      }
      return true;
    }

    const E& operator[](int index) const {
      if (index < 0 || index >= K) {
        throw "Wrong Index";
      }
      return _data[index];
    }

    friend std::ostream& operator<<(std::ostream& out, const Point& rhs) {
      out << "(";
      for (int i = 0; i < K; i++) {
        out << rhs._data[i];
        if (i < K - 1) {
          out << ", ";
        }
      }
      out << ")";

      return out;
    }
  };

  class Node {
  public:
    static int used_count;
    Point _point;
    Node* _left;
    Node* _right;

    Node() : _point(), _left(nullptr), _right(nullptr) {
      used_count++;
      std::cout << "Node::Node(), used_count = " << used_count << "\n";
    }

    Node(const Point& point) : _point(point), _left(nullptr), _right(nullptr) {
      used_count++;
      std::cout << "Node::Node(const Point& point), used_count = "
                << used_count << "\n";
    }

    Node(const Point& point, Node* left, Node* right)
        : _point(point), _left(left), _right(right) {
      used_count++;
      std::cout << "Node::Node(const Point& point, Node* left, Node* right), used_count = "
                << used_count << "\n";
    }

    ~Node() {
      used_count--;
      std::cout << "Node::~Node(), used_count = " << used_count << "\n";
    }

    friend std::ostream& operator<<(std::ostream& out, Node* rhs) {
      if (rhs == nullptr) {
        out << "Empty Node\n";
        return out;
      }

      if (rhs->_left != nullptr) {
        out << "L(";
        for (int i = 0; i < K; i++) {
          out << rhs->_left->_point[i];
          if (i < K - 1)
            out << ", ";
        }
        out << ")_";
      }

      out << "(";
      for (int i = 0; i < K; i++) {
        out << rhs->_point[i];
        if (i < K - 1)
          out << ", ";
      }
      out << ")";

      if (rhs->_right != nullptr) {
        out << "_R(";
        for (int i = 0; i < K; i++) {
          out << rhs->_right->_point[i];
          if (i < K - 1)
            out << ", ";
        }
        out << ")";
      }

      return out;
    }

  };

public:
  int _size;
  Node* _root;

  KDTree() : _size(0), _root(nullptr) {
    std::cout << "KDTree::KDTree()\n";
  }

  ~KDTree() {
    DeleteTreeIteratively(_root);
    _size = 0;
    _root = nullptr;
    std::cout << "KDTree::~KDTree()\n";
  }

  Node* CreateNode(const Point& point) {
    return new Node(point);
  }

  void Add(const Point& point) {
    _root = Insert(_root, point, 0);
  }

  Node* Insert(Node* node, const Point& point, int axis) {
    if (node == nullptr) {
      _size++;
      return CreateNode(point);
    }

    int _axis_index = axis % K;
    if (point[_axis_index] < node->_point[_axis_index]) {
      node->_left = Insert(node->_left, point, _axis_index + 1);
    } else {
      node->_right = Insert(node->_right, point, _axis_index + 1);
    }

    return node;
  }

  void Remove(const Point& point) {
    _root = Delete(_root, point, 0);
  }

  Node* Delete(Node* node, const Point& point, int axis) {
    if (node == nullptr)
      return nullptr;

    int _axis_index = axis % K;
    if (node->_point == point) { // 如果找到了需要删除的点
      if (node->_right != nullptr) { //如果右子树不为空
        Node* minNode = FindMinNode(node->_right, _axis_index);
        node->_point = minNode->_point;
        node->_right = Delete(node->_right, minNode->_point, axis + 1);
      } else if (node->_left != nullptr) { //如果左子树不为空
        Node* minNode = FindMinNode(node->_left, _axis_index);
        node->_point = minNode->_point;
        node->_left = Delete(node->_left, minNode->_point, axis + 1);
      } else { // 如果左右子树都为空，删除的是叶子节点，直接删除即可
        delete node;
        _size--;
        return nullptr;
      }
      return node;
    }

    if (point[_axis_index] < node->_point[_axis_index]) {
      // 往左子树找
      node->_left = Delete(node->_left, point, axis + 1);
    } else {
      // 往右子树找
      node->_right = Delete(node->_right, point, axis + 1);
    }

    return node;
  }

  Node* MinNode(Node* x, Node* y, Node* z, int axis) {
    Node* result = x;
    if (y != nullptr && y->_point[axis] < result->_point[axis]) {
      result = y;
    }
    if (z != nullptr && z->_point[axis] < result->_point[axis]) {
      result = z;
    }
    return result;
  }

  Node* FindMinNode(Node* node, int d) {
    return FindMinNode(node, d, 0);
  }

  Node* FindMinNode(Node* node, int d, int axis) {
    if (node == nullptr)
      return nullptr;

    int _axis_index = axis % K;
    if (_axis_index == d) {
      if (node->_left == nullptr)
        return node;
      return FindMinNode(node->_left, d, axis+1);
    }

    return MinNode(node,
                   FindMinNode(node->_left, d, axis+1),
                   FindMinNode(node->_right, d, axis+1), d);
  }

  // 层序遍历输出二叉树
  friend std::ostream& operator<<(std::ostream& out, const KDTree* rhs) {
    out << "Number of Tree Nodes Used = " << Node::used_count << "\n";
    if (rhs->_root == nullptr) {
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
    out << "Tree Size = " << rhs->_size << "\n";

    return out;
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
};

}


#endif // KDTREE_H
