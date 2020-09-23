#ifndef PRIORITYSEARCHTREE_H
#define PRIORITYSEARCHTREE_H

// 优先搜索树

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include "MyCommon.h"


namespace test {

template <typename E>
class PrioritySearchTree {
public:
  struct Element {
    std::string _name;
    Point2D<E> _point;
  };
  std::vector<Element> _points;
  int _size;

  PrioritySearchTree(int size) : _size(size) {
    _points.resize(_size);
    std::cout << "PrioritySearchTree(int size)\n";
  }
  PrioritySearchTree(std::string names[], E points[][2], int size)
    : PrioritySearchTree(size) {  // Nx2 points
    for (int i = 0; i < _size; i++) {
      _points[i]._name = names[i];
      _points[i]._point._x = points[i][0];
      _points[i]._point._y = points[i][1];
    }
    std::cout << "PrioritySearchTree(std::string names[], E points[][2])\n";
  }

  virtual ~PrioritySearchTree() {
    std::cout << "~PrioritySearchTree()\n";
  }

  void Print(const std::string& title) {
    std::cout << title << "\n";
    for (auto iter = _points.begin(); iter != _points.end(); iter++) {
      const Element& p = *iter;
      std::cout << p._name << ": (" << p._point._x << ", " << p._point._y << ")\n";
    }
  }

  void DoSort() {
    std::sort(_points.begin(), _points.end(), [](const Element& l, const Element& r){
      return (l._point._y > r._point._y) ||
          (l._point._y == r._point._y && l._point._x > r._point._x);
    });
  }
};



template <typename E>
class PSTMultiset {
public:
  struct Element {
    std::string _name;
    Point2D<E> _point;

    Element(const std::string& name, E arr[])
      : _name(name), _point(arr) {}
  };

  struct ElementComparatorY {
    bool operator()(const Element& l, const Element& r) const {  // 使用const有利于线程安全
      return (l._point._y > r._point._y) ||
          (l._point._y == r._point._y && l._point._x > r._point._x);
    }
  };
  struct ElementComparatorX {
    bool operator()(const Element& l, const Element& r) const {  // 使用const有利于线程安全
      return (l._point._x < r._point._x) ||
          (l._point._x == r._point._x && l._point._y < r._point._y);
    }
  };

  std::multiset<Element, ElementComparatorY> _points;
  typedef typename std::multiset<Element, ElementComparatorY>::iterator Iterator;
  int _size;

  PSTMultiset(int size) : _size(size) {
    std::cout << "PSTMultiset(int size)\n";
  }
  PSTMultiset(std::string names[], E points[][2], int size)
    : PSTMultiset(size) {  // Nx2 points
    for (int i = 0; i < _size; i++) {
      _points.insert(Element(names[i], points[i]));
    }
    std::cout << "PSTMultiset(std::string names[], E points[][2])\n";
  }

  virtual ~PSTMultiset() {
    Clear();
    std::cout << "~PSTMultiset()\n";
  }

  void Print(const std::string& title) {
    std::cout << title << "\n";
    for (auto iter = _points.begin(); iter != _points.end(); iter++) {
      const Element& p = *iter;
      std::cout << p._name << ": (" << p._point._x << ", " << p._point._y << ")\n";
    }
  }

  class Node {
  public:
    Element _element;
    E _separate_value;
    Node *_parent;
    Node *_left;
    Node *_right;

    Node(const Element& element, const E& separate_value, Node* parent)
      : _element(element),
        _separate_value(separate_value),
        _parent(parent),
        _left(nullptr),
        _right(nullptr) {

    }
    Node(const Element& element, Node* parent)
      : _element(element),
        _separate_value(element._point._x),
        _parent(parent),
        _left(nullptr),
        _right(nullptr) {

    }

    friend std::ostream& operator<<(std::ostream& out, Node* node) {
      if (node == nullptr) {
        return out;
      }

      if (node->_left != nullptr) {
        out << "L(" << node->_left->_element._name << ")_";
      }

      out << node->_element._name
          << ": (" << node->_element._point._x << ", "
          << node->_element._point._y << ")"
          << "[" << node->_separate_value << "]";

      if (node->_right != nullptr) {
        out << "_R(" << node->_right->_element._name << ")";
      }

      return out;
    }
  };

  Node *_root;

  Node* Build(Node* parent, std::multiset<Element, ElementComparatorY>& points) {

    if (points.size() == 0) return nullptr;
    if (points.size() == 1) {
      Node *node = new Node(*(points.begin()), parent);
      return node;
    }

    auto first = points.begin();

    std::multiset<Element, ElementComparatorX> remainPoints;
    auto iter = points.begin(); iter++;  // remove the first point (the point with greatest Y)
    for (; iter != points.end(); iter++) {
      remainPoints.insert(*iter);
    }

    auto mid_iter = remainPoints.begin();
    std::advance(mid_iter, remainPoints.size() / 2);

    E separate_value = (*mid_iter)._point._x;
    if (mid_iter != remainPoints.begin()) {
      auto tmp_iter = mid_iter;
      tmp_iter--;
      separate_value = static_cast<E>(((*tmp_iter)._point._x + separate_value) / 2);
    }

    std::multiset<Element, ElementComparatorY> leftPoints(remainPoints.begin(), mid_iter);
    std::multiset<Element, ElementComparatorY> rightPoints(mid_iter, remainPoints.end());

    Node *node = new Node(*first, separate_value, parent);
    node->_left = Build(node, leftPoints);
    node->_right = Build(node, rightPoints);

    return node;
  }

  void Build() {
    _root = Build(nullptr, _points);
  }

  void Clear() {  // clear the tree by level-order traverse
    if (_root == nullptr) return;
    std::queue<Node*> q;
    q.push(_root);
    int height = 1;
    int levelSize = q.size();
    while (!q.empty()) {
      Node *node = q.front();

      std::cout << node << " ";

      if (node->_left != nullptr) {
        q.push(node->_left);
      }
      if (node->_right != nullptr) {
        q.push(node->_right);
      }

      delete node;

      q.pop();
      levelSize--;
      if (levelSize == 0) {
        height++;
        levelSize = q.size();

        std::cout << "\n";
      }
    }

    std::cout << "Height: " << height << "\n";
  }

  // 找到那些节点，x坐标在[xmin,xmax]之间，y坐标大于ymin
  void SearchNode(Node* node,
                  const E& xmin, const E& xmax, const E& ymin,
                  std::vector<Node*>& results) {
    if (node == nullptr) return;

    if (node->_element._point._y < ymin) {
      return;
    }

    if (node->_element._point._x >= xmin &&
        node->_element._point._x <= xmax) {
      results.push_back(node);
    }

    if (xmin < node->_separate_value && node->_left != nullptr) {
      SearchNode(node->_left, xmin, xmax, ymin, results);
    }

    if (node->_separate_value < xmax && node->_right != nullptr) {
      SearchNode(node->_right, xmin, xmax, ymin, results);
    }
  }

  void Query(const E& xmin, const E& xmax, const E& ymin) {
    if (_root == nullptr) {
      std::cout << "Tree is empty. Stop query.\n";
    }

    std::vector<Node*> nodes;  // save all the result nodes
    SearchNode(_root, xmin, xmax, ymin, nodes);
    // print the nodes
    for (auto node : nodes) {
      std::cout << node << "\n";
    }
  }

};

// dynamic priority search tree 动态优先搜索树 (TODO)
/*
 * 插入 O(logn)
 * 删除 O(logn)
 */

}


#endif // PRIORITYSEARCHTREE_H
