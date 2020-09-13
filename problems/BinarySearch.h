#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include <iostream>
#include <vector>
#include <optional>
#include "MyCommon.h"

namespace problems {

struct BinarySearch {
    // 查找第一个大于等于x的数
    static std::optional<int> Run_v1(int arr[], int n, int x) {

        int l = 0, h = n, m;
        while (l < h) {
            m = l + (h - l) / 2; //m = (l + h) >> 1;
            if (arr[m] < x) {
                l = m + 1;
            } else {
                h = m;
            }
        }
        if (arr[l] < x) { // 如果没找到，返回空
            return std::nullopt;
        } else {
            return arr[l];
        }
    }

    // 查找第一个大于x的数
    static std::optional<int> Run_v2(int arr[], int n, int x) {

        int l = 0, h = n, m;
        while (l < h) {
            m = l + (h - l) / 2; //m = (l + h) >> 1;
            if (arr[m] <= x) {
                l = m + 1;
            } else {
                h = m;
            }
        }
        if (arr[l] <= x) { // 如果没找到，返回空
            return std::nullopt;
        } else {
            return arr[l];
        }
    }


    // 查找第一个小于等于x的数
    static std::optional<int> Run_v3(int arr[], int n, int x) {

        int l = 0, h = n, m;
        while (l < h) {
            m = l + (h - l) / 2; //m = (l + h) >> 1;
            if (arr[m] > x) {
                h = m;
            } else {
                l = m + 1;
            }
        }

        if (arr[h-1] > x) { // 如果没找到，返回空
            return std::nullopt;
        } else {
            return arr[h-1];  // h is the last index + 1, so we need to minus one
        }
    }
    // 查找第一个小于x的数
    static std::optional<int> Run_v4(int arr[], int n, int x) {

        int l = 0, h = n, m;
        while (l < h) {
            m = l + (h - l) / 2; //m = (l + h) >> 1;
            if (arr[m] >= x) {
                h = m;
            } else {
                l = m + 1;
            }
        }
        if (arr[h-1] >= x) { // 如果没找到，返回空
            return std::nullopt;
        } else {
            return arr[h-1];  // h is the last index + 1, so we need to minus one
        }
    }
};

}


#endif // BINARYSEARCH_H
