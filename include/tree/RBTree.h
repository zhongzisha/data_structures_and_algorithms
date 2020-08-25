#ifndef RBTREE_H
#define RBTREE_H


#include <iostream>
#include "MyCommon.h"
#include "BinarySearchTree.h"

namespace test {

template <typename E>
class RBTree : public BinarySearchTree<E> {
protected:
  typedef typename BinarySearchTree<E>::Node Node;
  static constexpr bool RED = false;
  static constexpr bool BLACK = true;
public:
  /*
   * 1. 节点red或者black
   * 2. 根节点black
   * 3. 叶子结点都是black
   * 4. red节点的子节点都是black, 父节点都是black, 从根节点到叶子节点不能有两个连续的red节点
   * 5. 从任意节点到叶子节点的所有路径都包含相同数目的black节点
   */
  class RBNode : public Node {
  public:
    bool _color;
    RBNode(const E& element, Node* parent)
        : Node(element, parent), _color(RED) {
      // 默认的新创建的节点是red
    }

    friend std::ostream& operator<< (std::ostream& out, const RBNode* rhs) {
      if (rhs == nullptr) {
        out << "null";
        return out;
      }
      if (rhs->_left != nullptr) {
        out << "L" << rhs->_left->_element;
      } else {
        out << "null";
      }

      out << "_(" << rhs->_element << "_" << (rhs->_color ? "B" : "R") << ")";

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
    return new RBNode(element, parent);
  }

  Node* Color(Node* node, bool color) {
    if (node == nullptr) return node;
    static_cast<RBNode*>(node)->_color = color;
    return node;
  }

  Node* Red(Node* node) {
    return Color(node, RED);
  }

  Node* Black(Node* node) {
    return Color(node, BLACK);
  }

  bool ColorOf(Node* node) const {
    return node == nullptr ? BLACK : static_cast<RBNode*>(node)->_color;
  }

  bool IsBlack(Node* node) const {
    return ColorOf(node) == BLACK;
  }

  bool IsRed(Node* node) const {
    return ColorOf(node) == RED;
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

  }

  void AfterAdd(Node* node) {
    // 添加只有4中情况：(默认添加的是red)
    /* 转换为2-4数之后，添加的情况有以下情况：
     *
     *        红黑红
     * 红黑红、黑红、红黑、黑
     * 1. 红黑红
     * 2. 黑红
     * 3. 红黑
     * 4. 黑
     *
     * parent为black，有4种情况，不做任何处理
     *
     * parent为red，有8种情况，违背了性质4(不能有连续的两个red)
     * 1). node的uncle为black
     *   1. 添加的red节点node的parent为red，且符合LL/RR情况
     *   (1)染色。parent染黑，grand染红
     *   (2)对于LL, 右旋转grand
     *      对于RR, 左旋转grand
     *   2. 添加的red节点的parent为red，且符合LR/RL情况
     *   (1)染色。node自己染黑，grand染红
     *   (2)对于RL, 右旋转parent,左旋转grand
     *      对于LR, 左旋转parent,右旋转grand
     * 2). node的uncle为red
     *   1. LL情况。上溢，挑出中间的节点，向上合并
     *   (1)染色。parent染黑,uncle染黑
     *   (2)grand向上合并。grand染红，grand当做新添加的节点插入。
     *      可能继续发生上溢，持续到根节点时，只需将根节点染黑
     *   2. RR情况。上溢，挑出中间的节点，向上合并
     *   (1)染色。parent染黑、uncle染黑
     *   (2)grand向上合并。grand染红，grand当做新添加的节点插入。
     *      可能继续发生上溢，持续到根节点时，只需将根节点染黑
     *   3. LR情况和RL情况，都是
     *   (1)染色。parent染黑、uncle染黑
     *   (2)grand向上合并。grand染红，当做新添加节点继续向上处理，递归。
     */

    Node* parent = node->_parent;

    // 添加的是根节点
    if (parent == nullptr) {
      Black(node);
      return;
    }

    // 如果parent为black(4种情况)，直接返回
    if (IsBlack(parent))
      return;

    // 以下处理parent为red情况(8种情况)
    Node* uncle = parent->sibling(); // 叔父节点
    Node* grand = parent->_parent; // 祖父节点

    // 叔父节点是red (染色+递归)
    if (IsRed(uncle)) {
      Black(parent);
      Black(uncle);
      // 祖父节点当做新添加的节点，染红，递归
      AfterAdd(Red(grand));
      return;
    }

    // 叔父节点是black (染色+旋转)
    if (parent->IsLeftChild()) { // L
      if(node->IsLeftChild()) { // LL
        Black(parent);
        Red(grand);
        RotateRight(grand);
      } else { // LR
        Black(node);
        Red(grand);
        RotateLeft(parent);
        RotateRight(grand);
      }
    } else { //R
      if(node->IsLeftChild()) { // RL
        Black(node);
        Red(grand);
        RotateRight(parent);
        RotateLeft(grand);
      } else { // RR
        Black(parent);
        Red(grand);
        RotateLeft(grand);
      }
    }
  }

