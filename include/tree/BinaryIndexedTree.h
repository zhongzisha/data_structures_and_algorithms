#ifndef BINARYINDEXEDTREE_H
#define BINARYINDEXEDTREE_H


/* 树状数组(也叫做Fenwick Tree)。

其初衷是解决数据压缩里的累积频率（Cumulative Frequency）的计算问题，
现多用于高效计算数列的前缀和, 区间和。它可以以 O(logn) 的时间得到任意前缀和，并同时支
持在 O(logn) 时间内支持动态单点值的修改。空间复杂度 O(n)。

References:
1. https://zhuanlan.zhihu.com/p/99167607
2. https://cp-algorithms.com/data_structures/fenwick.html
*/

#include <iostream>
#include <vector>
#include <algorithm>

#include "MyCommon.h"

namespace test {

// 0-based indexing
class BinaryIndexedTreeSum {
public:

    std::vector<int> _bits;
    int _size;

    BinaryIndexedTreeSum(int size) : _size(size) {
        _bits.assign(_size, 0);
    }
    BinaryIndexedTreeSum(int arr[], int size)
        : BinaryIndexedTreeSum(size) {
        for (int i = 0; i < _size; i++) {
            Add(i, arr[i]);
        }
    }
    BinaryIndexedTreeSum(const std::vector<int>& a)
        : BinaryIndexedTreeSum(a.size()) {
        for (int i = 0; i < _size; i++) {
            Add(i, a[i]);
        }
    }

    virtual ~BinaryIndexedTreeSum() {

    }

    int Sum(int index) {
        int ret = 0;
        for(; index >= 0; index = (index & (index + 1)) - 1) {
            ret += _bits[index];
        }
        return ret;
    }

    // 求区间[l,r]的和
    int Sum(int l, int r) {
        return Sum(r) - Sum(l - 1);
    }

    void Add(int index, int delta) {
        for (; index < _size; index = (index | (index + 1))) {
            _bits[index] += delta;
        }
    }
};

// 0-based indexing
// [0, index]区间内的最小值
class BinaryIndexedTreeMin {
public:
    std::vector<int> _bits;
    int _size;

    BinaryIndexedTreeMin(int size) : _size(size) {
        _bits.assign(_size, INT_MAX);
    }
    BinaryIndexedTreeMin(int arr[], int size)
        : BinaryIndexedTreeMin(size) {
        for (int i = 0; i < _size; i++) {
            Update(i, arr[i]);
        }
    }
    BinaryIndexedTreeMin(const std::vector<int>& a)
        : BinaryIndexedTreeMin(a.size()) {
        for (int i = 0; i < _size; i++) {
            Update(i, a[i]);
        }
    }
    virtual ~BinaryIndexedTreeMin() {

    }

    void Update(int index, int value) {
        for (; index < _size; index = (index | (index + 1))) {
            _bits[index] = std::min(_bits[index], value);
        }
    }

    // 返回区间[0,index]内的最小值
    int GetMin(int index) {
        int ret = INT_MAX;
        for (; index >= 0; index = (index & (index + 1)) - 1) {
            ret = std::min(ret, _bits[index]);
        }
        return ret;
     }
};


// 1-based indexing
class BITSumOneBasedIndexing {
private:
    std::vector<int> _bits;
    int _size;

    int Sum(int index) {
        int ret = 0;
        for (++index; index > 0; index -= (index & -index)) {
            ret += _bits[index];
        }
        return ret;
    }

    void Add(int index, int value) {
        for (++index; index < _size; index += (index & -index)) {
            _bits[index] += value;
        }
    }

    void RangeAdd(int from, int to, int value) {
        Add(from, value);
        Add(to + 1, -value);
    }

public:
    BITSumOneBasedIndexing(int size) : _size(size + 1) {
        _bits.assign(_size + 1, 0);
    }
    BITSumOneBasedIndexing(int arr[], int size)
        : BITSumOneBasedIndexing(size) {
        for (int i = 0; i < _size; i++) {
            Add(i, arr[i]);
        }
    }
    int Sum(int from, int to) {
        return Sum(to) - Sum(from - 1);
    }
};


// 支持range update和range query
class BIT {
private:
    std::vector<int> B1;
    std::vector<int> B2;
    int _size;

    void Add(std::vector<int>& B, int index, int value) {
        while (index < _size) {
            B[index] += value;
            index += (index & -index);
        }
    }

    int Sum(std::vector<int>& B, int index) {
        int ret = 0;
        while (index > 0) {
            ret += B[index];
            index -= (index & -index);
        }
        return ret;
    }

    int PrefixSum(int index) {
        return Sum(B1, index) * index - Sum(B2, index);
    }

public:
    BIT(int size) : _size(size + 1) {
        B1.assign(_size, 0);
        B2.assign(_size, 0);
    }

    BIT(int arr[], int size) : BIT(size) {
        for (int i = 1; i < _size; i++) {
            RangeAdd(i, i, arr[i - 1]);
        }
    }

    virtual ~BIT() {

    }

    void RangeAdd(int from, int to, int value) {
        Add(B1, from, value);
        Add(B1, to + 1, -value);
        Add(B2, from, value * (from - 1));
        Add(B2, to + 1, -value * to);
    }

    int RangeSum(int from, int to) {
        return PrefixSum(to) - PrefixSum(from - 1);
    }
};


}


#endif // BINARYINDEXEDTREE_H
