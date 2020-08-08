#ifndef SPLAYTREE_H
#define SPLAYTREE_H


#include <iostream>
#include <queue>
#include "MyCommon.h"

namespace test {

template <typename E>
class SplayTree {
public:
  class Node {
  public:
    E _element;
    int _size;  // 子树中的节点个数
    int _duplicate_count; // 相同的element重复多少次
    Node* _left;
    Node* _right;
    Node* _parent;
    static int used_count;

    Node(const E& element, Node* parent) : _element(element),
                                           _size(1),
                                           _duplicate_count(1),
                                           _left(nullptr),
                                           _right(nullptr),
                                           _parent(parent) {
      used_count++;
    }
    ~Node() {
      used_count--;
      std::cout << "invode Node::~Node() " << _element
                << ", remain used_count = " << used_count
                << "\n";
    }

    bool IsLeaf() const {
      return _left == nullptr && _right == nullptr;
    }

    bool HasTwoChildren() const {
      return _left != nullptr && _right != nullptr;
    }

    bool IsLeftChild() const {
      if (_parent == nullptr) return true; // 这里跟原始的BST里面的定义不同
      return _parent != nullptr && this == _parent->_left;
    }

    bool IsRightChild() const {
      if (_parent == nullptr) return true; // 这里跟原始的BST里面的定义不同
      return _parent != nullptr && this == _parent->_right;
    }

    void UpdateSize() {
      int leftSize = _left == nullptr ? 0 : _left->_size;
      int rightSize = _right == nullptr ? 0 : _right->_size;
      _size = _duplicate_count + leftSize + rightSize;
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

      out << "_(" << rhs->_element  // the element
          << "_S" << rhs->_size     // the count of nodes in subtrees
          << "_C" << rhs->_duplicate_count    // the count of the replicated element
          << ")";

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
  SplayTree() : _size(0), _root(nullptr) {
    std::cout << "SplayTree::SplayTree()\n";
  }
  virtual ~SplayTree() {
    std::cout << "SplayTree::~SplayTree()\n";
    Clear();
  }

  Node* Root() {
    return _root;
  }

  void SetRoot(Node* newRoot) {
    _root = newRoot;
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
      if (node != nullptr) {
        delete node;
        node = nullptr;
      }
    }
    _size = 0;
    _root = nullptr;
  }

  bool Contains(const E& element) {
    return Find(_root, element) != nullptr;
  }

  void Add(const E& element) {

    std::cout << "Add " << element << "\n";

    if (_root == nullptr) { // 如果添加的是第一个节点，则创建根节点
      _root = CreateNode(element, nullptr);
      _size++;
      return;
    } else {
      Node* node = _root;
      Node* parent = nullptr; // 记录父节点
      int cmp;
      while (node != nullptr) {
        cmp = Compare(node->_element, element);
        if (cmp == 0) { // element存在
          node->_duplicate_count++;
          node->_size++;
          break;
        } else {  // element 不存在
          if (cmp > 0) { // 进入左子树插入
            parent = node;
            node = node->_left;
          } else { // (cmp < 0) // 进入右子树插入
            parent = node;
            node = node->_right;
          }
        }
      }
      if (node == nullptr) {
        if (cmp > 0) {
          node = parent->_left = CreateNode(element, parent); // 插入到父节点的左边
        } else { // (cmp < 0)
          node = parent->_right = CreateNode(element, parent); // 插入到父节点的右边
        }
      }
      SplayUp(node);  // node为新创建的节点或者已经存在的节点,将其移动到根节点
      UpdateSize(node);
      _size++;
    }
  }

  // 给定一个元素，返回其排名，排名从1开始
  int GetRank_V1(const E& element) {
    if (_root == nullptr) return -1;

    Node* node = Find(_root, element); //首先找到该节点，该节点成为了根节点
    if (node == nullptr) {
      std::cout << "No this element! No rank.\n";
      return -1;
    }

    if (node->_left != nullptr)
      return node->_left->_size + 1;
    else
      return node->_duplicate_count;

  }

  // 给定一个元素，返回其排名(因为可能存在重复元素，因此返回排名上下界)，排名从1开始
  std::pair<int, int> GetRank(const E& element) {
    std::pair<int, int> result(-1, -1);

    if (_root == nullptr) return result;

    Node* node = Find(_root, element); //首先找到该节点，该节点成为了根节点
    if (node == nullptr) {
      std::cout << "No this element! No rank.\n";
      return result;
    }

    result.first = (node->_left == nullptr) ? 1 : node->_left->_size + 1;
    result.second = (node->_left == nullptr) ? node->_duplicate_count : node->_left->_size + node->_duplicate_count;
    return result;
  }

