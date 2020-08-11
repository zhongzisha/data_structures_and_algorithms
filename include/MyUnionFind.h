#ifndef MYUNIONFIND_H
#define MYUNIONFIND_H

#include <iostream>
#include <unordered_map>
#include "MyCommon.h"

namespace uf {

class UnionFind {  // quick find, slow union
public:
    int _capacity;
    int* parents;

    UnionFind(int capacity) : _capacity(capacity) {
        parents = new int[capacity];
        for (int i = 0; i < capacity; i++) {
            parents[i] = i;
        }
    }

    virtual ~UnionFind() {
        delete[] parents;
    }

    virtual int Find(int v);
    virtual void Union(int v1, int v2);


    // 是否属于同一个集合
    bool IsSame(int v1, int v2) {
        return Find(v1) == Find(v2);
    }


    void RangeCheck(int v) {
        if (v < 0 || v >= _capacity)
            throw "Error";
    }
};

class UnionFind_QF : public UnionFind {
public:
    UnionFind_QF(int capacity) : UnionFind(capacity) {

    }
    virtual ~UnionFind_QF() {

    }

    // 查找所属集合，根节点 O(1)
    int Find(int v) {
        RangeCheck(v);
        return parents[v];
    }

    void Union(int v1, int v2) {   // O(N)
        int p1 = Find(v1);
        int p2 = Find(v2);
        if (p1 == p2) return; //已经属于同一个集合，返回

        for (int i = 0; i < _capacity; i++) {
            if (parents[i] == p1) {
                parents[i] = p2; //将所以属于p1集合的节点合并到v2集合里，即修改其父节点
            }
        }


    }
};

class UnionFind_QU : public UnionFind {
public:
    UnionFind_QU(int capacity) : UnionFind(capacity) {

    }
    virtual ~UnionFind_QU() {

    }

    // 查找所属集合，根节点 O(logN)
    int Find(int v) {
        RangeCheck(v);
        while (v != parents[v]) {
            v = parents[v];  //一直向上找
        }
        return v;
    }

    //可能退化成链表
    //基于size的优化，元素少的树嫁接到元素多的树上
    //基于rank的优化，树矮的嫁接到树高的树上
    void Union(int v1, int v2) {   // O(N)
        int p1 = Find(v1);
        int p2 = Find(v2);
        if (p1 == p2) return; //已经属于同一个集合，返回

        parents[p1] = p2; //让左边的根节点指向右边节点的根节点，于是左边集合全部合并到右边集合了

    }
};


class UnionFind_QU_S : public UnionFind {
    //基于size的优化
public:
    int *sizes;

    UnionFind_QU_S(int capacity) : UnionFind(capacity) {

        sizes = new int[capacity];
        for (int i = 0; i < capacity; i++) {
            sizes[i] = 1;
        }
    }
    virtual ~UnionFind_QU_S() {
        delete[] sizes;
    }

    // 查找所属集合，根节点 O(logN)
    int Find(int v) {
        RangeCheck(v);
        while (v != parents[v]) {
            v = parents[v];  //一直向上找
        }
        return v;
    }

    //可能退化成链表
    //基于size的优化，元素少的树嫁接到元素多的树上
    void Union(int v1, int v2) {   // O(N)
        int p1 = Find(v1);
        int p2 = Find(v2);
        if (p1 == p2) return; //已经属于同一个集合，返回

        if (sizes[p1] < sizes[p2]) {
            parents[p1] = p2; //让左边的根节点指向右边节点的根节点，于是左边集合全部合并到右边集合了
            sizes[p2] += sizes[p1]; //更新size
        } else {
            parents[p2] = p1;
            sizes[p1] += sizes[p2];
        }

    }
};



class UnionFind_QU_R : public UnionFind {
    //基于rank的优化
public:
    int *ranks;

    UnionFind_QU_R(int capacity) : UnionFind(capacity) {

        ranks = new int[capacity];
        for (int i = 0; i < capacity; i++) {
            ranks[i] = 1;
        }
    }
    virtual ~UnionFind_QU_R() {
        delete[] ranks;
    }

    // 查找所属集合，根节点 O(logN)
    int Find(int v) {
        RangeCheck(v);
        while (v != parents[v]) {
            v = parents[v];  //一直向上找
        }
        return v;
    }

