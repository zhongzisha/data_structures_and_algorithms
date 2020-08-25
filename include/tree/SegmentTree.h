#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

// 线段树
/*

Segment tree: stores intervals, and optimized for "which of these intervals contains
a given point" queries.
线段树，存储区间，查询哪个区间包含给定的点

Interval tree: stores intervals as well, but optimized for
"which of these intervals overlap with a given interval" queries.
It can also be used for point queries - similar to segment tree.
区间树：存储区间，查询哪个区间与给定的区间重叠，也可用来做点查询

Range tree: stores points, and optimized for "which points fall within
a given interval" queries.
范围树：存储点，查询哪些点落在给定的区间里面

Binary indexed tree: stores items-count per index, and optimized for
"how many items are there between index m and n" queries.
树状数组：每个位置存储元素-数目对，查询在索引m和n之间有多少个元素

References:
1. https://algs4.cs.princeton.edu/99misc/SegmentTree.java.html
2. https://stackoverflow.com/questions/17466218/
*/


#include <iostream>
#include <vector>
#include <optional>
#include <queue>
#include <cmath>
#include <utility>
#include "MyCommon.h"

namespace test {

class SegmentTree {
public:

    struct Node {
        static int used_count;
        int _sum;
        int _min;
        std::optional<int> _pendingVal;
        int _from;
        int _to;
        int Size() {
            return _to - _from + 1;
        }

        Node(int from, int to)
            : _sum(0),
              _min(0),
              _pendingVal(std::nullopt),
              _from(from),
              _to(to) {
            used_count++;
            std::cout << "Node(), used_count = " << used_count << "\n";
        }
        virtual ~Node() {
            used_count--;
            std::cout << "~Node(), used_count = " << used_count << "\n";
        }
    };

    Node** _heap;
    int* _array;
    int _size;
    int _heapSize;

    SegmentTree(int arr[], int size) {
        _size = size;
        _array = new int[_size];
        for (int i = 0; i < _size; i++) {
            _array[i] = arr[i];
        }

        _heapSize = (int) (2 * std::pow(2.0, std::floor(std::log2(double(_size)) + 1)));
        _heap = new Node*[_heapSize]{nullptr};
        Build(1, 0, _size);

        std::cout << "SegmentTree(), size = " << _size << ", heapSize = " << _heapSize << "\n";
    }
    virtual ~SegmentTree() {
        for (int i = 0; i < _heapSize; i++) {
            if (_heap[i] != nullptr) {
                delete _heap[i];
            }
        }
        delete[] _heap;
        delete[] _array;
        std::cout << "~SegmentTree()\n";
    }

    int Size() {
        return _size;
    }

    void Build(int index, int from, int size) {
        _heap[index] = new Node(from, from + size - 1);
        if (size == 1) {
            _heap[index]->_sum = _array[from];
            _heap[index]->_min = _array[from];
        } else {
            int len = size >> 1;
            int lIndex = 2 * index;
            int rIndex = lIndex + 1;
            Build(lIndex, from , len);
            Build(rIndex, from + len, size - len);
            _heap[index]->_sum = _heap[lIndex]->_sum + _heap[rIndex]->_sum;
            _heap[index]->_min = std::min(_heap[lIndex]->_min, _heap[rIndex]->_min);

        }
    }

    //
    void Propagate(int index) {
        Node *node = _heap[index];
        if (node != nullptr && node->_pendingVal != std::nullopt) {
            Change(_heap[2 * index], node->_pendingVal.value());
            Change(_heap[2 * index + 1], node->_pendingVal.value());
            node->_pendingVal = std::nullopt;
        }
    }

    void Change(Node* node, int value) {
        node->_pendingVal = value;
        node->_sum = node->Size() * value;
        node->_min = value;
        _array[node->_from] = value;
    }

    // 是否[from1, to1]包含[from2, to2]
    bool Contains(int from1, int to1, int from2, int to2) {
        return from2 >= from1 && to2 <= to1;
    }