  E GetKth_V1(int k) {
    if (_root == nullptr) {
      throw "Empty Tree.";
    }
    k = k < 1 ? 1 : (k > _size) ? _size : k;

    Node* node = _root;
    while (node != nullptr) {
      if (node->_left != nullptr) {
        if (k >= (node->_left->_size + 1)
            && k <= (node->_left->_size + node->_duplicate_count)) {
          return node->_element;
        } else if ( k > (node->_left->_size + node->_duplicate_count)) {
          k -= (node->_left->_size + node->_duplicate_count);
          node = node->_right;
        } else {
          node = node->_left;
        }
      } else if (node->_left == nullptr) {
        if (k >= 1 && k <= node->_duplicate_count) {
          return node->_element;
        } else if (k > node->_duplicate_count) {
          k -= 1;
          node = node->_right;
        }
      } else {
        node = node->_left;
      }
    }
  }

  // 找到排名为k的元素，k从1开始，越界则返回一个合理的值
  E GetKth(int k) {
    if (_root == nullptr) {
      throw "Empty Tree.";
    }
    //控制k的范围
    k = k < 1 ? 1 : (k > _size) ? _size : k;

    Node* node = _root;
    int leftCount;
    while (node != nullptr) {
      // 左子树节点个数
      leftCount = node->_left == nullptr ? 0 : node->_left->_size;
      if (k >= (leftCount + 1)
          && k <= (leftCount + node->_duplicate_count)) {
        //左子树节点个数+1表示当前节点的最小排名
        //左子树节点个数+当前节点元素个数为当前节点最大排名
        //如果k介于之间，表示第k个元素就在当前节点内，返回节点元素
        return node->_element;
      } else if ( k > (leftCount + node->_duplicate_count)) {
        //如果排名要比最大排名还要大，则进入右子树继续查找，但是k需要减去左边那些元素
        k -= (leftCount + node->_duplicate_count);
        node = node->_right;
      } else {
        //如果排名要比最小排名还要小，则需要进入左子树继续查找
        node = node->_left;
      }
    }
  }

  void Remove(const E& element) {
    if (_root == nullptr) return;

    Node* node = Find(_root, element); //首先找到该节点，该节点成为了根节点
    if (node == nullptr) {
      std::cout << "No this element!\n";
      return;
    }

    if (node->_duplicate_count > 1) {
      node->_duplicate_count--;
      UpdateSize(node);
      _size--;
      return;
    }

    if(_root->_right == nullptr) { //如果根节点右子树为空，根节点指向左子树，这样就删除了那个元素
      _root = _root->_left;
      if (_root != nullptr)  //如果左子树不为空，则需要将父节点置为空，因为成为根节点了
        _root->_parent = nullptr;
    } else if (_root->_left == nullptr) { //如果左子树为空，根节点指向右子树
      _root = _root->_right;
      if (_root != nullptr) //如果右子树不为空，则需要将父节点置为空，因为成为根节点了
        _root->_parent = nullptr;
    } else {  //到这里，表示要删除节点的左右子树都不为空
      Node* right = _root->_right; // 右子树
      Node* left = _root->_left; // 左子树

      left->_parent = nullptr;  //
      Node* largestLeft = FindMaxNode(left); //找到左子树里面最大的元素
      SplayUp(largestLeft); //将最大的元素splay到根节点，右子树肯定为空

      largestLeft->_parent = nullptr; //根节点父节点为空
      largestLeft->_right = right; //根节点的右子树为原来的右子树
      right->_parent = largestLeft; //右子树的父节点设置
      _root = largestLeft; //设置根节点
    }

    if (_root != nullptr)
      UpdateSize(_root);

    _size--;
    delete node; // 删除那个节点, 删除那个节点
    node = nullptr;
  }

  // 合并另一个splay tree, tree2的元素必须都大于当前树
  SplayTree* MergeAnotherTree(SplayTree* tree2) {
    if (_root == nullptr) return tree2;
    if (tree2->Root() == nullptr) return this;
    Node* maxNode = FindMaxNode(_root);
    SplayUp(maxNode);
    maxNode->_right = tree2->Root();
    tree2->Root()->_parent = maxNode;
    _root = maxNode;
    return this;
  }

