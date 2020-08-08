#ifndef SKIPLIST_H
#define SKIPLIST_H

// https://github.com/paulross/skiplist
#include <iostream>
#include <queue>
#include "MyCommon.h"

namespace test {

template <typename K, typename V>
class SkipList {
public:
  class Node {
  public:
    static int used_count;
    K _key;
    V _value;
    int _levels;
    Node** _next;
    Node* _prev;

    Node(const K& key, int level, Node* prev, const V& value = V())
        : _key(key),
          _value(value),
          _levels(level),
          _next(new Node*[_levels]{nullptr}),
          _prev(prev) {
      used_count++;
      std::cout << "Node:Node(), key = " << _key
                << ", used_count = " << used_count << "\n";
    }
    ~Node() {
      delete[] _next;
      used_count--;
      std::cout << "Node:~Node(), key = " << _key
                << ", used_count = " << used_count << "\n";
    }

    friend std::ostream& operator<< (std::ostream& out, const Node* node) {
      if (node == nullptr) {
        out << "null";
        return out;
      }

      out << "{" << node->_key << "}";

      return out;
    }
  };

  class Pair {
  public:
    const K _first;
    V& _second;

    Pair() : _first(K()), _second(V()) {
    }
    Pair(const K& key, V& value)
        : _first(key),
          _second(value) {
    }
    Pair(const Pair& other)
        : _first(other._first),
          _second(other._second){
    }
  };

  class Iterator {
  private:
    Node* _current_node;
    Iterator(Node* node) {

    }
  public:
    Iterator() : _current_node(nullptr) {
    }

    Iterator& operator++() {
      if (_current_node != nullptr) {
        _current_node = _current_node->_next[0];
      }
      return *this;
    }

    Iterator& operator++(int) {
      Iterator copy = *this;
      ++(*this);
      return copy;
    }

    Iterator& operator--() {
      if (_current_node != nullptr) {
        _current_node = _current_node->_prev;
      }
      return *this;
    }

    Iterator& operator--(int) {
      Iterator copy = *this;
      --(*this);
      return copy;
    }

    Pair operator*() {
      return Pair(_current_node->_key, _current_node->_value);
    }

    bool operator==(const Iterator& rhs) const {
      return _current_node == rhs._current_node;
    }

    bool operator!=(const Iterator& rhs) const {
      return _current_node != rhs._current_node;
    }
  };

public:
  static int modulus_table[32];
//      = {
//      1, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767,
//      65535, 131071, 262143, 524287, 1048575, 2097151, 4194303, 8388607,
//      16777215, 33554431, 67108863, 134217727, 268435455, 536870911, 1073741823,
//      2147483647
//  };
  int _size;
  int _height;
  Node** _head;
  Node* _tail;
  int _index;
  int _prev_modules;
  int _modules;
  int _next_modules;

  SkipList()
      : _size(0),
        _height(1),
        _head(new Node*[_height]{nullptr}), //每一层都有个头部指针
        _tail(nullptr),
        _index(2),
        _prev_modules(1),
        _modules(1),
        _next_modules(3) {
    std::cout << "SkipList::SkipList()\n";
  }
  ~SkipList() {

    Node* node = _head[0];//nodes in level-0, height=0
    Node* tmp;
    while (node != nullptr) {
      tmp = node;
      node = node->_next[0];
      delete tmp;
    }
    delete[] _head;

    std::cout << "SkipList::~SkipList()\n";
  }

  bool IsEmpty() const {
    return _size == 0;
  }

  int Size() const {
    return _size;
  }

  V &operator[](const K& key) {
    if (IsEmpty()) {
      return V();
    }

    Iterator iter = Find(key);
    if (iter != End()) {
      return *(iter)._second;
    }


  }