    // 是否[from1, to1]与[from2, to2]交叉
    bool IsOverlap(int from1, int to1, int from2, int to2) {
        return ((from1 <= from2) && (to1 >= from2)) ||
                ((from1 >= from2) && (from1 <= to2));
    }

public:
    int RangeSumQuery(int from, int to) {
        return RangeSumQuery(1, from, to);
    }

private:
    int RangeSumQuery(int index, int from, int to) {
        Node *node = _heap[index];
        if (node->_pendingVal != std::nullopt &&
                Contains(node->_from, node->_to, from, to)) {
            return (to - from + 1) * (node->_pendingVal.value());
        }

        if (Contains(from, to, node->_from, node->_to)) {
            return _heap[index]->_sum;
        }

        if (IsOverlap(from, to, node->_from, node->_to)) {
            Propagate(index);
            int leftSum = RangeSumQuery(2*index, from, to);
            int rightSum = RangeSumQuery(2*index+1, from, to);
            return leftSum + rightSum;
        }

        return 0;
    }

public:
    int RangeMinQuery(int from, int to) {
        return RangeMinQuery(1, from, to);
    }
private:
    int RangeMinQuery(int index, int from, int to) {
        Node *node = _heap[index];

        if (node->_pendingVal != std::nullopt &&
                Contains(node->_from, node->_to, from, to)) {
            return node->_pendingVal.value();
        }

        if (Contains(from, to, node->_from, node->_to)) {
            return _heap[index]->_min;
        }

        if (IsOverlap(from, to, node->_from, node->_to)) {
            Propagate(index);
            int leftMin = RangeMinQuery(2*index, from, to);
            int rightMin = RangeMinQuery(2*index+1, from, to);
            return std::min(leftMin, rightMin);
        }

        return INT_MAX;
    }

public:
    void Update(int from, int to, int value) {
        return Update(1, from, to, value);
    }
private:
    // Lazy Propagation
    void Update(int index, int from, int to, int value) {
        Node *node = _heap[index];

        if (Contains(from, to, node->_from, node->_to)) {
            Change(node, value);
        }

        if (node->Size() == 1)
            return;

        if (IsOverlap(from, to, node->_from, node->_to)) {
            Propagate(index);
            int lIndex = 2 * index;
            int rIndex = lIndex + 1;
            Update(lIndex, from, to, value);
            Update(rIndex, from, to, value);

            node->_sum = _heap[lIndex]->_sum + _heap[rIndex]->_sum;
            node->_min = std::min(_heap[lIndex]->_min, _heap[rIndex]->_min);
        }
    }

public:
    friend std::ostream& operator<<(std::ostream& out, SegmentTree *t) {
        if (t == nullptr) {
            out << "Empty SegmentTree.\n";
            return out;
        }

        for (int i = 0; i < t->Size(); i++) {
            out << i << ": " << t->RangeSumQuery(i, i) << "\n";
        }
        return out;
    }
};



class SegmentTreeV2 {//统计区间内的最大值及其出现次数
public:
    typedef std::pair<int,int> Element;
    // first is the element itself, second is the number of occurences in segment
    Element *_array;
    int _size;

    SegmentTreeV2(int arr[], int size) {
        _size = size;
        _array = new Element[4 * _size];

        Build(arr, 1, 0, _size-1);
    }
    virtual ~SegmentTreeV2() {
        delete[] _array;
    }

private:
    Element Combine(const Element& a, const Element& b) {
        if (a.first > b.first) {
            return a;
        }
        if (b.first > a.first) {
            return b;
        }
        return std::make_pair(a.first, a.second + b.second);
    }

