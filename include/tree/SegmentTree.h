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
#include <algorithm>
#include <utility>
#include <set>
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

// 数组前缀和，找出最小的i，使得数组前i个元素之和大于等于x
// 可以用二分搜索，但是时间复杂度为O((logn)^2)，这种怎么解的？
// 用线段树只需要O(logn)
class SegmentTreeV5 {
public:
    int *_array; // 存储区间最大值
    int _size;

    SegmentTreeV5(int arr[], int size) {
        _size = size;
        _array = new int[4 * _size]{0};

        Build(arr, 1, 0, _size-1);
    }

    virtual ~SegmentTreeV5() {
        delete[] _array;
    }

    void Build(int arr[], int index, int tl, int tr) {
        if (tl == tr) {
            _array[index] = arr[tl];
        } else {
            int tm = (tl + tr) >> 1;
            Build(arr, 2*index, tl, tm);
            Build(arr, 2*index+1, tm+1, tr);
            _array[index] = _array[2*index] + _array[2*index+1];
        }
    }

    // 得到区间[from, to]内第一个数组前缀和等于x的索引
    int GetFirstSum(int from, int to, int x) {
        return GetFirstSum(1, 0, _size-1, from, to, x);
    }
    int GetFirstSum(int index, int tl, int tr, int l, int r, int x) {
        if (tl > r || tr < l || tl == _size-1) return -1;
        if (l <= tl && tr <= r) {
            if (_array[index] < x) return -1;
            while (tl != tr) {
                int tm = (tl + tr) >> 1;
                if (_array[2*index] >= x) {
                    index = 2*index;
                    tr = tm;
                } else {
                    index = 2*index+1;
                    tl = tm+1;
                }
            }
            return tl;
        }

        int tm = (tl + tr) >> 1;
        int rs = GetFirstSum(2*index, tl, tm, l, r, x);
        if (rs != -1)
            return rs;
        return GetFirstSum(2*index+1, tm+1, tr, l, r, x);
    }
};

// 搜索某区间内第一个大于x的元素索引, O(logn)
// 可以用二分搜索，但是时间复杂度为O((logn)^2)，这种怎么解的？
// 用线段树只需要O(logn)
class SegmentTreeV6 {
public:
    int *_array; // 存储区间最大值
    int _size;

    SegmentTreeV6(int arr[], int size) {
        _size = size;
        _array = new int[4 * _size]{0};

        Build(arr, 1, 0, _size-1);
    }

    virtual ~SegmentTreeV6() {
        delete[] _array;
    }

    void Build(int arr[], int index, int tl, int tr) {
        if (tl == tr) {
            _array[index] = arr[tl];
        } else {
            int tm = (tl + tr) >> 1;
            Build(arr, 2*index, tl, tm);
            Build(arr, 2*index+1, tm+1, tr);
            _array[index] = std::max(_array[2*index], _array[2*index+1]);
        }
    }

    // 得到区间[from, to]内第一个大于x的数
    int GetFirstGreater(int from, int to, int x) {
        return GetFirstGreater(1, 0, _size-1, from, to, x);
    }
    int GetFirstGreater(int index, int tl, int tr, int l, int r, int x) {
        if (tl > r || tr < l || tl == _size-1) return -1;
        if (l <= tl && tr <= r) {
            if (_array[index] <= x) return -1;
            while (tl != tr) {
                int tm = (tl + tr) >> 1;
                if (_array[2*index] > x) {
                    index = 2*index;
                    tr = tm;
                } else {
                    index = 2*index+1;
                    tl = tm+1;
                }
            }
            return tl;
        }

        int tm = (tl + tr) >> 1;
        int rs = GetFirstGreater(2*index, tl, tm, l, r, x);
        if (rs != -1)
            return rs;
        return GetFirstGreater(2*index+1, tm+1, tr, l, r, x);
    }

};