    //可能退化成链表
    //基于rank的优化，矮的树嫁接到高的树上，如果相同高度，左边嫁接到右边，但是右边树高度会加1
    void Union(int v1, int v2) {   // O(N)
        int p1 = Find(v1);
        int p2 = Find(v2);
        if (p1 == p2) return; //已经属于同一个集合，返回

        if (ranks[p1] < ranks[p2]) {
            parents[p1] = p2;
        } else if (ranks[p1] > ranks[p2]) {
            parents[p2] = p1;
        } else { //高度一样(特别要注意这种情况)
            parents[p1] = p2;
            ranks[p2]++;
        }

    }
};



class UnionFind_QU_R_PC : public UnionFind {
    //基于rank和path compression的优化
public:
    int *ranks;

    UnionFind_QU_R_PC(int capacity) : UnionFind(capacity) {

        ranks = new int[capacity];
        for (int i = 0; i < capacity; i++) {
            ranks[i] = 1;
        }
    }
    virtual ~UnionFind_QU_R_PC() {
        delete[] ranks;
    }

    // 查找所属集合，根节点 ，在这里进行path compression
    //缺点是实现成本较高，递归
    int Find(int v) {
        RangeCheck(v);
        if (parents[v] != v) {
            parents[v] = Find(parents[v]);
        }//路径上的每个节点都指向根节点，达到路劲压缩的目的
        return parents[v];
    }

    //可能退化成链表
    //基于rank的优化，矮的树嫁接到高的树上，如果相同高度，左边嫁接到右边，但是右边树高度会加1
    void Union(int v1, int v2) {   // O(N)
        int p1 = Find(v1);
        int p2 = Find(v2);
        if (p1 == p2) return; //已经属于同一个集合，返回

        if (ranks[p1] < ranks[p2]) {
            parents[p1] = p2;
        } else if (ranks[p1] > ranks[p2]) {
            parents[p2] = p1;
        } else { //高度一样(特别要注意这种情况)
            parents[p1] = p2;
            ranks[p2]++;
        }

    }
};



class UnionFind_QU_R_PS : public UnionFind {
    //基于path spliting的优化
public:
    int *ranks;

    UnionFind_QU_R_PS(int capacity) : UnionFind(capacity) {

        ranks = new int[capacity];
        for (int i = 0; i < capacity; i++) {
            ranks[i] = 1;
        }
    }
    virtual ~UnionFind_QU_R_PS() {
        delete[] ranks;
    }

    // 查找所属集合，根节点 O(logN)
    int Find(int v) {
        RangeCheck(v);
        while (v != parents[v]) {
            int p = parents[v]; //先保存其父节点
            parents[v] = parents[parents[v]];  //将当前节点直接指向祖父节点
            v = p;
        }
        return v;
    }

    //可能退化成链表
    //基于rank的优化，矮的树嫁接到高的树上，如果相同高度，左边嫁接到右边，但是右边树高度会加1
    void Union(int v1, int v2) {   // O(N)
        int p1 = Find(v1);
        int p2 = Find(v2);
        if (p1 == p2) return; //已经属于同一个集合，返回

        if (ranks[p1] < ranks[p2]) {
            parents[p1] = p2;
        } else if (ranks[p1] > ranks[p2]) {
            parents[p2] = p1;
        } else { //高度一样(特别要注意这种情况)
            parents[p1] = p2;
            ranks[p2]++;
        }

    }
};




class UnionFind_QU_R_PH : public UnionFind {
    //基于path halving的优化，每隔一个节点指向其祖父节点
public:
    int *ranks;

    UnionFind_QU_R_PH(int capacity) : UnionFind(capacity) {

        ranks = new int[capacity];
        for (int i = 0; i < capacity; i++) {
            ranks[i] = 1;
        }
    }
    virtual ~UnionFind_QU_R_PH() {
        delete[] ranks;
    }

    // 查找所属集合，根节点 O(logN)
    int Find(int v) {
        RangeCheck(v);
        while (v != parents[v]) {
            parents[v] = parents[parents[v]];  //将当前节点直接指向祖父节点
            v = parents[v];
        }
        return v;
    }

