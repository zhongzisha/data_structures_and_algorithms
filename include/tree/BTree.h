#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <vector>
#include "MyCommon.h"

namespace test {

/*
 * B树的一个节点可以包含多个元素，一个节点可以有多个子节点
 * 假设单个Node里面可以包含的最小元素个数为MINIMUM
 * 1. 根节点至少包含一个元素(如果没有子节点，则可以为空),其它节点至少包含MININUM个元素
 * 2. 一个节点里面最多可以包含2*MININUM个元素
 * 3. 每个节点的元素存储在一个部分填充的数组上，由小到大存储
 * 4. 对于非叶子节点来说，其子节点个数总是等于节点元素个数+1
 * 5. 对于任意非叶子节点，第i元素总是大于第i个子树上的所有元素，总是小于第i+1个子树上的所有元素
 * 6. 每个叶子节点有相同的深度
 */
template <typename E>
class BTree {
public:
  class Node {
  public:
    static int use_count;
    //    static std::vector<Node*> node_pointers;
    std::vector<E> _keys;
    std::vector<Node*> _children;
    int _num_keys;
    bool _is_leaf;
    Node* _parent;

    Node(int max_keys) {
      std::cout << "Node::Node()\n";
      use_count++;
      _num_keys = 0;
      _keys.resize(max_keys + 1);
      _children.resize(max_keys + 2, nullptr);
      _is_leaf = true;
      _parent = nullptr;
    }
    ~Node() {
      std::cout << "Node::~Node()\n";
      use_count--;
    }

    bool operator==(const Node& rhs) {
      std::cout << "Node::operator==(const Node&)\n";
      return *this == rhs;
    }
    bool operator==(const Node* rhs) {
      std::cout << "Node::operator==(const Node*)\n";
      return this == rhs;
    }

    friend std::ostream& operator<< (std::ostream& out, const Node* rhs) {
      if (rhs == nullptr) {
        out << "null";
        return out;
      }

      if (rhs->_parent != nullptr) {
        out << "P(" ;
        for (int i = 0; i < rhs->_parent->_num_keys; i++) {
          if (i > 0)
            out << ", ";
          out << rhs->_parent->_keys[i];
        }
        out << ")_";
      } else {
        out << "P(null)_";
      }

      out << "K(" ;
      for (int i = 0; i < rhs->_num_keys; i++) {
        if (i > 0)
          out << ", ";
        out << rhs->_keys[i];
      }
      out << ")";

      if (rhs->_is_leaf) {
        out << "_L";
      }

      return out;
    }
  };

public:
  int _max_degree;
  int _min_keys;
  int _max_keys;
  int _split_index;
  Node* _root;

  BTree() : BTree(4) {} // 2-3-4树，一个节点最多3个子树

  BTree(int degree) {
    std::cout << "BTree::BTree(int degree)\n";
    _max_degree = degree;
    _min_keys = (degree + 1) / 2 - 1; // Math.floor((newDegree + 1) / 2) - 1;
    _max_keys = degree - 1;
    _split_index = (degree - 1) / 2; // Math.floor((newDegree - 1) / 2);
    _root = nullptr;
  }

  ~BTree() {
    std::cout << "BTree::~BTree()\n";
    Clear();

    //    for(auto& p : Node::node_pointers) {
    //      std::cout << p << "\n";
    //      delete p; p = nullptr;
    //    }
  }

  void Add(const E& element) {
    if (_root == nullptr) {
      _root = new Node(_max_keys);
      //      Node::node_pointers.push_back(_root);
      _root->_num_keys = 1;
      _root->_keys[0] = element;
    } else {
      Insert(_root, element);
    }
  }

  void Remove(const E& element) {
    Delete(_root, element);

    if (_root != nullptr && _root->_num_keys == 0) {
      _root = _root->_children[0];
      _root->_parent = nullptr;
    }
  }

  void Clear() {
    if (_root == nullptr) return;

    DeleteTree(_root);
    delete _root;
  }

  // 层序遍历输出二叉树
  friend std::ostream& operator<<(std::ostream& out, const BTree& rhs) {
    out << "Number of BTree Nodes Used = " << Node::use_count << "\n";
    if (rhs._root == nullptr) {
      out << "Empty BTree.\n";
      return out;
    }

    out << "max_degree = " << rhs._max_degree << "\n";
    out << "min_keys = " << rhs._min_keys << "\n";
    out << "max_keys = " << rhs._max_keys << "\n";
    out << "split_index = " << rhs._split_index << "\n";
    Print(out, rhs._root);

    // out << "RBTree Height = " << height << "\n";

    return out;
  }

protected:

