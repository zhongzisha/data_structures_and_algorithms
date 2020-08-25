#ifndef INTERVALTREEV2_H
#define INTERVALTREEV2_H

/*

区间树，点查询，重叠区间查询

Red-Black-Tree based Interval-Tree, most code from the RBTree.h
基于红黑树实现的区间树，支持区间插入、删除、区间查询

References:
1. https://github.com/chaimleib/intervaltree
2. https://github.com/charcuterie/interval-tree/blob/master/src/datastructures/IntervalTree.java
3. https://www.coursera.org/lecture/algorithms-part1/interval-search-trees-ot9vw
*/

#include <iostream>
#include <queue>
#include <optional>
#include <vector>

#include "MyCommon.h"

namespace test {

template <typename K, typename V> // K need comparable, has operator -
class IntervalTreeV2
{
public:

    struct Interval { // [_start, _end), close-open interval
        K _start;
        K _end;

        Interval(const K& start, const K& end)
            : _start(start),
              _end(end) {

        }

        friend std::ostream& operator<<(std::ostream& out, Interval *interval) {
            out << "[" << interval->_start << ", " << interval->_end << ")";
            return out;
        }
        friend std::ostream& operator<<(std::ostream& out, Interval& interval) {
            out << "[" << interval._start << ", " << interval._end << ")";
            return out;
        }
        friend std::ostream& operator<<(std::ostream& out, const Interval *interval) {
            out << "[" << interval->_start << ", " << interval->_end << ")";
            return out;
        }
        friend std::ostream& operator<<(std::ostream& out, const Interval& interval) {
            out << "[" << interval._start << ", " << interval._end << ")";
            return out;
        }
    };

private:
    static const bool RED = false;
    static const bool BLACK = true;

    class Node {
    public:
        static int used_count;
        Interval _interval;
        V _value;
        Node* _left;
        Node* _right;
        Node* _parent;
        bool _color;   // for RBTree
        K _maxEnd;   // maximum endpoint

        Node(const Interval& interval, const V& value, Node* parent)
            : _interval(interval),
              _value(value),
              _left(nullptr),
              _right(nullptr),
              _parent(parent),
              _color(RED),
              _maxEnd(interval._end) {
            used_count++;
            std::cout << "Node(), used_count = " << used_count << "\n";
        }
        ~Node() {
            used_count--;
            std::cout << "~Node(), used_count = " << used_count << "\n";
        }

        bool IsLeaf() const {
            return _left == nullptr && _right == nullptr;
        }

        bool HasTwoChildren() const {
            return _left != nullptr && _right != nullptr;
        }

        bool IsLeftChild() const {
            return _parent != nullptr && this == _parent->_left;
        }

        bool IsRightChild() const {
            return _parent != nullptr && this == _parent->_right;
        }

        void UpdateMaxEnd() {
            K maxEnd = _interval._end;
            if (_left != nullptr && maxEnd < _left->_maxEnd) {
                maxEnd = _left->_maxEnd;
            }
            if (_right != nullptr && maxEnd < _right->_maxEnd) {
                maxEnd = _right->_maxEnd;
            }
            _maxEnd = maxEnd;
        }

        bool operator==(const Node& rhs) {
            return CompareInterval(_interval, rhs._interval) == 0 &&
                    _left == rhs._left &&
                    _right == rhs._right &&
                    _parent == rhs._parent;
        }

        bool operator!=(const Node& rhs) {
            return !operator==(rhs);
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
                out << "L" << rhs->_left->_interval;
            } else {
                out << "null";
            }

            out << "_(" << rhs->_interval << "_"
                << (rhs->_color ? "B" : "R")
                << "_" << rhs->_maxEnd
                << ")";

            if (rhs->_parent != nullptr) {
                out << "P" << rhs->_parent->_interval;
            }

            if (rhs->_right != nullptr) {
                out << "_R" << rhs->_right->_interval;
            } else {
                out << "_null";
            }

            return out;
        }
    };

public:
    IntervalTreeV2() : _root(nullptr), _size(0) {
        std::cout << "IntervalTreeV2()\n";
    }

