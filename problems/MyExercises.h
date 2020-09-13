#ifndef MYEXERCISES_H
#define MYEXERCISES_H

#include <iostream>
#include <algorithm>
#include "MyCommon.h"

namespace problems {

struct TwoSum {
    static void Run(int arr[], int n, int x) {
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
};

}

#endif // MYEXERCISES_H