// 求最大和的子序列(连续的)
class SegmentTreeV7 {
public:
    struct Element {
        static int used_count;
        int _id;
        int _sum;
        int _prefix_sum;
        int _suffix_sum;
        int _subarray_sum;

        Element() : _id(used_count) {     // 普通构造函数
            used_count++;
            std::cout << "Element(), used_count = " << used_count << ", id = " << _id << "\n";
        }

        Element(int val) : _id(used_count), _sum(val) {  // 普通构造函数
            _prefix_sum = _suffix_sum = _subarray_sum = std::max(0, val);

            used_count++;
            std::cout << "Element(int val), used_count = " << used_count << ", id = " << _id << "\n";
        }
        Element(const Element& e) : _id(used_count) {  // 拷贝构造函数
            used_count++;
            std::cout << "Element(const Element& e), used_count = " << used_count << ", id = " << _id << "\n";
            _sum = e._sum;
            _prefix_sum = e._prefix_sum;
            _suffix_sum = e._suffix_sum;
            _subarray_sum = e._subarray_sum;
        }
        Element(Element& e) : _id(used_count) {  // 拷贝构造函数
            used_count++;
            std::cout << "Element(Element& e), used_count = " << used_count << ", id = " << _id << "\n";
            _sum = e._sum;
            _prefix_sum = e._prefix_sum;
            _suffix_sum = e._suffix_sum;
            _subarray_sum = e._subarray_sum;
        }
        Element(Element&& e) : _id(used_count) {   // 移动构造函数
            used_count++;
            std::cout << "Element(Element&& e), used_count = " << used_count << ", id = " << _id << "\n";
            _sum = e._sum;
            _prefix_sum = e._prefix_sum;
            _suffix_sum = e._suffix_sum;
            _subarray_sum = e._subarray_sum;
        }

        Element& operator =(const Element& e) { //拷贝赋值操作符
            if (this != &e) {
                std::cout << "Element& operator =(const Element& e), used_count = " << used_count << ", id = " << _id << "\n";
                _sum = e._sum;
                _prefix_sum = e._prefix_sum;
                _suffix_sum = e._suffix_sum;
                _subarray_sum = e._subarray_sum;
            }
            return *this;
        }
        Element& operator =(Element& e) { //拷贝赋值操作符
            if (this != &e) {
                std::cout << "Element& operator =(Element& e), used_count = " << used_count << ", id = " << _id << "\n";
                _sum = e._sum;
                _prefix_sum = e._prefix_sum;
                _suffix_sum = e._suffix_sum;
                _subarray_sum = e._subarray_sum;
            }
            return *this;
        }
        Element& operator =(Element&& e) { //移动赋值操作符
            if (this != &e) {
                std::cout << "Element& operator =(Element&& e), used_count = " << used_count << ", id = " << _id << "\n";
                _sum = e._sum;
                _prefix_sum = e._prefix_sum;
                _suffix_sum = e._suffix_sum;
                _subarray_sum = e._subarray_sum;
            }
            return *this;
        }
        virtual ~Element() {
            used_count--;
            std::cout << "~Element(), used_count = " << used_count << ", id = " << _id << "\n";
        }
    };

private:
    Element Combine(const Element& e1, const Element& e2) {
        Element result;
        // 存储区间的子序列和
        result._sum = e1._sum + e2._sum;
        // 存储区间的最大前缀和
        result._prefix_sum = std::max(e1._prefix_sum,
                                      e1._sum + e2._prefix_sum);
        // 存储区间的最大后缀和
        result._suffix_sum = std::max(e2._suffix_sum,
                                      e2._sum + e1._suffix_sum);
        // 存储区间的最大子序列和
        result._subarray_sum = std::max(std::max(e1._subarray_sum,
                                                 e2._subarray_sum),
                                        e1._suffix_sum + e2._prefix_sum);
        std::cout << &result << "\n";
        return result;
    }

