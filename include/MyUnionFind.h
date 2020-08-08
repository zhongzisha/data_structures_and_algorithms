#ifndef MYUNIONFIND_H
#define MYUNIONFIND_H

#include <iostream>
#include "MyCommon.h"

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

#endif // MYUNIONFIND_H