  void AfterRemove(Node* node, Node* replaceNode) {
    // replaceNode 用以取代node的子节点

    // B树中真正被删除的都是B树的叶子节点
    /* 转换为2-4数之后，添加的情况有以下情况：
     *
     *        红黑红
     * 红黑红、黑红、红黑、黑
     * 1. 红黑红
     * 2. 黑红
     * 3. 红黑
     * 4. 黑
     *
     * 1. 如果删除的是red叶子节点，直接删除，不用任何调整
     * 2. 如果删除的是black节点
     * (1) 有2个red的black节点(不存在这种情况，因为会去找子节点替代删除)，不用考虑这种情况
     * (2) 有1个red的black节点
     * (2.1) 如何判断这种情况：用以替代的子节点为red
     * (2.2) 将替代的子节点染黑，即可保持红黑树性质
     * (3) black叶子节点
     * ()
     */

    // 如果删除的是red节点，直接删除节点，返回
    if (IsRed(node)) {
      return;
    }

    // 如果用以替代的节点是red，直接将替代的子节点染黑，删除即可
    if (IsRed(replaceNode)) {
      Black(replaceNode);
      return;
    }

    // 删除的是根节点
    Node* parent = node->_parent;
    if (parent == nullptr) {
      return;
    }

    // 删除的是黑色叶子节点
    /*
     * (1)看黑sibling能不能借(必须要有red子节点)
     * 如果黑sibling至少有1个red子节点
     * 1. 进行旋转
     * 2. 旋转之后的中心节点继承parent颜色
     * 3. 旋转之后的左右节点染黑
     *
     * (2)如果黑sibling没有red子节点
     * 1. 父节点为red，下来合并。sibling染红,parent染黑
     * 2. 父节点为black，只需将parent当做被删除的节点处理
     *
     * (3)如果红sibling
     */

    // 这里是一个陷阱，因为AfterRemove之前已经修改了node的parent的left,right属性
    // Node* sibling = node->sibling();
    // 判断被删除的node是左还是右
    bool left = parent->_left == nullptr || node->IsLeftChild();
    Node* sibling = left ? parent->_right : parent->_left;
    if (left) { // 被删除的节点在左边，兄弟节点在右边
      if (IsRed(sibling)) { // 如果兄弟节点是红色
        Black(sibling);
        Red(parent);
        RotateLeft(parent);

        //更换兄弟
        sibling = parent->_right;
      }

      // 兄弟节点必然是黑色情况
      // 就看能不能借
      if (IsBlack(sibling->_left) && IsBlack(sibling->_right)) {
        // sibling没有red子节点
        // 父节点向下和兄弟节点合并
        bool parentBlack = IsBlack(parent);
        Black(parent);
        Red(sibling);
        if (parentBlack) {
          AfterRemove(parent, nullptr);
        }
      } else {
        // sibling至少有一个red子节点
        // 向兄弟节点借元素
        // 红在左，红在右，左右都红

        // 如果兄弟节点的左边是黑色，兄弟要先左旋转
        if (IsBlack(sibling->_right)) {
          RotateRight(sibling);
          sibling = parent->_right;
        }

        Color(sibling, ColorOf(parent));
        Black(sibling->_right);
        Black(parent);

        RotateLeft(parent);

      }
    } else { // 被删除的节点在右边，兄弟节点在左边
      if (IsRed(sibling)) { // 如果兄弟节点是红色
        Black(sibling);
        Red(parent);
        RotateRight(parent);

        //更换兄弟
        sibling = parent->_left;
      }

      // 兄弟节点必然是黑色情况
      // 就看能不能借
      if (IsBlack(sibling->_left) && IsBlack(sibling->_right)) {
        // sibling没有red子节点
        // 父节点向下和兄弟节点合并
        bool parentBlack = IsBlack(parent);
        Black(parent);
        Red(sibling);
        if (parentBlack) {
          AfterRemove(parent, nullptr);
        }
      } else {
        // sibling至少有一个red子节点
        // 向兄弟节点借元素
        // 红在左，红在右，左右都红

        // 如果兄弟节点的左边是黑色，兄弟要先左旋转
        if (IsBlack(sibling->_left)) {
          RotateLeft(sibling);
          sibling = parent->_left;
        }

        Color(sibling, ColorOf(parent));
        Black(sibling->_left);
        Black(parent);

        RotateRight(parent);

      }
    }
  }

  bool IsRBTree() const {
    if (this->_root == nullptr) return true;
    if (IsRed(this->_root)) return false;

    int currentBlackNum = 0;
    int blackNum = 0;
    Node* node = this->_root;
    while (node != nullptr) {
      if (ColorOf(node) == BLACK)
        blackNum++;
      node = node->_left;
    }

    return _IsRBTree(this->_root, blackNum, currentBlackNum);
  }

  bool _IsRBTree(Node* node, int blackNum, int currentBlackNum) const {

    if (node == nullptr)
      return true;

    if (ColorOf(node) == RED && ColorOf(node->_parent) == RED)
      return false;

    if (ColorOf(node) == BLACK)
      currentBlackNum++;

    if (node->_left == nullptr && node->_right == nullptr) {
      if (currentBlackNum == blackNum)
        return true;
      else
        return false;
    }

    return _IsRBTree(node->_left, blackNum, currentBlackNum) &&
           _IsRBTree(node->_right, blackNum, currentBlackNum);
  }


  // 层序遍历输出二叉树
  friend std::ostream& operator<<(std::ostream& out, const RBTree& rhs) {
    out << "Number of RBTree Nodes Used = " << Node::use_count << "\n";
    if (rhs._root == nullptr) {
      out << "Empty RBTree.\n";
      return out;
    }
    std::queue<Node*> q;
    q.push(rhs._root);
    int levelSize = 1;
    int height = 0;
    while (!q.empty()) {
      Node* node = q.front();
      out << static_cast<RBNode*>(node) << " ";
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
    out << "RBTree Height = " << height << "\n";

    return out;
  }

};


}

#endif // RBTREE_H