  V& Push(const K& key, const V& value) {
    if (IsEmpty()) {  // 没有一个元素
      IncreaseSize();
      _head[0] = new Node(key, 1, nullptr, value);
      _tail = _head[0];
      return _head[0]->_value;
    }

    if (key < _head[0]->_key) { // 头部添加元素
      IncreaseSize();
      int h = NewHeight();
      Node* tmp = new Node(key, h, nullptr, value);
      _head[0]->_prev = tmp;
      for (int i = 0; i < h; ++i) {
        tmp->_next[i] = _head[i];
        _head[i] = tmp;
      }
      return _head[0]->_value;
    }

    // 下面就是向中间或者链表尾部添加元素

    int level;
    Node* node = nullptr;

    // 首先搜索第一个大于key的节点，结果保存在level和node里面
    SearchHead(key, level, node);

    // 保存该节点每一层的之前的节点
    Node** prev_nodes = new Node*[level + 1];

    for (int i = level; i >= 0; --i) {
      while (true) {
        if (node->_next[i] == nullptr
            || node->_next[i]->_key > key) {
          break;
        }
        node = node->_next[i];//向上层查找
      }

      if (node->_key == key) {
        // 如果关键字已经存在，返回该关键字的键值
        return node->_value;
      } else {
        // 找到了每一层的前一个节点，且这个节点的key小于给定的key
        prev_nodes[i] = node;
      }
    }

    IncreaseSize();

    if (node->_next[0] == nullptr) {
      //如果前一个节点是尾部节点，表示新插入的节点要插入到尾部
      int h = NewHeight();
      Node* tmp = new Node(key, h, node, value);
      for (int i = 0; i < h && i <= level; ++i) {
        prev_nodes[i]->_next[i] = tmp;
      }
      for (int i = level + 1; i < h; ++i) {
        _head[i] = tmp;
      }

      _tail = tmp; //将链表尾部指向新插入的节点

      delete[] prev_nodes;
      return tmp->_value;
    } else {
      //这里表示新插入的节点是插入在链表中间
      int h = NewHeight();
      Node* tmp = new Node(key, h, node, value);
      for (int i = 0; i < h && i <= level; ++i) {
        tmp->_next[i] = prev_nodes[i]->_next[i];
        prev_nodes[i]->_next[i] = tmp;
      }
      tmp->_next[0]->_prev = tmp;
      for (int i = level + 1; i < h; ++i) {
        tmp->_next[i] = _head[i];
        _head[i] = tmp;
      }
      delete[] prev_nodes;
      return tmp->_value;
    }
  }

  Iterator Begin() {
    return Iterator(_head[0]);
  }
  Iterator End() {
    return Iterator(nullptr);
  }

  Iterator Find(const K& key) {
    if (IsEmpty())
      return End();

    int level;
    Node* node = nullptr;
    SearchHead(key, level, node);

    if (node == nullptr) {
      return End();
    }

    return SearchList(key, level, node) ? Iterator(node) : End();
  }

  void SearchHead(const K& key, int& level, Node*& node) const {
    //从高层向低层搜索，返回第一个大于或者等于key的节点
    for (int i = _height - 1; i >= 0; i--) {
      if (_head[i]->_key <= key) {
        level = i;
        node = _head[i];
        return;
      }
    }
  }

  bool SearchList(const K& key, int level, Node*& node) const {
    //从这一层开始先向右找，没找到再向下找
    //找到了都会把节点赋值给node
    for (int i = level; i >= 0; i--) {
      while (true) {
        if (node->_next[i] == nullptr
            || node->_next[i]->_key > key) {
          break;
        }
        node = node->_next[i];
      }
      if (node->_key == key) {
        return true;
      }
    }
    return false;
  }

  void IncreaseSize() {
    ++_size;
    if (_size == _next_modules) {
      ++_index;
      _prev_modules = _modules;
      _modules = _next_modules;
      _next_modules = modulus_table[_index];
    }
  }

  void DecreaseSize() {
    --_size;
  }

  int NewHeight() {
    //这里没看懂
    int r = (rand() % _modules) + 1;

    for (int mask = 1, h = 1; true; mask <<= 1, ++h) {
      if (mask & r) {
        if (_height < h) {
          ResizeHeight(h);
        }
        return h;
      }
    }
  }

  void ResizeHeight(int new_height) {
    if (new_height < _height) {
      throw "new_height should be larger than original height";
      return;
    }

    Node** new_head = new Node*[new_height]{nullptr};
    for (int k = 0; k < _height; ++k) {
      new_head[k] = _head[k];
    }
    delete[] _head;
    _head = new_head;
    _height = new_height;
  }

  friend std::ostream &operator<<(std::ostream &out, const SkipList* list ) {
    out << "Size:     " << list->_size << std::endl;
    out << "Height:   " << list ->_height << std::endl;
//    out << "Total:    " << list.total_search() << std::endl;
//    out << "Average:  " << list.average_search() << std::endl;

    for ( int i = list->_height - 1; i >= 1; --i ) {
      out << i << "-";

      for (Node *ptr = list->_head[0]; ptr != 0; ptr = ptr->_next[0] ) {
        if ( ptr->_levels > i ) {
          out << "|";
        } else {
          out << "-";
        }
      }

      out << "-> 0" << std::endl;
    }

    out << "0-";

    for (Node* ptr = list->_head[0]; ptr != 0; ptr = ptr->_next[0] ) {
      out << "*";
    }

    out << "-> 0";

    if ( !list->IsEmpty() ) {
      out << std::endl << list->_head[0]->_key;

      for (Node* ptr = list->_head[0]->_next[0]; ptr != 0; ptr = ptr->_next[0] ) {
        out << ", " << ptr->_key;
      }
    }

    out << "\n";

    return out;
  }


};

}

#endif // SKIPLIST_H
