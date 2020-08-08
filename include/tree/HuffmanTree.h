#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <queue>
#include "MyCommon.h"
#include "heap/BinaryHeap.h"


namespace test {

template <typename E>
class HuffmanTree {
public:
  class Node {
  public:
    static int used_count;
    E _element;
    Node* _left;
    Node* _right;
    Node* _parent;

    Node(const E& element)
      : _element(element),
      _left(nullptr),
      _right(nullptr),
      _parent(nullptr) {
      used_count++;
      std::cout << "Node::Node() " << _element
                << ", used_count = " << used_count << "\n";
    }
    Node(const E& element, Node* left, Node* right, Node* parent)
      : _element(element),
      _left(left),
      _right(right),
      _parent(parent) {
      used_count++;
      std::cout << "Node::Node() " << _element
                << ", used_count = " << used_count << "\n";
    }
    ~Node() {
      used_count--;
      std::cout << "Node::~Node() " << _element
                << ", used_count = " << used_count << "\n";
    }

    bool operator<(const Node& rhs) {
      return _element < rhs._element;
    }
    bool operator>(const Node& rhs) {
      return _element > rhs._element;
    }
    bool operator<=(const Node& rhs) {
      return _element <= rhs._element;
    }
    bool operator>=(const Node& rhs) {
      return _element >= rhs._element;
    }
    bool operator==(const Node& rhs) {
      return _element == rhs._element;
    }
    bool operator!=(const Node& rhs) {
      return _element != rhs._element;
    }
    E operator-(const Node& rhs) {
      return _element - rhs._element;
    }

    bool operator<(const Node*& rhs) {
      return _element < rhs->_element;
    }
    bool operator>(const Node*& rhs) {
      return _element > rhs->_element;
    }
    bool operator<=(const Node*& rhs) {
      return _element <= rhs->_element;
    }
    bool operator>=(const Node*& rhs) {
      return _element >= rhs->_element;
    }
    bool operator==(const Node*& rhs) {
      return _element == rhs->_element;
    }
    bool operator!=(const Node*& rhs) {
      return _element != rhs->_element;
    }
    E operator-(const Node*& rhs) {
      return _element - rhs->_element;
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
public:
  Node* _root;

  HuffmanTree() : _root(nullptr) {
    std::cout << "HuffmanTree::HuffmanTree()\n";
  }

  ~HuffmanTree() {
    DeleteTreeIteratively(_root);
    std::cout << "HuffmanTree::~HuffmanTree()\n";
  }

  void Create(E* data, size_t size) {
    Node* left;
    Node* right;
    Node* parent;
    Node** node_array = new Node*[size];
    for (size_t i = 0; i < size; i++) {
      node_array[i] = new Node(data[i]);
    }
    //这里将指针类型作为堆的元素类型，需要重载比较操作，堆需要元素具有可比较性
    BinaryHeap<Node*>* heap = new BinaryHeap<Node*>(node_array, size);
    for (size_t i = 0; i < size - 1; i++) {
      left = heap->Pop();
      right = heap->Pop();
      parent = new Node(left->_element + right->_element, left, right, nullptr);
      left->_parent = parent;
      right->_parent = parent;

      heap->Add(parent);
    }

    _root = parent;
    delete heap;
    delete[] node_array;
  }


  // 层序遍历输出二叉树
  friend std::ostream& operator<<(std::ostream& out, const HuffmanTree* rhs) {
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


#endif // HUFFMANTREE_H