    void Build(int arr[], int index, int tl, int tr) {
        if (tl == tr) {
            _array[index] = std::make_pair(arr[tl], 1);
        } else {
            int tm = (tl + tr) >> 1;
            Build(arr, 2*index, tl, tm);
            Build(arr, 2*index+1, tm+1, tr);
            _array[index] = Combine(_array[2*index], _array[2*index+1]);
        }
    }

public:
    Element GetMax(int index, int tl, int tr, int l, int r) {
        if (l > r) {
            return std::make_pair(INT_MIN, 0);
        }
        if (l == tl && r == tr) {
            return _array[index];
        }
        int tm = (tl + tr) >> 1;
        return Combine(GetMax(2*index, tl, tm, l, std::min(r, tm)),
                       GetMax(2*index+1, tm+1, tr, std::max(l, tm+1), r));
    }

public:
    // 将pos位置的值更新为new_val
    void Update(int pos, int new_val) {
        Update(1, 0, _size-1, pos, new_val);
    }
private:
    void Update(int index, int tl, int tr, int pos, int new_val) {
        if (tl == tr) {
            _array[index] = std::make_pair(new_val, 1);
        } else {
            int tm = (tl + tr) >> 1;
            if (pos <= tm) {
                Update(2*index, tl, tm, pos, new_val);
            } else {
                Update(2*index+1, tm+1, tr, pos, new_val);
            }
            _array[index] = Combine(_array[2*index], _array[2*index+1]);
        }
    }

public:
    //将闭区间[from,to]的值更新为new_val
    void Update(int from, int to, int new_val) {
        Update(1, 0, _size-1, from, to, new_val);
    }
private:
    void Update(int index, int tl, int tr, int l, int r, int new_val) {
        if (l > r) return;
        if (tl == tr) {
            _array[index] = std::make_pair(new_val, 1);
        } else {
            int tm = (tl + tr) >> 1;
            Update(2*index, tl, tm, l, std::min(r, tm), new_val);
            Update(2*index+1, tm+1, tr, std::max(l, tm+1), r, new_val);
            _array[index] = Combine(_array[2*index], _array[2*index+1]);
        }
    }
};

// 计算区间内的最大公约数(Greatest Common Divisor)与最小公倍数(Least Common Multiple)
class SegmentTreeV3 {
public:
    int *_array; // 存储GCD
    int _size;

    SegmentTreeV3(int arr[], int size) {
        _size = size;
        _array = new int[4 * _size] {0};

        Build(arr, 1, 0, _size-1);
    }

    virtual ~SegmentTreeV3() {
        delete[] _array;
        _size = 0;
    }

private:
    std::optional<int> GCD(std::optional<int> a, std::optional<int> b) {
        if (a && b) {
            if (a.value() == b.value()) return a.value();
            int m = std::max(a.value(), b.value());
            int n = std::min(a.value(), b.value());
            int m1;
            while (n != 0) {
                m1 = m % n;
                m = n;
                n = m1;
            }
            return m;
        }
        if (a && !b) {
            return a;
        }
        if (!a && b) {
            return b;
        }
        return std::nullopt;
    }
    int LCM(int a, int b) {
        if (a == b) return a;
        int m = std::max(a, b);
        int n = std::min(a, b);
        int m1;
        long p = m*n;
        while (n != 0) {
            m1 = m % n;
            m = n;
            n = m1;
        }
        return (int)(p / m);
    }