    //可能退化成链表
    //基于rank的优化，矮的树嫁接到高的树上，如果相同高度，左边嫁接到右边，但是右边树高度会加1
    void Union(int v1, int v2) {   // O(N)
        int p1 = Find(v1);
        int p2 = Find(v2);
        if (p1 == p2) return; //已经属于同一个集合，返回

        if (ranks[p1] < ranks[p2]) {
            parents[p1] = p2;
        } else if (ranks[p1] > ranks[p2]) {
            parents[p2] = p1;
        } else { //高度一样(特别要注意这种情况)
            parents[p1] = p2;
            ranks[p2]++;
        }

    }
};


template <typename V,
          typename VHash = std::hash<V>,
          typename VCmp = std::equal_to<V>>
class GenericUnionFind {
public:
    class Node {
    public:
        static int used_count;
        V _value;
        Node* _parent;
        int _rank;
        Node() : _parent(this), _rank(1) {
            used_count++;
            std::cout << "Node(), used_count = " << used_count << "\n";
        }
        Node(const V& value) : _value(value), _parent(this), _rank(1) {
            used_count++;
            std::cout << "Node(), used_count = " << used_count << "\n";
        }
        ~Node() {
            used_count--;
            std::cout << "Node(), used_count = " << used_count << "\n";
        }
    };

    class NodeHash {
    public:
        size_t operator()(const Node& rhs) const {
            return VHash()(rhs._value);
        }
        size_t operator()(const Node* rhs) const {
            return VHash()(rhs->_value);
        }
        size_t operator()(Node& rhs) {
            return VHash()(rhs._value);
        }
        size_t operator()(Node* rhs) {
            return VHash()(rhs->_value);
        }
    };
    class NodeCmp {
    public:
        bool operator()(const Node& lhs, const Node& rhs) const {
            return VCmp()(lhs._value, rhs._value);
        }
        bool operator()(Node& lhs, Node& rhs) {
            return VCmp()(lhs._value, rhs._value);
        }
        bool operator()(const Node* lhs, const Node* rhs) const {
            return VCmp()(lhs->_value, rhs->_value);
        }
        bool operator()(Node* lhs, Node* rhs) {
            return VCmp()(lhs->_value, rhs->_value);
        }
    };

public:
    typedef typename std::unordered_map<V, Node*, VHash, VCmp> NodeMap;
    typedef typename std::unordered_map<V, Node*, VHash, VCmp>::iterator NodeMapIterator;
    NodeMap *nodes;

    GenericUnionFind() {
        nodes = new NodeMap();
    }
    ~GenericUnionFind() {
        if (nodes != nullptr) {
            NodeMapIterator iter = nodes->begin();
            while (iter != nodes->end()) {
                delete (*iter).second;
                iter++;
            }
            delete nodes;
        }
    }

    void MakeSet(const V& v) {
        if (nodes->count(v) > 0) return;
        Node* node = new Node(v);
        nodes->insert(std::make_pair(v, node));
    }

    //找到v的根节点
    Node* FindNode(const V& v) {
        NodeMapIterator iter = nodes->find(v);
        if (iter == nodes->end()) return nullptr;
        Node* node = (*iter).second;
        while (!VCmp()(node->_value, node->_parent->_value)) {
            node->_parent = node->_parent->_parent;
            node = node->_parent;
        }
        return node;
    }

    //查找所在集合的根节点
    //如果没找到，会返回一个false做判断
    std::pair<bool, V>
    Find(const V& v) {
        Node* node = FindNode(v);
        return node == nullptr ? std::make_pair(false, V()) :
                                 std::make_pair(true, node->_value);
    }

    void Union(const V& v1, const V& v2) {
        Node* p1 = FindNode(v1);
        Node* p2 = FindNode(v2);
        if (p1 == nullptr || p2 == nullptr) return;
        if (NodeCmp()(p1, p2)) return;

        if (p1->_rank < p2->_rank) {
            p1->_parent = p2;
        } else if (p1->_rank > p2->_rank) {
            p2->_parent = p1;
        } else {
            p1->_parent = p2;
            p2->_rank ++;
        }
    }

    //判断两个对象是否属于同一个集合
    bool IsSame(const V& v1, const V& v2) {
        std::pair<bool, V> r1 = Find(v1);
        std::pair<bool, V> r2 = Find(v2);
        if (!r1.first || !r2.first) return false;
        return VCmp()(r1.second, r2.second);
    }

};

}

#endif // MYUNIONFIND_H
