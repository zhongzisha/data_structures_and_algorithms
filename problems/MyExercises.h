#ifndef MYEXERCISES_H
#define MYEXERCISES_H

#include <iostream>
#include <algorithm>
#include <cstring>
#include <unordered_set>
#include <vector>
#include "MyCommon.h"

using namespace std;

namespace problems {

struct Exercises {
    //数组中查找和为x的两个数
    static void TwoSum(int arr[], int n, int x) {
        std::sort(arr, arr + n - 1);
        int i = 0, j = n - 1;
        while (i < j) {
            int sum = arr[i] + arr[j];
            if (sum > x)
                j--;
            if (sum < x)
                i++;
            if (sum == x) {
                std::cout << arr[i] << " + " << arr[j] << "\n";
                i++;
            }
        }
    }

    //判断字符串是否为回文
    static void CheckPalindromes(const std::string& s) {
        bool result = true;
        int i = 0, j = s.length() - 1;
        while (i < j) {
            if (s[i] != s[j]) {
                result = false;
                break;
            }
            i++;
            j--;
        }
        if (result)
            std::cout << "yes\n";
        else
            std::cout << "no\n";
    }

    //区间和
    static void IntervalSum(int arr[], int n, int x) {
        int i = 0, j = 0;
        int sum = arr[0];
        while (j < n) {
            if (sum >= x) {
                if (sum == x) {
                    if (i == j) {
                        std::cout << "[" << i << "], "
                                  << arr[i] << " = "
                                  << x << "\n";
                    } else {
                        std::cout << "[" << i << ", " << j << "], "
                                  << arr[i] << " + " << arr[j] << " = "
                                  << x << "\n";
                    }
                }
                sum -= arr[i];
                i++;
                if (i > j) {
                    sum = arr[i];
                    j++;
                }
            } else {
                j++;
                sum += arr[j];
            }
        }
    }