    virtual ~IntervalTreeV2() {
        Clear();
        std::cout << "~IntervalTreeV2()\n";
    }

    void Add(const Interval& interval, const V& value) {

        if (_root == nullptr) { // 如果添加的是第一个节点，则创建根节点
            _root = CreateNode(interval, value, nullptr);
            _size++;

            AfterAdd(_root);
            return;
        }

        //如果不是第一个节点，则沿着树进行比较，左边小，右边大
        Node* parent = nullptr;
        Node* node = _root;
        int cmp = 0;
        K maxEnd = interval._end;
        while (node != nullptr) {
            if (node->_maxEnd < maxEnd) { // update the maxEnd
                node->_maxEnd = maxEnd;
            }
            cmp = CompareInterval(interval, node->_interval);
            parent = node;
            if (cmp > 0) {
                node = node->_right;
            } else if (cmp < 0) {
                node = node->_left;
            } else {
                std::cout << "The inverval exists, just override the value.\n";
                node->_value = value;
                return;
            }
        }

        // 这时，需要创建一个新节点，看是插入到父节点的左边还是右边
        Node* newNode = CreateNode(interval, value, parent);
        if (cmp > 0) {
            parent->_right = newNode;
        } else {
            parent->_left = newNode;
        }
        _size++;

        // 新添加节点的处理
        AfterAdd(newNode);
    }


    void Remove(const Interval& interval) {
        Node* elementNode = GetNode(interval);
        //RemoveBySuccessor(elementNode);
        RemoveByPredecessor(elementNode);
    }

    std::optional<Interval> GetOverlapInterval(const Interval& interval) {
        if (_root == nullptr) return std::nullopt;
        Node *node = OverlappingNode(_root, interval);
        if (node == nullptr) {
            return std::nullopt;
        } else {
            return node->_interval;
        }
    }

    std::optional<Interval> GetMininumOverlapInterval(const Interval& interval) {
        if (_root == nullptr) return std::nullopt;
        Node *node = MininumOverlappingNode(_root, interval);
        if (node == nullptr) {
            return std::nullopt;
        } else {
            return node->_interval;
        }
    }

