#ifndef MYEXERCISES_H
#define MYEXERCISES_H

#include <iostream>
#include <algorithm>
#include <cstring>
#include <unordered_set>
#include "MyCommon.h"

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

};

}

#endif // MYEXERCISES_H