  // 分裂一颗树，根据一个给定的值，小于它的组成一棵树，大于它的组成一棵树
  std::vector<SplayTree<E>*> SplitToTwoTrees(const E& element) {
    std::vector<SplayTree<E>*> result(2, nullptr);
    if (_root == nullptr) return result;

    // 先找到小于等于element的那个节点
    Node* node = _root;
    Node* parent = node;
    int cmp;
    while (node != nullptr) {
      parent = node;
      cmp = Compare(node->_element, element);
      if (cmp > 0) { // 如果节点元素大于element，往左子树里面找
        node = node->_left;
      } else {  // 如果节点元素小于等于element, 往右子树里面找
        node = node->_right;
      }
    }
    node = parent; // node为小于等于element的节点
    SplayUp(node); // 将这个节点置为根节点
    Node* root = _root;

    result[0] = new SplayTree<E>();
    result[1] = new SplayTree<E>();
    result[0]->SetRoot(root);
    result[1]->SetRoot(root->_right);
    if (root->_right != nullptr) {
      root->_right->_parent = nullptr;
    }
    root->_right = nullptr;
    return result;
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

  // 层序遍历输出二叉树
  friend std::ostream& operator<<(std::ostream& out, const SplayTree& rhs) {
    out << "Number of Tree Nodes Used = " << Node::used_count << "\n";
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
    out << "Tree Size = " << rhs._size << "\n";

    return out;
  }

  // 层序遍历输出二叉树
  friend std::ostream& operator<<(std::ostream& out, const SplayTree* rhs) {
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


  //区间查询,找出在[e1, e2]之间的元素
  //
  void RangeFind(const E& e1, const E& e2) {
    if (e1 > e2) return;
    // 先找到小于等于element的那个节点
    Node* e1_node = Find(_root, e1); // 先查找e1
    Node* node = nullptr;
    if (_root != nullptr) {
      if (_root->_element < e1) {
        // 如果没找到e1,且root比e1小,那么root就是第一个小于e1的元素
        node = _root;
      } else {
        //1. 如果找到了e1,e1节点成为root,那么其左节点为第一个小于e1的元素
        //2. 如果没有找到e1,且root比e1大,那么其左节点为第一个小于e1的元素
        node = _root->_left;
      }
    }
    SplayUp(node); //将目标节点splay至根节点,右子树都>=e1

    Node* oldRoot = _root; //此时根节点是第一个小于e1的节点

    // 先断开右子树
    _root = _root->_right;
    if (_root != nullptr) //断开parent
      _root->_parent = nullptr;

    //找到小于等于e2的节点
    Node* e2_node = Find(_root, e2);
    if (_root != nullptr) {
      if (_root->_element <= e2) {
        // 1. 如果找到了e2,e2成为了root,那么其右节点为第一个大于e2的元素
        // 2. 如果没找到e2,且root比e2小,那么其右节点为第一个大于e2的元素
        node = _root->_right;
      } else {
        // 如果没找到e2,且root比e2大,那么root就是第一个比e2大的元素
        node = _root;
      }
    }
    SplayUp(node); // 将目标节点splay至根节点,左子树都<=e2

    // 将右子树重新连接
    oldRoot->_right = _root;
    if (_root != nullptr)
      _root->_parent = oldRoot;

    // 设置root
    _root = oldRoot;
    if (_root != nullptr)
      _root->_parent = nullptr;

    std::cout << "Range is [[[\n";
    // 当e1比所有元素都小时,root也包含在范围内
    if (_root != nullptr && _root->_element >= e1) {
      std::cout << _root << "\n";
    }
    if (_root != nullptr && _root->_right != nullptr) {
      InOrder(_root->_right->_left);
      //当e2比所有元素都大时,root右节点也包含在范围内
      std::cout << _root->_right << "\n";
    }
    std::cout << "]]]\n";
  }

  void InOrder() {
    std::cout << "InOrder: [[[\n";
    InOrder(_root);
    std::cout << "]]]\n";
  }

protected:

  // 中序遍历
  void InOrder(Node* node) {
    if (node == nullptr) return;

    InOrder(node->_left);
    std::cout << node << "\n";
    InOrder(node->_right);
  }

  //这里可以考虑直接传Comparator
  int Compare(const E& e1, const E& e2) {
    return e1 - e2;
  }

  // 将node旋转到其父节点是targetNode停止
  // targetNode==nullptr时，表示将node旋转至root
  // targetNode!=nullptr时，表示将node旋转至父节点为targetNode时停止
  void Splay(Node* node, Node* targetNode) {
    if (node == nullptr) return;

    Node* child = node;
    while(child->_parent != targetNode) {
      Node* parent = child->_parent;
      if (parent == nullptr) { // child已经是根节点了
        return;
      } else { // child 不是根节点
        Node* grand = parent->_parent; // grand
        if (grand == targetNode) { //grand是目标节点，那么只需旋转一次父节点即可满足要求
          if (child->IsLeftChild()) {
            RotateRight(parent); //如果child是左节点，那么沿着parent右旋一次即可
          } else {
            RotateLeft(parent);//如果child是右节点，那么沿着parent左旋一次即可
          }
        } else if (parent->IsLeftChild()) { // L
          if (child->IsLeftChild()) { //LL zig-zig
            RotateRight(grand);
            RotateRight(parent);
          } else { //LR  zig-zag
            RotateLeft(parent);
            RotateRight(grand);
          }
        } else { //R
          if (child->IsRightChild()) { // RR zag-zag
            RotateLeft(grand);
            RotateLeft(parent);
          } else { // RL  zag-zig
            RotateRight(parent);
            RotateLeft(grand);
          }
        }
      }
    }
  }

  void SplayUp(Node* node) {
    if (node == nullptr) return;  //node为空，不需要splay

    Node* child = node;
    Node* parent = child->_parent;

    if (parent == nullptr) return; // node为根节点，也不需要splay

    //来到这里, child不为空, parent不为空
    Node* grand = parent->_parent;

    if (grand == nullptr) {  // node是根节点的左右儿子
      if (child->IsLeftChild()) {
        RotateRight(parent); // node为根节点左二子，沿着parent(也就是根节点)右旋转
      } else {
        RotateLeft(parent); // node为根节点右二子，沿着parent(也就是根节点)左旋转
      }
    } else { // //来到这里, child不为空, parent不为空, grand不为空. (node在第3层及以下层)
      if (parent->IsLeftChild()) { // L
        if (child->IsLeftChild()) { //LL  zigzig  一字型旋转(转化为两次同方向的单旋转)
          RotateRight(grand);  //single rotate单旋转
          RotateRight(parent);
        } else { // LR    zig-zag
          RotateLeft(parent);
          RotateRight(grand);
        }
      } else { //R
        if (child->IsLeftChild()) { // RL zag-zig
          RotateRight(parent);  //single rotate单旋转
          RotateLeft(grand);
        } else { // RR  zagzag  一字型旋转(转化为两次同方向的单旋转)
          RotateLeft(grand);
          RotateLeft(parent);
        }
      }

      SplayUp(child); // 递归进行splay,让node成为根节点
    }
  }

  Node* Find(Node* node, const E& element) {
    if (node == nullptr) return nullptr;
    int cmp = Compare(node->_element, element);
    if (cmp == 0) {
      SplayUp(node); //如果找到了节点，将那个节点移动到根节点
      return node;
    } else if (cmp < 0) { //如果节点元素小于给定元素,则进入右子树查找
      if (node->_right == nullptr) { //如果节点右子树为空
        //则对当前节点伸展操作，移动到根节点(这一步操作是为了将于查询元素最接近的节点移动到根节点)
        SplayUp(node);
        return nullptr; //返回空节点(没找到该节点)
      } else {  // 如果节点右子树不为空，则进入右子树继续查找
        return Find(node->_right, element);
      }
    } else if (cmp > 0) {
      if (node->_left == nullptr) {
        SplayUp(node);
        return nullptr;
      } else {
        return Find(node->_left, element);
      }
    }
    return nullptr;
  }

  //找到node右子树的最大值的节点
  Node* FindMaxNode(Node* node) {
    if (node->_right == nullptr) { //如果节点右子树为空，直接返回当前节点
      return node;
    } else { //如果节点右子树不为空，持续进入右子树
      while (node->_right != nullptr) {
        node = node->_right;
      }
      return node; // 找到了那个节点
    }
  }

  Node* FindMinNode(Node* node) {
    if (node->_left == nullptr) {
      return node;
    } else {
      while (node->_left != nullptr) {
        node = node->_left;
      }
      return node;
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
    if (grand->_parent == nullptr) {
       _root = parent;
    } else if(grand->IsLeftChild()) {
      grand->_parent->_left = parent;
    } else if (grand->IsRightChild()) {
      grand->_parent->_right = parent;
    }

    if (child != nullptr) {
      child->_parent = grand;
    }

    grand->_parent = parent;

    UpdateSize(grand);
    UpdateSize(parent);
  }

  void UpdateSize(Node* node) {
    node->UpdateSize();
  }

  // 递归删除一棵树的所有节点
  void DeleteTreeRecursively(Node* root) {
    if (root == nullptr) return;
    DeleteTreeRecursively(root->_left);
    Node* right = root->_right;
    delete root; root = nullptr;
    DeleteTreeRecursively(right);
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

#endif // SPLAYTREE_H