  static void Print(std::ostream& out, Node* node) {
    out << node << "\n";
    if (node->_is_leaf) {
    } else {
      Print(out, node->_children[0]);
      for (int i = 0; i < node->_num_keys; i++) {
        Print(out, node->_children[i+1]);
      }
    }
  }

  void Insert(Node* node, const E& element) {
    if (node->_is_leaf) {
      node->_num_keys++;
      int insertIndex = node->_num_keys - 1;
      while (insertIndex > 0 && node->_keys[insertIndex - 1] > element) {
        node->_keys[insertIndex] = node->_keys[insertIndex - 1];
        insertIndex--;
      }
      node->_keys[insertIndex] = element;
      InsertRepair(node);
    } else {
      int findIndex = 0;
      while (findIndex < node->_num_keys && node->_keys[findIndex] < element) {
        findIndex++;
      }
      Insert(node->_children[findIndex], element);
    }
  }

  void InsertRepair(Node* node) {
    if (node->_num_keys <= _max_keys) {
      return;
    } else if (node->_parent == nullptr) {
      _root = Split(node);
      return;
    } else {
      Node* newNode = Split(node);
      InsertRepair(newNode);
    }
  }


  Node* Split(Node* node) {
    std::cout << "Split\n";

    Node* rightNode = new Node(_max_keys);
    //    Node::node_pointers.push_back(rightNode);
    rightNode->_num_keys = node->_num_keys - _split_index - 1;
    E risingElement = node->_keys[_split_index];

    if (node->_parent != nullptr) {
      Node* currentParent = node->_parent;
      int parentIndex;
      for (parentIndex = 0;
           parentIndex < currentParent->_num_keys + 1
           && currentParent->_children[parentIndex] != node;
           parentIndex++);
      if (parentIndex == currentParent->_num_keys + 1) {
        throw "Could not find which child we were!";
      }
      for (int i = currentParent->_num_keys; i > parentIndex; i--) {
        currentParent->_children[i + 1] = currentParent->_children[i];
        currentParent->_keys[i] = currentParent->_keys[i - 1];
      }
      currentParent->_num_keys++;
      currentParent->_keys[parentIndex] = risingElement;

      currentParent->_children[parentIndex + 1] = rightNode;
      rightNode->_parent = currentParent;
    }

    int i;
    for (i = _split_index + 1; i < node->_num_keys + 1; i++) {
      rightNode->_children[i - _split_index - 1] = node->_children[i];
      if (node->_children[i] != nullptr) {
        rightNode->_is_leaf = false;
        if (node->_children[i] != nullptr) {
          node->_children[i]->_parent = rightNode;
        }
        node->_children[i] = nullptr;
      }
    }
    for (i = _split_index + 1; i < node->_num_keys; i++) {
      rightNode->_keys[i - _split_index - 1] = node->_keys[i];
    }

    Node* leftNode = node;
    leftNode->_num_keys = _split_index;

    if (node->_parent != nullptr) {
      return node->_parent;
    } else {
      _root = new Node(_max_keys);
      //      Node::node_pointers.push_back(_root);
      _root->_num_keys = 1;
      _root->_keys[0] = risingElement;
      _root->_children[0] = leftNode;
      _root->_children[1] = rightNode;
      leftNode->_parent = _root;
      rightNode->_parent = _root;
      _root->_is_leaf = false;
      return _root;
    }
  }

