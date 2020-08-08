#ifndef BPLUSTREE_H
#define BPLUSTREE_H


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
class BPlusTree {
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
    Node* _prev;
    Node* _next;

    Node(int max_keys) {
      std::cout << "Node::Node(), used_count = " << use_count << "\n";
      use_count++;
      _num_keys = 0;
      _keys.resize(max_keys + 1);
      _children.resize(max_keys + 2, nullptr);
      _is_leaf = true;
      _parent = nullptr;
      _prev = nullptr;
      _next = nullptr;
    }
    ~Node() {
      use_count--;
      std::cout << "Node::Node(), used_count = " << use_count << "\n";
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
        out << "PA(" ;
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
        out << "_L_";
      } else {
        out << "_";
      }

      if (rhs->_prev != nullptr) {
        out << "P(" ;
        for (int i = 0; i < rhs->_prev->_num_keys; i++) {
          if (i > 0)
            out << ", ";
          out << rhs->_prev->_keys[i];
        }
        out << ")";
      } else {
        out << "P(null)";
      }

      if (rhs->_next != nullptr) {
        out << "N(" ;
        for (int i = 0; i < rhs->_next->_num_keys; i++) {
          if (i > 0)
            out << ", ";
          out << rhs->_next->_keys[i];
        }
        out << ")";
      } else {
        out << "N(null)";
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

  BPlusTree() : BPlusTree(4) {} // 2-3-4树，一个节点最多3个子树

  BPlusTree(int degree) {
    std::cout << "BPlusTree::BPlusTree(int degree)\n";
    _max_degree = degree;
    _min_keys = (degree + 1) / 2 - 1; // Math.floor((newDegree + 1) / 2) - 1;
    _max_keys = degree - 1;
    _split_index = degree / 2; // Math.floor((newDegree - 1) / 2);
    _root = nullptr;
  }

  ~BPlusTree() {
    std::cout << "BPlusTree::~BPlusTree()\n";
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
  friend std::ostream& operator<<(std::ostream& out, const BPlusTree& rhs) {
    out << "Number of BPlusTree Nodes Used = " << Node::use_count << "\n";
    if (rhs._root == nullptr) {
      out << "Empty BPlusTree.\n";
      return out;
    }

    out << "max_degree = " << rhs._max_degree << "\n";
    out << "min_keys = " << rhs._min_keys << "\n";
    out << "max_keys = " << rhs._max_keys << "\n";
    out << "split_index = " << rhs._split_index << "\n";
    Print(out, rhs._root);

    // out << "RBPlusTree Height = " << height << "\n";

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
      //最终，新插入的元素一定是先插入到叶子节点
      // 如果是叶子节点上插入新的元素
      node->_num_keys++;
      //找到插入的位置(从小到大排列的，此处为顺序查找)
      int insertIndex = node->_num_keys - 1;
      while (insertIndex > 0 && node->_keys[insertIndex - 1] > element) {
        node->_keys[insertIndex] = node->_keys[insertIndex - 1];
        insertIndex--;
      }
      //插入
      node->_keys[insertIndex] = element;
      //修复性质
      InsertRepair(node);
    } else {
      //如果插入不是叶子节点，先找到合适的子树进行插入
      //先找到第一个大于等于待插入元素的已有元素
      int findIndex = 0;
      while (findIndex < node->_num_keys && node->_keys[findIndex] < element) {
        findIndex++;
      }
      //在子树上进行插入
      Insert(node->_children[findIndex], element);
    }
  }

  void InsertRepair(Node* node) {
    //叶子节点插入新元素后，需要修复性质
    if (node->_num_keys <= _max_keys) {
      //如果叶子节点个数还没满，不需要修复，返回
      return;
    } else if (node->_parent == nullptr) {
      //如果插入的是根节点，而且插入的元素个数太多了，对根节点进行分裂
      //这个大概就是上溢过程
      _root = Split(node);
      return;
    } else {
      //如果插入的节点不是根节点，对节点进行分裂
      Node* newNode = Split(node);
      InsertRepair(newNode);
    }
  }


  Node* Split(Node* node) {
    //node为待分裂的节点，此时，这个节点元素个数太多了
    std::cout << "Split\n";

    // 新建一个右节点
    Node* rightNode = new Node(_max_keys);
    //    Node::node_pointers.push_back(rightNode);
    //右节点的元素个数为从分裂索引开始右边的元素
    rightNode->_num_keys = node->_num_keys - _split_index - 1;
    //先保存分裂后需要上溢到父节点的元素，即为分裂索引对应的元素
    E risingElement = node->_keys[_split_index];

    int i;
    int parentIndex;
    if (node->_parent != nullptr) {
      //如果节点的父节点不为空(不是根节点)
      //记录当前的父节点
      Node* currentParent = node->_parent;
      //在父节点里的子树中查找，查找当前节点位于父节点的哪个子树
      for (parentIndex = 0;
           parentIndex < currentParent->_num_keys + 1
           && currentParent->_children[parentIndex] != node;
           parentIndex++);
      //如果发现没有当前的子树，抛出异常(这个会发生吗？)
      if (parentIndex == currentParent->_num_keys + 1) {
        throw "Could not find which child we were!";
      }
      //父节点中会多出一个节点(上溢)
      //先将父节点的子树和元素位置右移，为新元素腾出位置
      for (i = currentParent->_num_keys; i > parentIndex; i--) {
        currentParent->_children[i + 1] = currentParent->_children[i];
        currentParent->_keys[i] = currentParent->_keys[i - 1];
      }
      //父节点元素个数加1
      currentParent->_num_keys++;
      //父节点的合适位置插入上溢的元素
      currentParent->_keys[parentIndex] = risingElement;

      //父节点的子树中插入一个新的子节点
      currentParent->_children[parentIndex + 1] = rightNode;
      //设置新的子节点的父节点指向当前父节点
      rightNode->_parent = currentParent;
    }

    int rightSplit;
    if (node->_is_leaf) {
      //如果是叶子节点
      rightSplit = _split_index;
      rightNode->_next = node->_next;
      rightNode->_prev = node;
      if (node->_next != nullptr) {
        node->_next->_prev = rightNode;
      }
      node->_next = rightNode;
    } else {
      //如果不是叶子节点
      rightSplit = _split_index + 1;
    }

    //右节点的元素个数
    rightNode->_num_keys = node->_num_keys - rightSplit;

    //将node上的子节点移动到新节点上
    for (i = rightSplit; i < node->_num_keys + 1; i++) {
      rightNode->_children[i - rightSplit] = node->_children[i];
      //子节点的移动
      if (node->_children[i] != nullptr) {
        //如果包含了子节点，那么这个新增加的加点肯定不是叶子节点
        rightNode->_is_leaf = false;
        if (node->_children[i] != nullptr) {
          //设置子节点的新的父节点
          node->_children[i]->_parent = rightNode;
        }
        //将当前节点的对应子节点清空，因为已经移动到了新的节点上
        node->_children[i] = nullptr;
      }
    }
    //将node上的元素移动到新节点上
    for (i = rightSplit; i < node->_num_keys; i++) {
      rightNode->_keys[i - rightSplit] = node->_keys[i];
    }

    //此时node成为了左节点
    Node* leftNode = node;
    //设置左节点的元素个数
    leftNode->_num_keys = _split_index;

    if (node->_parent != nullptr) {
      //如果左节点的父节点不为空，直接返回父节点
      //这时父节点已经添加了一个元素，且添加了一个右子树，而原来的左子树则元素减少了，上溢了
      //继续上溢
      return node->_parent;
    } else {
      //当上溢到了根节点时，这时需要新建一个根节点，只有一个元素，其左右子树已经建立好了
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
    if (node->_is_leaf) {
      //不需要任何操作
    } else {
      // 父节点中的那个元素也添加到自己节点这里
      node->_keys[node->_num_keys] = parentNode->_keys[parentIndex];
    }

    for (int i = 0; i < rightSib->_num_keys; i++) {
      int insertIndex = node->_num_keys + 1 + i;
      if (node->_is_leaf)
        insertIndex -= 1;
      node->_keys[insertIndex] = rightSib->_keys[i];
    }

    //如果节点不是叶子节点，需要将有兄弟中的子树移动到自己这里，并且设置它们的父节点指向自己
    if (!node->_is_leaf) {
      for (int i = 0; i <= rightSib->_num_keys; i++) {
        node->_children[node->_num_keys + 1 + i] = rightSib->_children[i];
        node->_children[node->_num_keys + 1 + i]->_parent = node;
      }
      node->_num_keys = node->_num_keys + rightSib->_num_keys + 1;
    } else {
      //如果是叶子节点
      node->_num_keys = node->_num_keys + rightSib->_num_keys;
      node->_next = rightSib->_next;
      if (rightSib->_next != nullptr) {
        rightSib->_next->_prev = node;
      }
    }

    // 因为右兄弟已经合并了，父节点中要删除负责这个子树的children
    for (int i = parentIndex + 1; i < parentNode->_num_keys; i++) {
      parentNode->_children[i] = parentNode->_children[i + 1];
      parentNode->_keys[i - 1] = parentNode->_keys[i];
    }

    //父节点中的元素个数减1，因为右兄弟已经合并了
    parentNode->_num_keys--;
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

    if (node->_is_leaf) {
      node->_keys[0] = leftSib->_keys[leftSib->_num_keys - 1];
      parentNode->_keys[parentIndex - 1] = leftSib->_keys[leftSib->_num_keys - 1];
    } else {
      node->_keys[0] = parentNode->_keys[parentIndex - 1];
      parentNode->_keys[parentIndex - 1] = leftSib->_keys[leftSib->_num_keys - 1];
    }


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
    } else {
      //node是叶子节点

    }

    leftSib->_num_keys--;

    return node;
  }

  Node* BorrowFromRight(Node* node, int parentIndex) {
    //node是被删除了元素的节点,parentIndex是该节点在父节点中子树的索引
    Node* parentNode = node->_parent;
    //获取当前节点的右兄弟
    Node* rightSib = parentNode->_children[parentIndex + 1];
    //当前节点会从右兄弟中借一个元素来，因此元素个数加1
    node->_num_keys++;

    if (node->_is_leaf) {
      node->_keys[node->_num_keys - 1] = rightSib->_keys[0];
      parentNode->_keys[parentIndex] = rightSib->_keys[1];
    } else {
      //当前节点的最后一个元素为父节点中下来的那个元素
      node->_keys[node->_num_keys - 1] = parentNode->_keys[parentIndex];
      //父节点中的那个位置用右兄弟的第一个元素覆盖
      parentNode->_keys[parentIndex] = rightSib->_keys[0];
    }

    if (!node->_is_leaf) { //如果当前节点不是叶子节点,还需要移动子树
      //当前节点的子树为右兄弟的第一个子树
      node->_children[node->_num_keys] = rightSib->_children[0];
      //当前节点子树的父节点指向当前节点
      node->_children[node->_num_keys]->_parent = node;
      //因为右兄弟中移除了第一颗子树，因此需要将后面的子树前移
      for(int i = 1; i < rightSib->_num_keys + 1; i++) {
        rightSib->_children[i - 1] = rightSib->_children[i];
      }
    } else {
      // node是叶子节点
    }

    //因为右兄弟移除了一个元素(上去到了父节点中),因此需要将后面的元素前移
    for (int i = 1; i < rightSib->_num_keys; i++) {
      rightSib->_keys[i - 1] = rightSib->_keys[i];
    }
    //右兄弟的元素个数减1
    rightSib->_num_keys--;

    return node;
  }

  void Delete(Node* node, const E& element) {
    if (node == nullptr) return;
    // 顺序查找该节点下的元素
    int i;
    for (i = 0; i < node->_num_keys && node->_keys[i] < element; i++);
    // 开始删除
    if (i == node->_num_keys) {
      // 当前节点的元素都比删除元素小，进入最后一颗子树
      if (!node->_is_leaf) {
        Delete(node->_children[node->_num_keys], element);
      } else {
        // 如果是叶子节点，这种情况不可能
      }
    } else if (!node->_is_leaf && node->_keys[i] == element) {
      //如果不是叶子节点，进入对应的子树
      Delete(node->_children[i+1], element);
    } else if (!node->_is_leaf) {
      Delete(node->_children[i], element);
    } else if (node->_is_leaf && node->_keys[i] == element) {
      // 如果是叶子节点，找到了这个元素
      // 将后面的元素移动前移(没有children,因为不需要前移)
      for (int j = i; j < node->_num_keys - 1; j++) {
        node->_keys[j] = node->_keys[j + 1];
      }
      node->_num_keys--; // 当前节点元素个数减1

      //如果删除的正好是叶子节点的第一个元素，且不是根节点
      if (i == 0 && node->_parent != nullptr) {
        E nextSmallestElement; // 下一个最小的元素
        Node* parentNode = node->_parent; // 父节点
        int parentIndex;
        for (parentIndex = 0; parentNode->_children[parentIndex] != node; parentIndex++);
        //如果该节点删除之后没有元素了
        if (node->_num_keys == 0) {
          if (parentIndex == parentNode->_num_keys) {
            //在父节点的子节点中不存在当前node?不存在这种情况
          } else {
            //下一个最小元素为父节点的子节点树中的第一个元素(右兄弟的第一个元素)
            nextSmallestElement = parentNode->_children[parentIndex + 1]->_keys[0];
          }
        } else {
          //如果该节点元素删除之后还存在元素，则下一个最小元素就是当前节点的第一个元素
          nextSmallestElement = node->_keys[0];
        }

        // 此时，nextSmallestElement肯定存在一个值

        // 当父节点不为空
        while (parentNode != nullptr) {
          //如果父节点中存在当前元素，则将当前元素用找到的最小元素替代
          if (parentIndex > 0 && parentNode->_keys[parentIndex - 1] == element) {
            parentNode->_keys[parentIndex - 1] = nextSmallestElement;
          }
          //祖父节点
          Node* grandParentNode = parentNode->_parent;
          //找到祖父节点中父节点位置
          for (parentIndex = 0;
               grandParentNode != nullptr
               && grandParentNode->_children[parentIndex] != parentNode;
               parentIndex++);
          parentNode = grandParentNode;
        }
      }

      // 删除元素之后的修复
      RepairAfterDelete(node);
    }
  }

  void RepairAfterDelete(Node* node) {
    //node此时必然为叶子节点，而且已经删除了那个元素

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
    } else if (node->_parent != nullptr) {

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
#endif // BPLUSTREE_H
