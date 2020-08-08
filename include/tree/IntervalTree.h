#ifndef INTERVALTREE_H
#define INTERVALTREE_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cassert>
#include "MyCommon.h"

namespace test {

template <typename K, typename V>
class IntervalTree {
public:
    static int used_count;
    class Interval {
    public:
        K _start; //区间起始值
        K _stop;  //区间结束值
        V _value;  //区间附加的数据
        Interval(const K& start, const K& stop, const V& value)
            : _start(std::min(start, stop)),
              _stop(std::max(start, stop)),
              _value(value) {
            used_count++;
            std::cout << "Interval::Interval(), [" << _start << ", " << _stop
                      << "], used_count = " << used_count << "\n";

        }
        ~Interval() {
            used_count--;
            std::cout << "Interval::~Interval(), [" << _start << ", " << _stop
                      << "], used_count = " << used_count << "\n";
        }

        friend std::ostream& operator<<(std::ostream& out, const Interval& i) {
            out << "Interval(" << i._start << ", " << i._stop << "): " << i._value;
            return out;
        }
    };

public:
    typedef std::vector<Interval> IntervalVector;  //区间向量
    IntervalVector _intervals;     //存储区间树的所有区间
    std::unique_ptr<IntervalTree> _left;  //区间树的左子树
    std::unique_ptr<IntervalTree> _right; //区间树的右子树
    K _center;

    struct IntervalStartCmp { //比较器，比较区间起始值的大小
        bool operator()(const Interval& a, const Interval& b) {
            return a._start < b._start;
        }
    };

    struct IntervalStopCmp { //比较器，比较区间结束值得大小
        bool operator()(const Interval& a, const Interval& b) {
            return a._stop < b._stop;
        }
    };


    IntervalTree() : _left(nullptr), _right(nullptr), _center(K()) {
        std::cout << "IntervalTree::IntervalTree()\n";
    }

    IntervalTree(const IntervalTree& other)   //拷贝构造函数
        : _intervals(other._intervals),
          _left(other._left == nullptr ? other._left->Clone() : nullptr),
          _right(other._right == nullptr ? other._right->Clone() : nullptr),
          _center(other._center) {
        std::cout << "IntervalTree::IntervalTree(const IntervalTree& other)\n";
    }

    IntervalTree& operator=(IntervalTree&&) = default;  //赋值操作符，使用默认
    IntervalTree(IntervalTree&&) = default;   //移动构造函数，使用默认

    IntervalTree& operator=(const IntervalTree& other) {
        _center = other._center;
        _intervals = other._intervals;
        _left = other._left == nullptr ? other._left->Clone() : nullptr;
        _right = other._right == nullptr ? other._right->Clone() : nullptr;
        return *this;
    }

    ~IntervalTree() {
        std::cout << "IntervalTree::~IntervalTree()\n";
    }

    std::unique_ptr<IntervalTree> Clone() const {  //克隆一颗区间树
        return std::unique_ptr<IntervalTree>(new IntervalTree(*this));
    }

    //从区间向量构造一棵区间树
    IntervalTree(IntervalVector&& ivals,
                 std::size_t depth = 16,
                 std::size_t minBucket = 64,
                 std::size_t maxBucket = 512,
                 K leftExtent = 0,
                 K rightExtent = 0)
        : _left(nullptr), _right(nullptr){
        --depth;
        //获取区间向量中起始值里面的最小最大值
        const auto minmaxStart = std::minmax_element(ivals.begin(), ivals.end(), IntervalStartCmp());
        //获取区间向量中结束值里面的最小最大值
        const auto minmaxStop = std::minmax_element(ivals.begin(), ivals.end(), IntervalStopCmp());

        if (!ivals.empty()) {
            //计算中间切分点
            _center = (minmaxStart.first->_start + minmaxStop.second->_stop) / 2;
        }

        if (leftExtent == 0 && rightExtent == 0) {
            std::sort(ivals.begin(), ivals.end(), IntervalStartCmp());
        } else {
            assert(std::is_sorted(ivals.begin(), ivals.end(), IntervalStartCmp()));
        }

        if (depth == 0 || (ivals.size() < minBucket && ivals.size() < maxBucket)) {
            std::sort(ivals.begin(), ivals.end(), IntervalStartCmp());
            _intervals = std::move(ivals);
            assert(IsValid().first);
            return;
        } else {
            K leftp = 0;
            K rightp = 0;

            if (leftExtent || rightExtent) {
                leftp = leftExtent;
                rightp = rightExtent;
            } else {
                leftp = ivals.front()._start;
                rightp = std::max_element(ivals.begin(), ivals.end(), IntervalStopCmp())->_stop;
            }

            IntervalVector lefts;
            IntervalVector rights;

            for(typename IntervalVector::const_iterator i = ivals.begin();
                i != ivals.end(); ++i) {
                const Interval& interval = *i;
                if (interval._stop < _center) {
                    lefts.push_back(interval);
                } else if (interval._start > _center) {
                    rights.push_back(interval);
                } else {
                    assert(interval._start <= _center);
                    assert(_center <= interval._stop);
                    _intervals.push_back(interval);
                }
            }

            if (!lefts.empty()) {
                _left.reset(new IntervalTree(std::move(lefts),
                                             depth, minBucket, maxBucket,
                                             leftp, _center));
            }

            if (!rights.empty()) {
                _right.reset(new IntervalTree(std::move(rights),
                                              depth, minBucket, maxBucket,
                                              _center, rightp));
            }
        }
        assert(IsValid().first);
    }