    void TestElement() {
        Element e1(10);
        Element e2(20);
        Element e3 = Combine(e1, e2);
        std::cout << "e3 address: " << &e3 << "\n";//这个地址与Combine中分配的局部变量地址一致，没有调用构造函数进行拷贝
        Element e4(e3); // 调用拷贝构造函数
        std::cout << "e4 address: " << &e4 << "\n";
        Element e5 = e4; // !!!!调用拷贝构造函数，不是赋值运算符!!!!
        std::cout << "e5 address: " << &e5 << "\n";
        Element e6; // 调用普通构造函数
        std::cout << "e6 address" << &e6 << "\n";
        e6 = e5;  // 调用赋值运算符
        std::cout << "e6 address" << &e6 << "\n";

        Element e7(std::move(e6));  // 移动构造函数
        std::cout << "e7 address: " << &e7 << ", e6 address: " << &e6 << ", "
                  << &(e7._sum) << ", " << &(e6._sum) << "\n";


        Element e8 = std::move(e6); // 移动构造函数
        std::cout << "e8 address: " << &e8 << ", e6 address: " << &e6 << ", "
                  << &(e8._sum) << ", " << &(e6._sum) << "\n";

        Element e9;  // 普通构造函数
        e9 = std::move(e6);   // 移动赋值运算符
        std::cout << "e9 address: " << &e9 << ", e6 address: " << &e6 << ", "
                  << &(e9._sum) << ", " << &(e6._sum) << "\n";
    }


