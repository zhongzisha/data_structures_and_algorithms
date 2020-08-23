#ifndef SKIPLISTV2_H
#define SKIPLISTV2_H

#include <iostream>
#include <cmath>
#include "MyCommon.h"

namespace test {

template <typename K,
          typename V>
class SkipListV2 {
public:
    static constexpr int MAX_LEVEL = 32;
    static constexpr double PROB = 0.25;

    class Node {
    public:
        static int used_count;  // 类外初始化
        K _key;
        V _value;
        int _level;
        Node **_nexts; // Node*数组

        Node() {
            _level = MAX_LEVEL;
            _nexts = new Node*[MAX_LEVEL]{nullptr};
            used_count++;
            std::cout << "Node(), used_count = " << used_count << "\n";
        }
        Node(const K& key, const V& value, int level)
            : _key(key), _value(value), _level(level) {
            _nexts = new Node*[level]{nullptr};

            used_count++;
            std::cout << "Node(), used_count = " << used_count << "\n";
        }
        virtual ~Node() {
            delete[] _nexts;

            used_count--;
            std::cout << "~Node(), used_count = " << used_count << "\n";
        }
    };

public:
    int _size;
    int _level;
    Node *_first; //头结点

    SkipListV2()
        : _size(0),
          _level(1),
          _first(new Node()) {

    }

    virtual ~SkipListV2() {
        Node *node = _first;
        while (node->_nexts[0] != nullptr) {
            _first = node->_nexts[0];
            delete node;
            node = _first;
        }
        delete _first;
    }

    int Size() {
        return _size;
    }

    bool IsEmpty() {
        return _size == 0;
    }

    int Compare(const K& k1, const K& k2) {
        return k1 - k2;
    }

    void Put(const K& key, const V& value) {

        Node *node = _first;
        Node **prevs = new Node*[_level]{nullptr};

        bool found = false;
        // 从上面的层往下面走
        for (int i = _level - 1; i >= 0; i--) {
            int cmp = -1;
            while (node->_nexts[i] != nullptr
                   && (cmp = Compare(key, node->_nexts[i]->_key)) > 0) {
                node = node->_nexts[i];
            }
            if (cmp == 0) { //如果已经存在了key，则覆盖那个节点
                // V oldV = node->_nexts[i]->_value;
                node->_nexts[i]->_value = value;

                found = true;
                break;
            }
            prevs[i] = node; // 记录第i层的前驱节点
        }

        if (found) {
            delete[] prevs;
            return;
        }

        // 到这里，prevs里面保存了新插入节点的每一层的前驱节点

        int newLevel = RandomLevel();
        Node *newNode = new Node(key, value, newLevel);
        for (int i = 0; i < newLevel; i++) {
            if (i >= _level) {
                _first->_nexts[i] = newNode;
            } else {
                newNode->_nexts[i] = prevs[i]->_nexts[i];
                prevs[i]->_nexts[i] = newNode;
            }
        }
        delete[] prevs;

        _size++;

        _level = std::max(_level, newLevel);

    }

    std::pair<bool, V> Get(const K& key) {
        Node *node = _first;
        for (int i = _level - 1; i >= 0; i--) {
            int cmp = -1;
            while (node->_nexts[i] != nullptr
                   && (cmp = Compare(key, node->_nexts[i]->_key)) > 0) {
                node = node->_nexts[i];
            }
            if (cmp == 0)
                return std::make_pair(true, node->_nexts[i]->_value);
        }
        return std::make_pair(false, V());
    }

    std::pair<bool, V> Remove(const K& key) {
        Node *node = _first;
        Node **prevs = new Node*[_level]{nullptr};

        bool found = false;
        // 从上面的层往下面走
        for (int i = _level - 1; i >= 0; i--) {
            int cmp = -1;
            while (node->_nexts[i] != nullptr
                   && (cmp = Compare(key, node->_nexts[i]->_key)) > 0) {
                node = node->_nexts[i];
            }
            if (cmp == 0) {
                found = true;
            }
            prevs[i] = node; // 记录第i层的前驱节点
        }

        if (!found) {
            delete[] prevs;
            return std::make_pair(false, V());
        }
        // 到这里，prevs里面保存了新插入节点的每一层的前驱节点
        _size--;

        // 更新nexts
        Node *removedNode = node->_nexts[0]; //需要被删除的节点
        for (int i = 0; i < removedNode->_level; i++) {
            prevs[i]->_nexts[i] = removedNode->_nexts[i];
        }

        // 更新level
        int newLevel = _level;
        while (--newLevel >= 0 &&
               _first->_nexts[newLevel] == nullptr) {
            _level = newLevel;
        }

        delete[] prevs;

        V oldV = removedNode->_value;
        delete removedNode;
        return std::make_pair(true, oldV);

    }

    int RandomLevel() {
        int level = 1;
        while (((double)(rand() % 100) / 100) < PROB &&
               level < MAX_LEVEL) {
            level++;
        }
        return level;
    }

    friend std::ostream& operator<<(std::ostream& out, SkipListV2 *t) {
        if (t == nullptr) {
            out << "Empty SkipList.\n";
            return out;
        }

        // 打印每一层的链表
        out << "Size: " << t->Size() << "\n";
        out << "Level: " << t->_level << "\n";
        Node *node;
        for (int i = t->_level - 1; i >= 0; i--) {
            node = t->_first->_nexts[i];
            out << "Level " << i << ": ";
            while (node != nullptr) {
                out << node->_key;
                node = node->_nexts[i];
                if (node != nullptr) {
                    out << ", ";
                }
            }
            out << "\n";
        }
        return out;
    }

};

}


#endif // SKIPLISTV2_H
