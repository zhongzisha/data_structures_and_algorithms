#ifndef RADIXTREE_H
#define RADIXTREE_H

#include <iostream>
#include <string>       // string
#include <algorithm>    // transform
#include <queue>        // queue
#include "MyCommon.h"
using std::string;

namespace test {

template <typename V>
class RadixTree {
public:
  class Node {
    static int used_count;
  public:
    V* _value;
    string _wordRemain;
    Node* _parent;
    Node* _children[26];
    bool _word;

    Node() : _value(nullptr),
             _wordRemain(),
             _parent(nullptr),
             _word(false) {
      used_count++;
      std::cout << "Node::Node(), used_count = " << used_count << "\n";
      for (int i = 0; i < 26; i++) {
        _children[i] = nullptr;
      }
    }

    Node(const string& key, V* value = nullptr)
        : _value(value),
          _wordRemain(key),
          _parent(nullptr),
          _word(false) {
      used_count++;
      std::cout << "Node::Node(const string& key), used_count = " << used_count << "\n";
      for (int i = 0; i < 26; i++) {
        _children[i] = nullptr;
      }
    }


    ~Node() {

      if (_value != nullptr) {
        delete _value; _value = nullptr;
      }

      used_count--;
      std::cout << "Node::~Node()"
                << ", used_count = " << used_count
                << ", wordRemain = " << _wordRemain
                << "\n";
    }

    bool IsLeaf() const {
      if (this == nullptr) return false;
      for (int i = 0; i < 26; i++) {
        if (_children[i] != nullptr)
          return false;
      }
      return true;
    }

    int GetParentIndex() const {
      if (_parent == nullptr) return -1;
      Node* parent = _parent;
      for (int i = 0; i < 26; i++) {
        if (parent->_children[i] == this)
          return i;
      }
      return -1;
    }

    int NumOfChildren() const {
      int childCount = 0;
      for (int i = 0; i < 26; i++) {
        if (_children[i] != nullptr)
          childCount++;
      }
      return childCount;
    }

    friend std::ostream& operator<< (std::ostream& out, const Node* node) {
      if (node == nullptr) {
        out << "Empty Tree.\n";
        return out;
      }

      out << "{" << node->_wordRemain << "}";

      if (node->_word) {
        out << "_W";
      }

      // for parent
      if (node->_parent != nullptr) {
        out << "_P(" << node->_parent->_wordRemain << ")";
      } else {
        out << "_P()";
      }

      // for child
      int childCount = 0;
      for (int i = 0; i < 26; i++)  {
        if (node->_children[i] != nullptr) {
          childCount++;
        }
      }
      out << "_C" << childCount;

      // for value
      if (node->_value != nullptr) {
        out << "_V(" << node->_value << ")";
      } else {
        out << "_V(null)";
      }

      return out;
    }
  };

public:
  int _size;
  Node* _root;

  RadixTree() : _size(0), _root(nullptr) {
    std::cout << "RadixTree::RadixTree()\n";
  }

  virtual ~RadixTree() {
    Clear();
    std::cout << "RadixTree::~RadixTree()\n";
  }

  int Size() const {
    return _size;
  }

  bool IsEmpty() const {
    return _size == 0;
  }

  void Add(const string& key, V* value) {
    // std::transform(key.begin(), key.end(), key.begin(), ::toupper);

    if (key.length() == 0)
      return;

    _root = Insert(_root, key, value);

  }

  void Remove(const string& key) {
    if (key.length() == 0)
      return;

    // 找到要删除的那个节点
    Node* node = Find(_root, key);
    if (node != nullptr) {
      node->_word = false;
      AfterRemove(node);
    }
  }

  void Clear() {
    if (_root == nullptr) return;

    // 层序遍历删除每个节点
    std::queue<Node*> q;
    q.push(_root);
    while (!q.empty()) {
      Node* node = q.front();

      for (int i = 0; i < 26; i++) {
        if (node->_children[i] != nullptr) {
          q.push(node->_children[i]);
        }
      }
      q.pop();

      delete node; node = nullptr;
    }

    _root = nullptr;
    _size = 0;
  }

  bool Contains(const string& key) {
    if (key.length() == 0)
      return false;

    Node* node = Find(_root, key);

    if (node == nullptr) {
      std::cout << "Not Found: " << key << "\n";
      return false;
    } else {
      std::cout << "Found: " << key << ", value is: " << node->_value << "\n";
      return true;
    }
  }

