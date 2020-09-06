#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <string>
#include <map>
#include <queue>
#include "MyCommon.h"
using std::map;
using std::string;
using std::queue;

namespace test {


/*
 * 前缀树 prefix tree
 */
template <typename V>
class Trie {
public:
  class Node {
  public:
    static int used_count;
    Node* _parent;
    map<char, Node*> _children;
    bool _word;
    char _charactor;
    V* _value;
    Node(Node* parent)
      : _parent(parent),
        _children(),
        _word(false),
        _charactor(' '),
        _value(nullptr) {
      std::cout << "Node::Node(), used_count = " << used_count << "\n";
      used_count++;
    }

    ~Node() {

      if (_value != nullptr) {
        delete _value; _value = nullptr;
      }

      used_count--;
      std::cout << "Node::~Node(), charactor = " << _charactor << ", used_count = " << used_count << "\n";
    }

    friend std::ostream& operator<< (std::ostream& out, const Node* rhs) {
      if (rhs == nullptr) {
        out << "null";
        return out;
      }
      out << "(" << rhs->_charactor  // the charactor
          << "_W" << rhs->_word     // if this is a word node
          << "_C" << rhs->_children.size()    // the number of children nodes
          << ")";

      if (rhs->_parent != nullptr) {
        out << "_P(" << rhs->_parent->_charactor << ")";
      } else {
        out << "_P(null)";
      }

      if (rhs->_value != nullptr) {
        out << "_V(" << rhs->_value << ")";
      } else {
        out << "_V(null)";
      }
      return out;
    }
  };

private:
  int _size;
  Node* _root;

public:

  Trie() : _size(0), _root(nullptr) {
    std::cout << "Trie::Trie()\n";
  }

  virtual ~Trie() {
    Clear();
    std::cout << "Trie::~Trie()\n";
  }

  void Clear() {
    if (_root == nullptr) return;

    // 层序遍历删除每个节点
    std::queue<Node*> q;
    q.push(_root);
    while (!q.empty()) {
      Node* node = q.front();

      if (!node->_children.empty()) {
        for(auto& elem : node->_children) {
          q.push(elem.second);
        }
      }
      q.pop();

      delete node; node = nullptr;
    }

    _root = nullptr;
    _size = 0;
  }

  int Size() const {
    return _size;
  }

  bool IsEmpty() const {
    return _size == 0;
  }

  V* Get(const string& key) {
    Node* node = GetNode(key);
    return node != nullptr && node->_word ? node->_value : nullptr;
  }

  bool Contains(const string& key) {
    Node* node = GetNode(key);
    return node != nullptr && node->_word;
  }

  V* Add(const string& key, V* value) {
    KeyCheck(key);

    if (_root == nullptr) {
      _root = new Node(nullptr);
    }

    Node* node = _root;
    size_t len = key.length();
    for (size_t i = 0; i < len; i++) {
      char c = key.at(i);
      //先判断这个字符是否存在
      bool emptyChildren = node->_children.empty();
      Node* childNode = emptyChildren ? nullptr : node->_children[c];
      //如果字符节点不存在，创建
      if (childNode == nullptr) {
        childNode = new Node(node); //设置父节点
        childNode->_charactor = c; //设置字符
        //设置父节点的字符节点
        node->_children[c] = childNode;
      }
      node = childNode; //继续向下
    }

    // 如果到了最后一个字符，那个节点的word标记为单词结束
    if (node->_word) {
      V* oldValue = node->_value; // 获取节点的value
      node->_value = value; // 设置节点的新的value
      return oldValue;
    }

    node->_word = true; //标记当前节点是单词结束
    node->_value = value; //覆盖value
    _size++;   //表示新添加了一个单词
    return nullptr;
  }

  V* Remove(const string& key) {
    Node* node = GetNode(key); //首先获取单词对应的节点
    //如果不存在这个单词，返回
    if (node == nullptr || !node->_word)
      return nullptr;

    //到此，表示找到了这个单词


    _size--;
    V* oldValue = node->_value;

    //如果还有其他字符，将这个节点的单词标记设为false，即可删除该单词
    if (!node->_children.empty()) {
      node->_word = false;
      return oldValue;
    }

    // 如果该节点没有其他字符了，向上删除
    Node* parent = nullptr;
    while ((parent = node->_parent) != nullptr) {

      parent->_children.erase(node->_charactor);
      // 如果父节点是一个单词或者父节点还有其他字符，删除停止
      if (parent->_word || !parent->_children.empty()) {
        delete node;
        break;
      }
      //否则继续向上删除
      delete node;
      node = parent;
    }

    return oldValue;
  }

  bool StartsWith(const string& prefix) {
    return GetNode(prefix) != nullptr;
  }

  // 层序遍历输出二叉树
  friend std::ostream& operator<<(std::ostream& out, const Trie* rhs) {
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

      if (!node->_children.empty()) {
        for(auto& elem : node->_children) {
          q.push(elem.second);
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
  Node* GetNode(const string& key) {
    KeyCheck(key);

    Node* node = _root;
    size_t len = key.length();
    for (size_t i = 0; i < len; i++) {
      if (node == nullptr || node->_children.empty())
        return nullptr;
      char c = key.at(i);
      node = node->_children[c];
    }
    return node;
  }

  void KeyCheck(const string& key) {
    if (key.length() == 0) {
      throw "Key is empty";
    }
  }

};

}

#endif // TRIE_H