    template <class UnaryFunction>
    void VisitNear(const K& start,
                   const K& stop,
                   UnaryFunction f) const {
        if (!_intervals.empty() && !(stop < _intervals.front()._start)) {
            for (auto &i : _intervals) {
                f(i);
            }
        }

        if (_left && start <= _center) {
            _left->VisitNear(start, stop, f);
        }

        if (_right && stop >= _center) {
            _right->VisitNear(start, stop, f);
        }
    }

    template <class UnaryFunction>
    void VisitOverlapping(const K& pos, UnaryFunction f) const {
        VisitOverlapping(pos, pos, f);
    }

    template <class UnaryFunction>
    void VisitOverlapping(const K& start, const K& stop, UnaryFunction f) const {
        auto filterF = [&](const Interval& interval) {
            if (interval._stop >= start && interval._start <= stop) {
                f(interval);
            }
        };

        VisitNear(start, stop, filterF);
    }

    template <class UnaryFunction>
    void VisitContained(const K& start, const K& stop, UnaryFunction f) const {
        auto filterF = [&](const Interval& interval) {
            if (start <= interval._start && interval._stop <= stop) {
                f(interval);
            }
        };

        VisitNear(start, stop, filterF);
    }

    IntervalVector FindOverlapping(const K& start, const K& stop) const {
        IntervalVector result;
        VisitOverlapping(start, stop,
                         [&](const Interval& interval) {
            result.emplace_back(interval);
        });
        return result;
    }

    IntervalVector FindContained(const K& start, const K& stop) const {
        IntervalVector result;
        VisitContained(start, stop,
                       [&](const Interval& interval) {
            result.push_back(interval);
        });
        return result;
    }

    bool Empty() const {
        if (_left && !_left->Empty()) {
            return false;
        }
        if (!_intervals.empty()) {
            return false;
        }
        if (_right && !_right->Empty()) {
            return false;
        }
        return true;
    }

    template <class UnaryFunction>
    void VisitAll(UnaryFunction f) const {
        if (_left) {
            _left->VisitAll(f);
        }
        std::for_each(_intervals.begin(), _intervals.end(), f);
        if (_right) {
            _right->VisitAll(f);
        }
    }

    std::pair<K, K> ExtentBruteForce() const {
        struct Extent {
            std::pair<K, K> x = {
                std::numeric_limits<K>::max(),
                std::numeric_limits<K>::min(),
            };
            void operator()(const Interval& interval) {
                x.first = std::min(x.first, interval._start);
                x.second = std::max(x.second, interval._stop);
            }
        };
        Extent extent;
        VisitAll([&](const Interval& interval) {
            extent(interval);
        });
        return extent.x;
    }

    std::pair<bool, std::pair<K, K>> IsValid() const {
        const auto minmaxStart = std::minmax_element(_intervals.begin(), _intervals.end(), IntervalStartCmp());
        const auto minmaxStop = std::minmax_element(_intervals.begin(), _intervals.end(), IntervalStopCmp());
        std::pair<bool, std::pair<K, K>> result = {true,
                                                   {std::numeric_limits<K>::max(),
                                                    std::numeric_limits<K>::min()}};
        if (!_intervals.empty()) {
            result.second.first = std::min(result.second.first, minmaxStart.first->_start);
            result.second.second = std::min(result.second.second, minmaxStop.second->_stop);
        }

        if (_left != nullptr) {
            auto valid = _left->IsValid();
            result.first &= valid.first;
            result.second.first = std::min(result.second.first, valid.second.first);
            result.second.second = std::min(result.second.second, valid.second.second);
            if (!result.first) {
                return result;
            }
            if (valid.second.second >= _center) {
                result.first = false;
                return result;
            }
        }

        if (_right != nullptr) {
            auto valid = _right->IsValid();
            result.first &= valid.first;
            result.second.first = std::min(result.second.first, valid.second.first);
            result.second.second = std::min(result.second.second, valid.second.second);
            if (!result.first) {
                return result;
            }
            if (valid.second.first <= _center) {
                result.first = false;
                return result;
            }
        }

        if (!std::is_sorted(_intervals.begin(), _intervals.end(), IntervalStartCmp())) {
            result.first = false;
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const IntervalTree& itree) {
        return writeOut(os, itree);
    }

    friend std::ostream& writeOut(std::ostream& os, const IntervalTree& itree,
                                  std::size_t depth = 0) {
        auto pad = [&]() { for (std::size_t i = 0; i != depth; ++i) { os << ' '; } };
        pad(); os << "center: " << itree._center << '\n';
        for (const Interval& inter : itree._intervals) {
            pad(); os << inter << '\n';
        }
        if (itree._left) {
            pad(); os << "left:\n";
            writeOut(os, *itree._left, depth + 1);
        } else {
            pad(); os << "left: nullptr\n";
        }
        if (itree._right) {
            pad(); os << "right:\n";
            writeOut(os, *itree._right, depth + 1);
        } else {
            pad(); os << "right: nullptr\n";
        }
        return os;
    }

};


}


#endif // INTERVALTREE_H