    // 数组去重
    static void RemoveDuplicates(int a[], int n) {
        std::sort(a, a + n - 1);

        std::cout << "before remove: ";
        for (int i = 0; i < n; i++) {
            std::cout << a[i] << ", ";
        }
        std::cout << "\n";

        int i = 0, j = 0;
        int tmp;
        while (i < n) {
            i++;
            if (a[i] != a[j]) {
                j++;
                tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
        }

        std::cout << "after remove: ";
        for (int i = 0; i < j; i++) {
            std::cout << a[i] << ", ";
        }
        std::cout << "\n";
    }

    // 检测链表有环
    struct ListCycle {
        struct ListNode {
            static int used_count;
            int data;
            ListNode *next;
            ListNode() : next(nullptr) {
                used_count++;
                std::cout << "ListNode(), " << used_count << "\n";
            }
            ListNode(int data) : data(data), next(nullptr) {
                used_count++;
                std::cout << "ListNode(), " << used_count << "\n";
            }
            ~ListNode() {
                used_count--;
                std::cout << "~ListNode(), " << used_count << "\n";
            }
        };

        ListNode *head;

        ListCycle(int arr[], int n, int cycle_index = -1)
            : head(new ListNode()) {
            ListNode *node = head;
            ListNode *cycle_node = nullptr;
            for (int i = 0; i < n; i++) {
                node->next = new ListNode(arr[i]);
                node = node->next;
                if (i == cycle_index) {
                    cycle_node = node;
                }
            }

            if (cycle_index >= 0 && cycle_index < n) {
                node->next = cycle_node;
            }
        }

        ~ListCycle() {
            ListNode *node = nullptr;
            std::unordered_set<ListNode*> nodes;
            while (head != nullptr) {
                node = head;
                head = node->next;
                if (nodes.count(node) > 0)
                    break;
                nodes.insert(node);
                delete node;
            }
        }

        void Print() {
            std::cout << "\nList is: ";
            ListNode *node = head->next;
            if (node == nullptr) {
                std::cout << "empty.\n";
                return;
            }
            std::unordered_set<ListNode*> nodes;
            nodes.insert(node);
            while (node != nullptr) {
                std::cout << node->data;
                node = node->next;
                if (nodes.count(node) > 0) {
                    break;
                }
                nodes.insert(node);
                if (node != nullptr)
                    std::cout << " --> ";
            }
            std::cout << "\n";
        }

        // 检查链表是否有环
        bool IsCycled() {
            ListNode *slow = head;
            ListNode *fast = head->next;
            bool hasCycle = false;
            while (fast != nullptr && fast->next != nullptr) {
                if (slow == fast) {
                    hasCycle = true;
                    break;
                }
                slow = slow->next;
                fast = fast->next->next;
            }
            return hasCycle;
        }
    };

    // 二维树状数组, 1-based indexing
    struct TwoDimBIT {
        int n, m; // n rows, m cols
        vector<vector<int>> t1, t2, t3, t4;
        TwoDimBIT(int n_, int m_)
            : n(n_), m(m_),
            t1(n+1, vector<int>(m+1, 0)),
            t2(n+1, vector<int>(m+1, 0)),
            t3(n+1, vector<int>(m+1, 0)),
            t4(n+1, vector<int>(m+1, 0)) {}

        // lowbit(x) = (x & -(x))表示求x的二进制表示中最后一个1对应的十进制值
        // 在第一位=1，在第二位=2，在第三位=4，在第四位=8，...

        void Update(int x, int y, int delta) {
            for(int i = x; i <= n; i += (i & -i)) {
                for (int j = y; j <= m; j += (j & -j)) {
                    t1[i][j] += delta;
                    t2[i][j] += x * delta;
                    t3[i][j] += y * delta;
                    t4[i][j] += x * y * delta;
                }
            }
        }

        int Sum(int x, int y) {
            int result = 0;
            for (int i = x; i > 0; i -= (i & -i)) {
                for (int j = y; j > 0; j -= (j & -j)) {
                    result += (x+1)*(y+1)*t1[i][j] - (y+1)*t2[i][j] - (x+1)*t3[i][j] + t4[i][j];
                }
            }
            return result;
        }

        // 将[a,b],[c,d]为顶点的矩形区域内所有元素加上delta，有点积分图的感觉
        void RangeUpdate(int a, int b, int c, int d, int delta) {
            Update(a,   b,   delta);
            Update(c+1, d+1, delta);
            Update(a,   d+1, -delta);
            Update(c+1, b,   -delta);
        }

        // 求[a,b],[c,d]为顶点的矩形区域和
        int RangeSum(int a, int b, int c, int d) {
            return Sum(c, d) + Sum(a-1, b-1) - Sum(a-1, d) - Sum(c, b-1);
        }
    };

    // 逆序对的个数，树状数组，离散化
    struct ReversePair {
        struct Item {
            int index; // 离散化后的值
            int value; // 原始数值
        };
        int n;
        vector<Item> items;
        vector<int> tree;
        vector<int> rank;

        ReversePair(vector<int>&& arr) : n(arr.size()) {
            items.assign(n+1, Item());
            tree.assign(n+1, 0);
            rank.assign(n+1, 0);
            for (int i = 1; i <= n; i++) {
                items[i].index = i;
                items[i].value = arr[i - 1];
            }
        }

        int Count() {
            int count = 0;

            std::sort(items.begin() + 1, items.end(), [](Item& x, Item& y) -> bool {
                if (x.value == y.value)
                    return x.index < y.index;
                return x.value < y.value;
            });

            for (int i = 1; i <= n; i++) {
                rank[items[i].index] = i;  // 保存相对大小关系
            }

            for (int i = 1; i <= n; i++) { // 正序操作
                Update(rank[i], 1);
                count += i - Sum(rank[i]);
            }

//            for (int i = n; i > 0; i--) { // 倒序操作
//                Update(rank[i], 1);
//                count += Sum(rank[i] - 1);
//            }
            return count;
        }

        void Update(int x, int delta) {
            for(int i = x; i <= n; i += (i & -i)) {
                tree[i] += delta;
            }
        }

        int Sum(int x) {
            int result = 0;
            for (int i = x; i > 0; i -= (i & -i)) {
                result += tree[i];
            }
            return result;
        }

    };

};

}

#endif // MYEXERCISES_H