    void Build(int arr[], int index, int tl, int tr) {
        if (tl == tr) {
            _array[index] = arr[tl];
        } else {
            int tm = (tl + tr) >> 1;
            Build(arr, 2*index, tl, tm);
            Build(arr, 2*index+1, tm+1, tr);
            _array[index] = GCD(_array[2*index], _array[2*index+1]).value();
        }
    }

public:
    std::optional<int> GetGCD(int from, int to) {
        return GetGCD(1, 0, _size-1, from, to);
    }
private:
    std::optional<int> GetGCD(int index, int tl, int tr, int l, int r) {
        if (l > r) {
            return std::nullopt;  // 两个区间没有重叠的情况，认为他们不存在最小公约数，返回空值
        }
        if (l == tl && r == tr) {
            return _array[index];
        }
        int tm = (tl + tr) >> 1;
        return GCD(GetGCD(2*index, tl, tm, l, std::min(r, tm)),
                   GetGCD(2*index+1, tm+1, tr, std::max(l, tm+1), r));
    }

public:
    //将闭区间[from,to]的值更新为new_val
    void Update(int from, int to, int new_val) {
        Update(1, 0, _size-1, from, to, new_val);
    }
private:
    void Update(int index, int tl, int tr, int l, int r, int new_val) {
        if (l > r) return;
        if (tl == tr) {
            _array[index] = new_val;
        } else {
            int tm = (tl + tr) >> 1;
            Update(2*index, tl, tm, l, std::min(r, tm), new_val);
            Update(2*index+1, tm+1, tr, std::max(l, tm+1), r, new_val);
            _array[index] = GCD(_array[2*index], _array[2*index+1]).value();
        }
    }
};

// 统计区间内0的个数，找出第k个0
class SegmentTreeV4 {
public:
    int *_array;
    int _size;

    SegmentTreeV4(int arr[], int size) {
        _size = size;
        _array = new int[4 * _size]{0};

        Build(arr, 1, 0, _size-1);
    }
    virtual ~SegmentTreeV4() {
        delete[] _array;
    }

    void Build(int arr[], int index, int tl, int tr) {
        if (tl == tr) {
            _array[index] = arr[tl] == 0 ? 1 : 0;
        } else {
            int tm = (tl + tr) >> 1;
            Build(arr, 2*index, tl, tm);
            Build(arr, 2*index+1, tm+1, tr);
            _array[index] = _array[2*index] + _array[2*index+1];
        }
    }

public:
    int CountZeros(int from, int to) {
        return CountZeros(1, 0, _size-1, from, to);
    }
private:
    int CountZeros(int index, int tl, int tr, int l, int r) {
        if (l > r) {
            return 0;
        }
        if (l == tl && r == tr) {
            return _array[index];
        }
        int tm = (tl + tr) >> 1;
        return CountZeros(2*index, tl, tm, l, std::min(r, tm)) +
                CountZeros(2*index+1, tm+1, tr, std::max(l, tm+1), r);
    }

public:
    void Update(int from, int to, int new_val) {
        Update(1, 0, _size-1, from, to, new_val);
    }
private:
    void Update(int index, int tl, int tr, int l, int r, int new_val) {
        if (l > r) return;
        if (tl == tr) {
            _array[index] = new_val == 0 ? 1 : 0;
        } else {
            int tm = (tl + tr) >> 1;
            Update(2*index, tl, tm, l, std::min(r, tm), new_val);
            Update(2*index+1, tm+1, tr, std::max(tm+1, l), r, new_val);
            _array[index] = _array[2*index] + _array[2*index+1];
        }
    }

public:
    int FindKthZero(int k) {
        return FindKthZero(1, 0, _size-1, k);
    }
    int FindKthZero(int k, int from, int to) { //返回区间内第k个0的位置
        // 先统计[0,from-1]位置有多少个0
        // k加上前面的0的个数就是[0,to]之间需要找的0的个数，
        // 再利用上面的算法，求得索引
        // 这个索引减去起始位置，就是那一段区间内第k个0的索引位置
        int zero_count = CountZeros(0, std::max(0, std::min(from-1, _size-1)));
        int pos = FindKthZero(1, 0, std::max(0, std::min(to, _size-1)), k + zero_count);
        if (pos >= 0)
            return pos - from;
        else
            return -1;
    }
private:
    int FindKthZero(int index, int tl, int tr, int k) {
        if (k > _array[index]) {
            return -1;
        }
        if (tl == tr) {
            return tl;
        }
        int tm = (tl + tr) >> 1;
        if (_array[2*index] >= k) {
            return FindKthZero(2*index, tl, tm, k);
        } else {
            return FindKthZero(2*index+1, tm+1, tr, k-_array[2*index]);
        }
    }
};

}

#endif // SEGMENTTREE_H