    Element* _array;
    int _size;

public:
    SegmentTreeV7(int arr[], int size) {
        _size = size;
        _array = new Element[4 * _size];

        Build(arr, 1, 0, _size-1);
    }
    virtual ~SegmentTreeV7() {
        delete[] _array;
    }

private:
    void Build(int arr[], int index, int tl, int tr) {
        if (tl == tr) {
            _array[index] = Element(arr[tl]);
        } else {
            int tm = (tl + tr) >> 1;
            Build(arr, 2*index, tl, tm);
            Build(arr, 2*index+1, tm+1, tr);
            _array[index] = Combine(_array[2*index], _array[2*index+1]);
        }
    }

public:
    void Update(int pos, int new_val) {  // 点更新
        Update(1, 0, _size-1, pos, new_val);
    }
private:
    void Update(int index, int tl, int tr, int pos, int new_val) {
        if (tl == tr) {
            _array[index] = Element(new_val);
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
    void Update(int from, int to, int new_val) { // 区间更新
        Update(1, 0, _size-1, from, to, new_val);
    }
private:
    void Update(int index, int tl, int tr, int l, int r, int new_val) {
        if (l > r) return;
        if (l == tl && r == tr) {
            _array[index] = Element(new_val);
        } else {
            int tm = (tl + tr) >> 1;
            Update(2*index, tl, tm, l, std::min(r, tm), new_val);
            Update(2*index+1, tm+1, tr, std::max(l, tm+1), r, new_val);
            _array[index] = Combine(_array[2*index], _array[2*index+1]);
        }
    }

public:
    Element Query(int from, int to) {
        return Query(1, 0, _size-1, from, to);
    }
private:
    Element Query(int index, int tl, int tr, int l, int r) {
        if (l > r) {
            return Element(0);
        }
        if (l == tl && r == tr) {
            return _array[index];
        }
        int tm = (tl + tr) >> 1;
        return Combine(Query(2*index, tl, tm, l, std::min(r, tm)),
                       Query(2*index+1, tm+1, tr, std::max(l, tm+1), r));
    }
};


// 查询区间[from, to]内大于等于x的最小的那个数
class SegmentTreeV8 {
private:

    std::vector<int> *_array;
    int _size;
public:
    SegmentTreeV8(int arr[], int size) {
        _size = size;
        _array = new std::vector<int>[4 * _size];
        Build(arr, 1, 0, _size-1);
    }
    virtual ~SegmentTreeV8() {
        delete[] _array;
    }

private:
    void Build(int arr[], int index, int tl, int tr) {
        if (tl == tr) {
            _array[index] = std::vector<int>(1, arr[tl]);
        } else {
            int tm = (tl + tr) >> 1;
            Build(arr, 2*index, tl, tm);
            Build(arr, 2*index+1, tm+1, tr);
            std::merge(_array[2*index].begin(), _array[2*index].end(),
                    _array[2*index+1].begin(), _array[2*index+1].end(),
                    std::back_inserter(_array[index]));
        }
    }

public:
    int Query(int from, int to, int x) {
        return Query(1, 0, _size-1, from, to, x);
    }
private:
    int Query(int index, int tl, int tr, int l, int r, int x) {
        if (l > r) return INT_MAX;
        if (l == tl && r == tr) {
            std::vector<int>::iterator iter =
                    std::lower_bound(_array[index].begin(),
                                     _array[index].end(),
                                     x);
            if (iter != _array[index].end())
                return *iter;
            return INT_MAX;
        } else {
            int tm = (tl + tr) >> 1;
            return std::min(Query(2*index, tl, tm, l, std::min(r, tm), x),
                            Query(2*index+1, tm+1, tr, std::max(l, tm+1), r, x));
        }
    }

};


// 查询区间[from, to]内大于等于x的最小的那个数
// V8的基础上加入Update，使用multiset
class SegmentTreeV9 {
private:

    int *_data;
    std::multiset<int> *_array;
    int _size;
public:
    SegmentTreeV9(int arr[], int size) {
        _size = size;
        _data = new int[_size]{0};  // 更新时需要用到原始值
        for (int i = 0; i < _size; i++) {
            _data[i] = arr[i];
        }
        _array = new std::multiset<int>[4 * _size];
        Build(arr, 1, 0, _size-1);
    }
    virtual ~SegmentTreeV9() {
        delete[] _array;
        delete[] _data;
    }

private:
    void Build(int arr[], int index, int tl, int tr) {
        if (tl == tr) {
            _array[index].insert(arr[tl]);
        } else {
            int tm = (tl + tr) >> 1;
            Build(arr, 2*index, tl, tm);
            Build(arr, 2*index+1, tm+1, tr);
            std::merge(_array[2*index].begin(), _array[2*index].end(),
                    _array[2*index+1].begin(), _array[2*index+1].end(),
                    std::inserter(_array[index], _array[index].begin()));
        }
    }

public:
    int Query(int from, int to, int x) {
        return Query(1, 0, _size-1, from, to, x);
    }
private:
    int Query(int index, int tl, int tr, int l, int r, int x) {
        if (l > r) return INT_MAX;
        if (l == tl && r == tr) {
            std::multiset<int>::iterator iter =
                    std::lower_bound(_array[index].begin(),
                                     _array[index].end(),
                                     x);
            if (iter != _array[index].end())
                return *iter;
            return INT_MAX;
        } else {
            int tm = (tl + tr) >> 1;
            return std::min(Query(2*index, tl, tm, l, std::min(r, tm), x),
                            Query(2*index+1, tm+1, tr, std::max(l, tm+1), r, x));
        }
    }

public:
    void Update(int pos, int new_val) { // 点更新
        Update(1, 0, _size-1, pos, new_val);
    }
private:
    void Update(int index, int tl, int tr, int pos, int new_val) {
        _array[index].erase(_array[index].find(_data[pos]));
        _array[index].insert(new_val);
        if (tl != tr) {
            int tm = (tl + tr) >> 1;
            if (pos <= tm) {
                Update(2*index, tl, tm, pos, new_val);
            } else {
                Update(2*index+1, tm+1, tr, pos, new_val);
            }
        } else {
            _data[pos] = new_val;
        }
    }

};


// 可持久化线段树 Persistent Segment Tree

// 查找区间中第k小的数

// Lazy Propagation，懒惰更新，懒惰传播

// 隐式线段树 Implicit Segment Tree

// 2D Segment Tree，高维线段树

// 空间压缩

}

#endif // SEGMENTTREE_H
