#ifndef MYBLOOMFILTER_H
#define MYBLOOMFILTER_H


#include <iostream>
#include <cmath>
#include "MyCommon.h"

namespace test {

// 一个元素一定不存在，或者可能存在
// 空间效率和查询效率都很高
// 缺点是有一定的误判率，删除困难
template <typename V,
          typename VHash = std::hash<V>>
class BloomFilter {
public:
#ifdef __i386
  typedef long long Long;
#else
  typedef long Long;
#endif
  int _bitSize; // 二进制向量长度
  int _hashSize; //哈希函数个数
  int _elementSize;
  Long *_bits;  // 二进制向量,数组长度取决于_bitSize

  // n: 数据规模，p:误判率
  BloomFilter(int n, double p) {
    if (n <= 0 || p <= 0 || p >= 1) {
      throw "Error parameters";
      exit(-1);
    }

    double ln2 = log(2);
    _bitSize = (int) (-(n*log(p)) / (ln2 * ln2));
    _hashSize = (int) (_bitSize * ln2 / n);
    _elementSize = sizeof(Long) * 8;
    int bitsLength = (int) ((_bitSize + _elementSize + 1 ) / _elementSize);
    _bits = new Long[bitsLength]{0};

    std::cout << "bitSize = " << _bitSize << "\n";
    std::cout << "hashSize = " << _hashSize << "\n";
    std::cout << "elementSize = " << _elementSize << "\n";
    std::cout << "bitsLength = " << bitsLength << "\n";

  }

  virtual ~BloomFilter() {
    if (_bits != nullptr)
      delete[] _bits;
  }

  // 添加元素
  // 每个哈希函数生成的位置都置1
  bool Put(const V& value) {
    int hash1 = VHash()(value);
    int hash2 = hash1 >> 16;

    bool result = false;
    for (int i = 1; i <= _hashSize; i++) {
      int combinedHash = hash1 + (i * hash2);
      if (combinedHash < 0) {
        combinedHash = ~combinedHash;
      }
      int index = combinedHash % _bitSize;
      if (SetBit(index))
        result = true;
    }
    return result;
  }

  // 判断元素是否存在
  bool Contains(const V& value) {
    int hash1 = VHash()(value);
    int hash2 = hash1 >> 16;

    for (int i = 1; i <= _hashSize; i++) {
      int combinedHash = hash1 + (i * hash2);
      if (combinedHash < 0) {
        combinedHash = ~combinedHash;
      }
      int index = combinedHash % _bitSize;
      if (!GetBit(index))
        return false;
    }
    return true;
  }

  bool SetBit(int index) {
    Long value = _bits[index / _elementSize];
    Long bitValue = (1 << (index % _elementSize));
    _bits[index / _elementSize] = value | bitValue;
    return (value & bitValue) == 0; //之前为0，现在设置为1了，返回true。之前为1，现在设置为1了，返回false
  }

  bool GetBit(int index) {
    Long value = _bits[index / _elementSize];
    return (value &= (1 << (index % _elementSize))) != 0;
  }
};

}
#endif // MYBLOOMFILTER_H