    std::optional<std::vector<Interval>> GetAllOverlappingInterval(const Interval& interval) {
        if (_root == nullptr) return std::nullopt;
        std::vector<Interval> results;
        Node *next = MininumOverlappingNode(_root, interval);
        if (next == nullptr) {
            return std::nullopt;
        } else {
            results.push_back(next->_interval);

            while (next != nullptr) {
                next = NextOverlappingNode(next, interval);
                if (next != nullptr) {
                    results.push_back(next->_interval);
                }
            }
        }
        return results;
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

    // 层序遍历输出二叉树
    friend std::ostream& operator<<(std::ostream& out, const IntervalTreeV2& rhs) {
        out << "Number of RBTree Nodes Used = " << Node::used_count << "\n";
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
        out << "RBTree Height = " << height << "\n";

        return out;
    }

    // 层序遍历输出二叉树
    friend std::ostream& operator<<(std::ostream& out, const IntervalTreeV2* rhs) {
        out << "Number of RBTree Nodes Used = " << Node::used_count << "\n";
        if (rhs->_root == nullptr) {
            out << "Empty RBTree.\n";
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
        out << "RBTree Height = " << height << "\n";
        out << "IsRBTree = " << rhs->IsRBTree() << "\n";

        return out;
    }


private:
    Node *_root;  // 根节点
    int _size;    // 树中节点个数

    // 查询给定区间对应的节点
    Node* SearchNode(const Interval& interval) {
        if (_root == nullptr) return nullptr;

        Node *node = _root;
        int cmp = -2;
        while (node != nullptr
               && (cmp = CompareInterval(interval, node->_interval)) != 0) {
            node = cmp == -1 ? node->_left : node->_right;
        }
        return node;
    }

    // 查询当前节点下面的最小区间节点
    Node* MinimumNode(Node *node) {
        if (_root == nullptr || node == nullptr) return nullptr;

        while (node->_left != nullptr) {
            node = node->_left;
        }
        return node;
    }

    // 查询当前节点下面的最大区间节点
    Node* MaximumNode(Node *node) {
        if (_root == nullptr || node == nullptr) return nullptr;

        while (node->_right != nullptr) {
            node = node->_right;
        }
        return node;
    }

    // 找一个与给定区间重叠的节点，任意节点
    Node* OverlappingNode(Node *node, const Interval& interval) {
        if (_root == nullptr || node == nullptr) return nullptr;

        while (node != nullptr && !IsIntervalOverlap(interval, node->_interval)) {
            node = (node->_left != nullptr
                    && node->_left->_maxEnd > interval._start) ?
                        node->_left : node->_right;
        }
        return node;
    }

    // 找最小区间重叠的节点
    Node* MininumOverlappingNode(Node* node, const Interval& interval) {
        if (_root == nullptr || node == nullptr) return nullptr;

        Node *result = nullptr;
        if (node->_maxEnd > interval._start) { // 这种情况才需要向下找，否则没必要找了
            while (true) {
                if (IsIntervalOverlap(node->_interval, interval)) {
                    // 当前节点与给定区间重叠，那么其左子树可能存在一个更小重叠的区间
                    result = node; //已经找到了一个重叠的区间
                    node = node->_left; //进入其左子树，因为左边可能还存在重叠区间
                    if (node == nullptr || node->_maxEnd <= interval._start)  {
                        // 如果左子树为空或者节点的最大右端点比区间起始值还小，则表明左子树里面肯定不存在重叠区间了
                        break;
                    }
                } else {
                    // 没有重叠，检查左子树是否存在重叠
                    Node *left = node->_left;
                    if (left != nullptr && left->_maxEnd > interval._start) {
                        node = left; // 左子树可能包含重叠，继续左子树查找
                    } else {
                        if (node->_interval._start >= interval._end) {
                            break;
                        }

                        // 左子树一定不包含重叠，进入右子树查找
                        node = node->_right;
                        if (node == nullptr || node->_maxEnd <= interval._start) {
                            break;
                        }
                    }
                }
            }
        }
        return result;
    }

    Node* NextOverlappingNode(Node *node, const Interval& interval) {
        if (_root == nullptr || node == nullptr) return nullptr;

        Node *right = nullptr;
        if (node->_right != nullptr) {
            right = MininumOverlappingNode(node->_right, interval);
        }

        while (node->_parent != nullptr && right == nullptr) {
            if (node->IsLeftChild()) {
                right = (IsIntervalOverlap(node->_parent->_interval, interval)) ?
                            node->_parent :
                            MininumOverlappingNode(node->_parent->_right, interval);
            }
            node = node->_parent;
        }
        return right;
    }

    Node* CreateNode(const Interval& interval, const V& value, Node* parent) {
        return new Node(interval, value, parent);
    }

    Node* Color(Node* node, bool color) {
        if (node == nullptr) return node;
        node->_color = color;
        return node;
    }

    Node* Red(Node* node) {
        return Color(node, RED);
    }

    Node* Black(Node* node) {
        return Color(node, BLACK);
    }

    bool ColorOf(Node* node) const {
        return node == nullptr ? BLACK : node->_color;
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

        // 更新maxEnd
        UpdateMaxEnd(grand);
        UpdateMaxEnd(parent);

    }

    void AfterAdd(Node* node) {
        // 添加只有4中情况：(默认添加的是red)
        /* 转换为2-4树之后，添加的情况有以下情况：
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
            delete node;
        }
        _size = 0;
        _root = nullptr;
    }

    // 比较两个区间
    int CompareInterval(const Interval& i1, const Interval& i2) {
        if (i1._start > i2._start) {
            return 1;
        } else if (i1._start < i2._start) {
            return -1;
        } else if (i1._end > i2._end) {
            return 1;
        } else if (i1._end < i2._end) {
            return -1;
        } else {
            return 0;
        }
    }

    // 两个区间是否重叠
    bool IsIntervalOverlap(const Interval& i1, const Interval& i2) {
        return i1._end > i2._start && i2._end > i1._start;
    }

    void RemoveBySuccessor(Node* node) {
        if (node == nullptr) return;

        _size--;

        // 删除的是度为2的节点
        if (node->HasTwoChildren()) {
            Node* s = Successor(node); // 找到后继节点
            node->_interval = s->_interval; // 将后继节点的值覆盖需要删除的节点
            node->_value = s->_value;
            node = s; // 删除那个后继节点(后继节点肯定是度为0或者度为1)
        }

        // 删除度为0或1的节点node
        Node* replaceNode = (node->_left == nullptr) ? node->_right : node->_left;
        if (replaceNode != nullptr) { // 度为1的节点
            replaceNode->_parent = node->_parent;
            if (node->_parent == nullptr) { // 度为1，且是根节点
                _root = replaceNode;
            } else if (node == node->_parent->_left) {
                node->_parent->_left = replaceNode;
            } else if( node == node->_parent->_right) {
                node->_parent->_right = replaceNode;
            }

            // 删除节点后进行处理
            AfterRemove(node, replaceNode);
        } else if (node->_parent == nullptr) { // 度为0的节点，根节点
            _root = nullptr;

            AfterRemove(node, nullptr);
        } else { // 度为0的节点，叶子节点
            if (node == node->_parent->_left) {
                node->_parent->_left = nullptr;
            } else {
                node->_parent->_right = nullptr;
            }

            AfterRemove(node, nullptr);

            // 从父节点向上更新maxEnd
            Node* parent = node->_parent;
            while (parent != nullptr) {
                UpdateMaxEnd(parent);
                parent = parent->_parent;
            }
        }

        delete node;
    }


    void RemoveByPredecessor(Node* node) {
        if (node == nullptr) return;

        _size--;

        // 删除的是度为2的节点
        if (node->HasTwoChildren()) {
            Node* s = Predecessor(node); // 找到后继节点
            node->_interval = s->_interval; // 将后继节点的值覆盖需要删除的节点
            node->_value = s->_value;
            node = s; // 删除那个后继节点(后继节点肯定是度为0或者度为1)
        }

        // 删除度为0或1的节点node
        Node* replaceNode = (node->_left == nullptr) ? node->_right : node->_left;
        if (replaceNode != nullptr) { // 度为1的节点
            replaceNode->_parent = node->_parent;
            if (node->_parent == nullptr) { // 度为1，且是根节点
                _root = replaceNode;
            } else if (node == node->_parent->_left) {
                node->_parent->_left = replaceNode;
            } else if( node == node->_parent->_right) {
                node->_parent->_right = replaceNode;
            }

            // 删除节点后进行处理
            AfterRemove(node, replaceNode);
        } else if (node->_parent == nullptr) { // 度为0的节点，根节点
            _root = nullptr;

            AfterRemove(node, nullptr);
        } else { // 度为0的节点，叶子节点
            if (node == node->_parent->_left) {
                node->_parent->_left = nullptr;
            } else {
                node->_parent->_right = nullptr;
            }

            AfterRemove(node, nullptr);

            // 从父节点向上更新maxEnd
            Node* parent = node->_parent;
            while (parent != nullptr) {
                UpdateMaxEnd(parent);
                parent = parent->_parent;
            }
        }

        delete node;
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

    // 获取区间对应的节点
    Node* GetNode(const Interval& interval) {
        Node* node = _root;
        while (node != nullptr) {
            int cmp = CompareInterval(interval, node->_interval);
            if (cmp == 0) return node;
            if (cmp > 0) {
                node = node->_right;
            } else {
                node = node->_left;
            }
        }
        return nullptr;
    }

    // 更新节点的maxEnd
    void UpdateMaxEnd(Node* node) {
        if (_root == nullptr || node == nullptr) return;
        node->UpdateMaxEnd();
//        while (node != nullptr) {
//            node->UpdateMaxEnd();
//            node = node->_parent;
//        }
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
};

}


#endif // INTERVALTREEV2_H