  friend std::ostream& operator<< (std::ostream& out, const RadixTree* rhs) {
    if (rhs == nullptr) {
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

      for (int i = 0; i < 26; i++) {
        if (node->_children[i] != nullptr) {
          q.push(node->_children[i]);
        }
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

private:


  Node* Insert(Node* node, const string& key, V* value) {
    if (node == nullptr) {
      node = new Node(key, value);
      node->_word = true;
      return node;
    }

    string wordRemain = node->_wordRemain;

    size_t indexDifference = FindIndexDifference(key, wordRemain);

    if (indexDifference == wordRemain.length()) {
      if (key.length() > indexDifference) {
        int index = key[indexDifference] - 'a';
        node->_children[index] = Insert(node->_children[index],
                                        key.substr(indexDifference),
                                        value);
        node->_children[index]->_parent = node;
        return node;
      }

      node->_word = true;
      node->_value = value;
      return node;
    }

    string firstRemain = node->_wordRemain.substr(0, indexDifference);
    string secondRemain = node->_wordRemain.substr(indexDifference);
    Node* newNode = new Node(firstRemain);
    int index = node->_wordRemain[indexDifference] - 'a';
    newNode->_parent = node->_parent;
    newNode->_children[index] = node;
    if(node->_parent != nullptr) {
      int childIndex = newNode->_wordRemain[0] - 'a';
      node->_parent->_children[childIndex] = newNode;
      node->_parent = newNode;
    } else {
      _root = newNode;
    }
    node->_parent = newNode;
    node->_wordRemain = secondRemain;

    if (indexDifference == key.length()) {
      newNode->_word = true;
      newNode->_value = value;
    } else {
      index = key[indexDifference] - 'a';
      newNode->_children[index] = Insert(nullptr,
                                         key.substr(indexDifference),
                                         value);
      newNode->_children[index]->_parent = newNode;
    }
    return newNode;
  }

  Node* Find(Node* node, const string& key) {
    if (node == nullptr) return nullptr;

    string wordRemain = node->_wordRemain;
    size_t indexDifference = FindIndexDifference(key, wordRemain);
    if (indexDifference == wordRemain.length()) {
      if (key.length() > indexDifference) {
        int index = key[indexDifference] - 'a';
        bool noChild = node->_children[index] == nullptr;
        if (noChild) {
          return nullptr;
        }
        return Find(node->_children[index], key.substr(indexDifference));
      }

      if (node->_word) {
        return node;
      } else {
        return nullptr;
      }

    }
    return nullptr;
  }

  size_t FindIndexDifference(const string& key1, const string& key2) {
    size_t index = 0;
    while (index < key1.length() && index < key2.length()) {
      if(key1[index] == key2[index]) {
        index++;
      } else {
        break;
      }
    }
    return index;
  }

  void AfterRemove(Node* node) {
    //node是待删除的节点
    if (node == nullptr) return;

    // 获取那个节点的子节点个数
    int numChildren = node->NumOfChildren();
    if (numChildren == 0 && !node->_word) {//如果子节点为0，且不是单词节点
      if (node->_parent != nullptr) {  //
        int parentIndex = node->GetParentIndex();
        node->_parent->_children[parentIndex] = nullptr;
        AfterRemove(node->_parent);
        delete node;
      } else { // 如果父节点为空，也没有子节点，直接删除
        delete node;
        _root = nullptr;
      }
    } else if (numChildren == 1 && !node->_word) {//如果包含一个孩子，且不是单词节点
      int childIndex = -1;
      for (int i = 0; i < 26; i++) {
        if (node->_children[i] != nullptr) {
          childIndex = i;
          break;
        }
      }

      Node* child = node->_children[childIndex];
      child->_wordRemain = node->_wordRemain + child->_wordRemain;

      if (node->_parent == nullptr) {
        child->_parent = nullptr;
        delete node; //删除该节点
        _root = child;
      } else {
        int parentIndex = node->GetParentIndex();
        node->_parent->_children[parentIndex] = child;
        child->_parent = node->_parent;
        delete node;
      }

    }

  }
};


}

#endif // RADIXTREE_H