  Node* MergeRight(Node* node) {
    //node为被删除了元素的节点
    Node* parentNode = node->_parent;
    int parentIndex = 0;
    for (parentIndex = 0; parentNode->_children[parentIndex] != node; parentIndex++);
    //右兄弟中的内容合并到自己这里
    Node* rightSib = parentNode->_children[parentIndex + 1];
    // 父节点中的那个元素也添加到自己节点这里
    node->_keys[node->_num_keys] = parentNode->_keys[parentIndex];
    for (int i = 0; i < rightSib->_num_keys; i++) {
      node->_keys[node->_num_keys + 1 + i] = rightSib->_keys[i];
    }

    //如果节点不是叶子节点，需要将有兄弟中的子树移动到自己这里，并且设置它们的父节点指向自己
    if (!node->_is_leaf) {
      for (int i = 0; i <= rightSib->_num_keys; i++) {
        node->_children[node->_num_keys + 1 + i] = rightSib->_children[i];
        node->_children[node->_num_keys + 1 + i]->_parent = node;
      }
    }

    // 因为右兄弟已经合并了，父节点中要删除负责这个子树的children
    for (int i = parentIndex + 1; i < parentNode->_num_keys; i++) {
      parentNode->_children[i] = parentNode->_children[i + 1];
      parentNode->_keys[i - 1] = parentNode->_keys[i];
    }

    //父节点中的元素个数减1，因为右兄弟已经合并了
    parentNode->_num_keys--;
    //当前节点的元素个数等于原来的元素个数+右兄弟的元素个数+1
    node->_num_keys = node->_num_keys + rightSib->_num_keys + 1;
    delete rightSib; //因为右兄弟已经被删除，需要删除该节点
    return node;
  }

  Node* BorrowFromLeft(Node* node, int parentIndex) {
    //node是被删除了元素的节点,parentIndex是该节点在父节点中子树的索引
    Node* parentNode = node->_parent; //获取当前节点的父节点
    node->_num_keys++;  //当前节点的元素个数加1，因为会从左边子树中借一个元素过来
    int i;
    // 因为从左边借元素过来，肯定比这个节点里的元素都小，左右将当前节点的元素都右移
    // 为新元素腾出位置
    for (i = node->_num_keys - 1; i > 0; i--) {
      node->_keys[i] = node->_keys[i - 1];
    }
    //获取左兄弟
    Node* leftSib = parentNode->_children[parentIndex - 1];

    if (!node->_is_leaf) { //如果左兄弟不是叶子节点?
      //如果不是叶子节点，必然存在子树，所以将子树右移，为新的子树腾出空间
      for (int i = node->_num_keys; i > 0; i--) {
        node->_children[i] = node->_children[i - 1];
      }
      //第一个子树为左兄弟的最后一颗子树
      node->_children[0] = leftSib->_children[leftSib->_num_keys];
      //左兄弟的最后一个子树指向空
      leftSib->_children[leftSib->_num_keys] = nullptr;
      //第一个子树的父节点指向当前节点
      node->_children[0]->_parent = node;
    }

    // 父节点的元素下来，成为当前节点的第一个元素
    node->_keys[0] = parentNode->_keys[parentIndex - 1];
    // 左兄弟的最后一个元素覆盖父节点的元素
    parentNode->_keys[parentIndex - 1] = leftSib->_keys[leftSib->_num_keys - 1];
    leftSib->_num_keys--;

    if (leftSib->_num_keys == 0) {
      std::cout << "leftSib is nullptr\n";
    }

    return node;
  }

  Node* BorrowFromRight(Node* node, int parentIndex) {
    //node是被删除了元素的节点,parentIndex是该节点在父节点中子树的索引
    Node* parentNode = node->_parent;
    //获取当前节点的右兄弟
    Node* rightSib = parentNode->_children[parentIndex + 1];
    //当前节点会从右兄弟中借一个元素来，因此元素个数加1
    node->_num_keys++;
    //当前节点的最后一个元素为父节点中下来的那个元素
    node->_keys[node->_num_keys - 1] = parentNode->_keys[parentIndex];
    //父节点中的那个位置用右兄弟的第一个元素覆盖
    parentNode->_keys[parentIndex] = rightSib->_keys[0];

    if (!node->_is_leaf) { //如果当前节点不是叶子节点,还需要移动子树
      //当前节点的子树为右兄弟的第一个子树
      node->_children[node->_num_keys] = rightSib->_children[0];
      //当前节点子树的父节点指向当前节点
      node->_children[node->_num_keys]->_parent = node;
      //因为右兄弟中移除了第一颗子树，因此需要将后面的子树前移
      for(int i = 1; i < rightSib->_num_keys + 1; i++) {
        rightSib->_children[i - 1] = rightSib->_children[i];
      }
    }

    //因为右兄弟移除了一个元素(上去到了父节点中),因此需要将后面的元素前移
    for (int i = 1; i < rightSib->_num_keys; i++) {
      rightSib->_keys[i - 1] = rightSib->_keys[i];
    }
    //右兄弟的元素个数减1
    rightSib->_num_keys--;

    if (rightSib->_num_keys == 0) {
      std::cout << "rightSib is nullptr\n";
    }

    return node;
  }

  void Delete(Node* node, const E& element) {
    if (node == nullptr) return;
    // 顺序查找该节点下的元素
    int i;
    for (i = 0; i < node->_num_keys && node->_keys[i] < element; i++);
    // 开始删除
    if (i == node->_num_keys) { // 当前节点的元素都比删除元素小，进入最后一颗子树，重复
      if (!node->_is_leaf) {
        Delete(node->_children[node->_num_keys], element);
      }
    } else if (node->_keys[i] > element) { // 当前节点的元素都比删除元素大，i=0
      if (!node->_is_leaf) {  //如果当前节点不是叶子节点，进入第一颗子树，重复
        Delete(node->_children[i], element);
      }
      else { // 如果当前节点是叶子节点，进入此处表示没找到需要删除的元素

      }
    } else { // 如果在当前的节点找到了需要删除的元素
      if (node->_is_leaf) { //如果当前节点是叶子节点
        // 将后面的元素移动前移(没有children,因为不需要前移)
        for (int j = i; j < node->_num_keys - 1; j++) {
          node->_keys[j] = node->_keys[j + 1];
        }
        node->_num_keys--; // 当前节点元素个数减1
        RepairAfterDelete(node);
      } else { //如果当前节点不是叶子节点
        //i指向节点里面要删除的元素的位置
        Node* maxNode = node->_children[i]; //第一个比删除元素小的节点
        while (!maxNode->_is_leaf) {//沿着子树向右找，找到叶子节点
          maxNode = maxNode->_children[maxNode->_num_keys];
        }
        //叶子节点里面的最大一个元素覆盖要删除的元素
        node->_keys[i] = maxNode->_keys[maxNode->_num_keys - 1];
        maxNode->_num_keys--;
        //那个叶子节点里的元素个数减1,然后问题转变为从该节点删除了一个元素
        RepairAfterDelete(maxNode);
      }
    }
  }

  void RepairAfterDelete(Node* node) {
    if (node->_num_keys < _min_keys) { //如果删除元素后，节点元素个数小于最小个数
      if (node->_parent == nullptr) { //如果节点父节点为空
        if (node->_num_keys == 0) { // 如果节点元素个数为0
          _root = node->_children[0];  // 根节点指向节点的第一颗子树
          if (_root != nullptr) {
            _root->_parent = nullptr; //根节点的父节点置为空
          }

          delete node; // 此时,node节点元素已经为空，且如果有子树，也是被根节点指向，因此可以删除这个节点了
        }
      } else { //如果父节点不为空，删除的肯定是叶子节点
        Node* parentNode = node->_parent; //得到当前节点的父节点
        // 找到当前节点在父节点中的哪一个子树
        int parentIndex;
        for (parentIndex = 0; parentNode->_children[parentIndex] != node; parentIndex++);
        if (parentIndex > 0 &&
            parentNode->_children[parentIndex - 1]->_num_keys > _min_keys) {
          //从左边子树借元素
          BorrowFromLeft(node, parentIndex);
        } else if (parentIndex < parentNode->_num_keys &&
                   parentNode->_children[parentIndex + 1]->_num_keys > _min_keys) {
          //从右边子树借元素
          BorrowFromRight(node, parentIndex);
        } else if (parentIndex == 0) {
          //如果左右子树都不能借元素，则合并
          Node* nextNode = MergeRight(node);
          RepairAfterDelete(nextNode->_parent);
        } else {
          Node* nextNode = MergeRight(parentNode->_children[parentIndex - 1]);
          RepairAfterDelete(nextNode->_parent);
        }
      }
    }
  }

  void DeleteTree(Node* node) {
    if (node == nullptr) return;
    if (!node->_is_leaf) {
      for (int i = 0; i <= node->_num_keys; i++) {
        DeleteTree(node->_children[i]);

        if (node->_children[i] != nullptr) {
          delete node->_children[i];
          node->_children[i] = nullptr;
        }
      }
    }
  }

};

}

#endif // BTREE_H
