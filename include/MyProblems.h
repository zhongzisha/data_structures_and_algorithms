#ifndef MYPROBLEMS_H
#define MYPROBLEMS_H

#include <iostream>
#include <string>
#include "MyCommon.h"

using std::string;

namespace my_problems {

// Recursion
class Fibonacci {
public:

    // 多次重复计算
    static int Run_v1(int n) {
        std::cout << "Run_v1(" << n << ")\n";  //pre-order traverse
        if (n <= 2) return 1;
        return Run_v1(n - 1) + Run_v1(n - 2);
    }

    // 用一个数组缓存结果，减少调用次数
    static int Run_v2(int n) {
        if (n <= 2) return 1;
        int *arr = new int[n+1]{0};
        arr[1] = arr[2] = 1;
        int result = Run_v2(n, arr);
        delete[] arr;
        return result;
    }
    static int Run_v2(int n, int arr[]) {
        if (arr[n] == 0) {
            std::cout << "Run_v2(" << n << ")\n";
            arr[n] = Run_v2(n - 1, arr) + Run_v2(n - 2, arr);
        }
        return arr[n];
    }

    // v2基础上去除递归
    static int Run_v3(int n) {
        if (n <= 2) return 1;
        int *arr = new int[n+1]{0};
        arr[1] = arr[2] = 1;
        for (int i = 3; i <= n; i++) {
            arr[i] = arr[i - 1] + arr[i - 2];
        }
        int result = arr[n];
        delete[] arr;
        return result;
    }

    // 滚动数组
    static int Run_v4(int n) {
        if (n <= 2) return 1;
        int arr[2] = {1, 1};
        int tmp;  //用了一个临时变量
        for (int i = 3; i <= n; i++) {
            tmp = arr[1];
            arr[1] += arr[0];
            arr[0] = tmp;
        }
        return arr[1];
    }
    static int Run_v5(int n) {  //使用取模
        if (n <= 2) return 1;
        int arr[2] = {1, 1};
        for (int i = 3; i <= n; i++) {
            arr[i%2] = arr[(i-1)%2] + arr[(i-2)%2];
        }
        return arr[n%2];
    }
    static int Run_v6(int n) {  //用位运算替换取模
        if (n <= 2) return 1;
        int arr[2] = {1, 1};
        for (int i = 3; i <= n; i++) {
            arr[i & 1] = arr[(i-1) & 1] + arr[(i-2) & 1];
        }
        return arr[n & 1];
    }

    // 只用两个变量
    static int Run_v7(int n) {
        if (n <= 2) return 1;
        int first = 1;
        int second = 1;
        for (int i = 3; i <= n ; i++) {
            second += first;
            first = second - first;
        }
        return second;
    }

};

// Recursion
class ClimpStairs {
public:
    // n: 台阶个数
    // f(n) = f(n-1) + f(n-2)
    // 与Fibonacci类似，只是初始条件不同
    static int Run(int n) {
        if (n == 1) return 1;
        if (n == 2) return 2;
        int first = 1;
        int second = 2;
        for (int i = 3; i <= n ; i++) {
            second += first;
            first = second - first;
        }
        return second;
    }
};

// Recursion
class Hanoi {
public:
    // a,b,c三个柱子
    // a上有n个盘子，把这些盘子移动到c上
    // 大盘子必须在小盘子下面
    static void Run(int n, const string& a, const string& b, const string& c) {
        if (n == 1) {
            Move(1, a, c);
            return;
        }
        // 将a上的n-1个盘子移动到b，c为中继
        // 将a的第n个盘子移动到c, 此时,a为空柱子
        // 将b上的n-1个盘子移动到c，a为中继
        Run(n-1, a, c, b);
        Move(n, a, c);
        Run(n-1, b, a, c);
    }

    static void Move(int plate_id, const string& from, const string& to) {
        std::cout << "move " << plate_id << " from " << from << " to " << to << "\n";
    }
};


// Back-tracking
// 优化：bool数组、位运算
class Queens {
private:
    int _n;
    int *_cols;
    int _ways;

public:
    Queens(int n) {
        if (n <= 0) {
            throw "n must be positive";
            exit(-1);
        }

        _n = n;
        _cols = new int[_n]; // 存储每一行皇后摆放的列号
        _ways = 0;

        Place(0);
    }
    virtual ~Queens() {
        delete[] _cols;
    }

    int GetWays() {
        return _ways;
    }

    void Place(int row) {
        if (row == _n) {
            _ways++;
            Show();
            return;
        }

        for (int col = 0; col < _n; col++) {
            if (IsValid(row, col)) {
                _cols[row] = col;
                Place(row + 1);
            }
        }
    }

    // _cols里面保存每一行的第几列已经摆放了皇后
    //
    bool IsValid(int row, int col) {
       for (int i = 0; i < row; i++) {
           if (_cols[i] == col) { //前面的行里面，已经在这一列摆放了皇后
               return false;
           }
           if ((row - i) == std::abs(col - _cols[i])) { // 有皇后与当前位置是处在一条斜线上
               return false;
           }
       }
       return true;
    }

    void Show() {
        for (int row = 0; row < _n; row++) {
            for (int col = 0; col < _n; col++) {
                if (_cols[row] == col) {
                    std::cout << "1 ";
                } else {
                    std::cout << "0 ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "---------------------------------\n";
    }

};


// Greedy
class Pirate {
public:
    static int cmp(const void* a, const void* b) {
        return *(int*)a > *(int*)b;
    }

    static int Run(int capacity, int weights[], int size) {

        std::cout << "size=" <<size <<"\n";
        std::qsort(weights, size, sizeof(int), cmp);
        for (int i = 0; i <size; i++) {
            std::cout <<weights[i] <<",";
        }
        std::cout <<"\n";
        int weight = 0, count = 0;
        for (int i = 0; i < size; i++) {
            int newWeight = weight + weights[i];
            if (newWeight <= capacity) {
                weight = newWeight;
                count++;
            }
        }
        return count;
    }

};

// Greedy
class CoinChange {
public:

    static int cmp(const void* a, const void* b) {
        return *(int*)a < *(int*)b;
    }
    static int Run(int money, int faces[], int size) {
        std::qsort(faces, size, sizeof(int), cmp);
        int coins = 0, i = 0;
        while (i < size) {
            if (money < faces[i]) {
                i++;
                continue;
            }
            std::cout << faces[i] << "\n";
            money -= faces[i];
            coins++;
        }
        return coins;
    }
};

// Greedy, sort by what
class Knapsack {

};

// Divide and Conquer
class MaxSubSeqSum { //最大连续子序列和, Greatest Slice，最大切片问题
public:
    // bruteforce O(n^3)
    static int Run_V1(int nums[], int size) {
        int max = INT_MIN;
        for (int begin = 0; begin < size; begin++) {
            for (int end = 0; end < size; end++) {
                int sum = 0;
                for (int i = begin; i <= end; i++) {
                    sum += nums[i];
                }
                max = std::max(max, sum);
            }
        }
        return max;
    }

    // bruteforce, O(n^2)
    static int Run_V2(int nums[], int size) {
        int max = INT_MIN;
        for (int begin = 0; begin < size; begin++) {
            int sum = 0;
            for (int end = begin; end < size; end++) {
                sum += nums[end];
                max = std::max(max, sum);
            }
        }
        return max;
    }

    // divide and conquer
    // T(n) = T(n/2) + T(n/2) + O(n)
    // O(nlogn)
    static int Run_V3(int nums[], int size) {
        return Run_V3(nums, size, 0, size);
    }
    static int Run_V3(int nums[], int size, int begin, int end) {
        if (end - begin < 2) return nums[begin];
        int mid = (begin + end) >> 1;
        int leftMax = 0;
        int leftSum = 0;
        for (int i = mid - 1; i >= begin; i--) {
            leftSum += nums[i];
            leftMax = std::max(leftMax, leftSum);
        }
        int rightMax = 0;
        int rightSum = 0;
        for (int i = mid; i < end; i++) {
            rightSum += nums[i];
            rightMax = std::max(rightMax, rightSum);
        }
        int max = leftMax + rightMax;
        return std::max(max, std::max(Run_V3(nums, size, begin, mid),
                                      Run_V3(nums, size, mid, end)));
    }
};

// Dynamic Programming
// 子序列：可以不连续
// 子串、子数组、子区间：必须是连续的

class CoinChange_DP
{
public:
    // 很多次递归调用
    static int Run(int money, int faces[], int size) {
        // std::cout << "Run(" << money << ")\n";
        if (money < 1) {
            return INT_MAX;
        }
        for (int i = 0; i < size; i++) {
            if (money == faces[i]) { //如果恰好金额与硬币面值相等，直接一枚硬币即可
                return 1;
            }
        }

        int min_sol = INT_MAX;
        for (int i = 0; i < size; i++) {
            int sol = Run(money - faces[i], faces, size);
            if (sol < min_sol) {
                min_sol = sol;
            }
        }
        return min_sol + 1;
    }

    // 记忆化搜索，减少对子问题的重复递归调用
    static int Run_V2(int money, int faces[], int size) {
        if (money < 1) return -1;
        int* dp = new int[money + 1] {0};
        for (int i = 0; i < size; i++) {
            if (faces[i] > money)
                continue;
            dp[faces[i]] = 1;
        }

        int coin_count = Run_V2(money, dp, faces, size);
        delete[] dp;

        return coin_count;
    }
    static int Run_V2(int money, int dp[], int faces[], int size) {
        if (money < 1) return INT_MAX;
        if (dp[money] == 0) {
            //std::cout << "Run_V2(" << money << ")\n";
            int min_sol = INT_MAX;
            for (int i = 0; i < size; i++) {
                int sol = Run_V2(money - faces[i], dp, faces, size);
                if (sol < min_sol) {
                    min_sol = sol;
                }
            }
            dp[money] = min_sol + 1;
        }
        return dp[money];
    }

    // 记忆化搜索，减少对子问题的重复递归调用
    // 先对面值排序，先解决面值较小的子问题，也可以减少一些重复调用
    static int cmp_less(const void* a, const void* b) {
        return *(int*)a < *(int*)b;  // >是从小到大排序，<是从大到小排序
    }
    static int Run_V3(int money, int faces[], int size) {
        if (money < 1) return -1;
        std::qsort(faces, size, sizeof(int), cmp_less);

        int* dp = new int[money + 1] {0}; // 必须初始化为0
        for (int i = 0; i < size; i++) {
            if (faces[i] > money) // 这里需要考虑面值比金额大的情况，否则会出现数组越界问题
                continue;
            dp[faces[i]] = 1;
        }

        int coin_count = Run_V3(money, dp, faces, size);
        delete[] dp;

        return coin_count;
    }
    static int Run_V3(int money, int dp[], int faces[], int size) {
        if (money < 1) return INT_MAX;
        if (dp[money] == 0) {
            //std::cout << "Run_V3(" << money << ")\n";
            int min_sol = INT_MAX;
            for (int i = 0; i < size; i++) {
                int sol = Run_V3(money - faces[i], dp, faces, size);
                if (sol < min_sol) {
                    min_sol = sol;
                }
            }
            dp[money] = min_sol + 1;
        }
        return dp[money];
    }

    // 递推，自底向上
    static int Run_V4(int money, int faces[], int size) {
        if (money < 1) return -1;

        std::qsort(faces, size, sizeof(int), cmp_less);

        int* dp = new int[money + 1] {0};

        // 先计算小面额需要的最少硬币个数，逐步递推到大面额需要的最小硬币个数
        for(int i = 1; i <= money; i++) {
            int min = INT_MAX;
            for (int j = 0; j < size; j++) {
                if (i >= faces[j]) {
                    min = std::min(dp[i - faces[j]], min);
                }
            }
            dp[i] = min + 1;
        }

        int coin_count = dp[money];
        delete[] dp;

        return coin_count;
    }

    // 递推，自底向上
    // 记录所选择的硬币面值
    static int Run_V5(int money, int faces[], int size) {
        if (money < 1) return -1;

        std::qsort(faces, size, sizeof(int), cmp_less);

        int* dp = new int[money + 1] {0};
        int* selected_coins = new int[money + 1] {0};

        // 先计算小面额需要的最少硬币个数，逐步递推到大面额需要的最小硬币个数
        for(int i = 1; i <= money; i++) {
            int min = INT_MAX;
            for (int j = 0; j < size; j++) {
                if (i >= faces[j] && dp[i - faces[j]] < min) {
                    min = dp[i - faces[j]];
                    selected_coins[i] = faces[j]; //  记录凑够i钱最后一次选择的硬币
                }
            }
            dp[i] = min + 1;
            PrintSelectedCoins(selected_coins, i); //打印凑够i这么多钱需要的硬币组成
        }

        int coin_count = dp[money];

        //PrintSelectedCoins(selected_coins, money);

        delete[] dp;
        delete[] selected_coins;

        return coin_count;
    }
    static void PrintSelectedCoins(int* selected_coins, int money) {
        std::cout << "selected coins for " << money << ": ";
        int tmp = money;
        while (tmp > 0) {
            std::cout << selected_coins[tmp] << ", ";
            tmp -= selected_coins[tmp];
        }
        std::cout << "\n";
    }

    // 递推，自底向上
    // 记录所选择的硬币面值
    // 考虑凑不够的情况
    static int Run_V6(int money, int faces[], int size) {
        if (money < 1) return -1;

        std::qsort(faces, size, sizeof(int), cmp_less);

        int* dp = new int[money + 1] {0};
        int* selected_coins = new int[money + 1] {0};

        // 先计算小面额需要的最少硬币个数，逐步递推到大面额需要的最小硬币个数
        for(int i = 1; i <= money; i++) {
            int min = INT_MAX;
            for (int j = 0; j < size; j++) {
                if (i >= faces[j] && dp[i - faces[j]] >= 0 && dp[i - faces[j]] < min) {
                    min = dp[i - faces[j]];
                    selected_coins[i] = faces[j]; //  记录凑够i钱最后一次选择的硬币
                }
            }
            if (min == INT_MAX) {
                dp[i] = -1;
            } else {
                dp[i] = min + 1;
                PrintSelectedCoins(selected_coins, i); //打印凑够i这么多钱需要的硬币组成
            }
        }

        int coin_count = dp[money];

        //PrintSelectedCoins(selected_coins, money);

        delete[] dp;
        delete[] selected_coins;

        return coin_count;
    }
};

// 最大连续子序列和，DP
class MaxSubSeqSum_DP
{
public:
    static int Run(int nums[], int length) {
        if (length < 1) return INT_MIN;
        if (length == 1) return nums[0];
        int* dp = new int[length + 1];
        dp[0] = nums[0];
        int max = nums[0];
        for (int i = 1; i < length; i++) {
            if (dp[i - 1] <= 0) {
                dp[i] = nums[i];
            } else {
                dp[i] = dp[i - 1] + nums[i];
            }
            max = std::max(max, dp[i]);
        }
        delete[] dp;
        return max;
    }


    static int Run_V2(int nums[], int length) {
        if (length < 1) return INT_MIN;
        if (length == 1) return nums[0];
        int dp = nums[0];
        int max = nums[0];
        for (int i = 1; i < length; i++) {
            if (dp <= 0) {
                dp = nums[i];
            } else {
                dp += nums[i];
            }
            max = std::max(max, dp);
        }
        return max;
    }

};


// 最长上升子序列的长度 LIS，子序列可以不连续
class LongestIncreasiveSubSeq
{
public:
    //DP算法
    static int Run(int nums[], int length) {  // O(N^2), O(N)
        if (length < 1) return 0;
        if (length == 1) return 1;
        if (length == 2) return nums[1] > nums[0] ? 1 : 0;
        int* dp = new int[length + 1];
        int max = INT_MIN;
        for (int i = 0; i < length; i++) {
            dp[i] = 1; // nums[i]当前元素就是一个序列，因此为1
            for (int j = 0; j < i; j++) {
                if (nums[j] >= nums[i]) continue;
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
            max = std::max(max, dp[i]);
            std::cout << nums[i] << " : " << dp[i] << "\n";
        }

        delete[] dp;
        return max;
    }

    //二分搜索算法,O(nlogn), O(n)
    static int Run_V2(int nums[], int length) {
        if (length < 1) return 0;
        if (length == 1) return 1;
        if (length == 2) return nums[1] > nums[0] ? 1 : 0;
        int* top = new int[length + 1] {0};
        int len = 0;
        for (int i = 0; i < length; i++) {
            int begin = 0, end = len;
            while (begin < end) {
                int mid = (begin + end) >> 1;
                if (nums[i] <= top[mid]) {
                    end = mid;
                } else {
                    begin = mid + 1;
                }
            }
            top[begin] = nums[i];
            if (begin == len)
                len++;
        }
        delete[] top;
        return len;
    }
};

// 最长公共子序列，LCS
class LongestCommonSubSeq
{
public:
    // 递归
    static void Run_V1(int nums1[], int len1,
                       int nums2[], int len2) {
        if (len1 == 0 || len2 == 0) return;
        int result = Run_V1_sub(nums1, len1, nums2, len2);
        std::cout << "result = " << result << "\n";
    }
    static int Run_V1_sub(int nums1[], int i,
                           int nums2[], int j) {
        if (i == 0 || j == 0) return 0;
        if (nums1[i-1] != nums2[j-1]) {
            return std::max(Run_V1_sub(nums1, i, nums2, j-1),
                            Run_V1_sub(nums1, i-1, nums2, j));
        }
        return Run_V1_sub(nums1, i-1, nums2, j-1) + 1;
    }

    // DP
    static void Run_V2(int nums1[], int len1,
                       int nums2[], int len2) {
        //dp array: nums1的前i个元素与nums2的前j个元素最长公共子序列长度
        //dp[0,j],dp[i,0]都是0
        int** dp = new int*[len1+1];
        for (int i = 0; i < len1+1; i++) {
            dp[i] = new int[len2+1]{0}; //初始化0，默认不为0
        }
        for (int i = 1; i <= len1; i++) {
            for (int j = 1; j <= len2; j++) {
                if (nums1[i-1] == nums2[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i][j-1], dp[i-1][j]);
                }
            }
        }
        std::cout << "result = " << dp[len1][len2] << "\n";

        // print dp array
        std::cout << "dp array: \n";
        for (int i = 0; i < len1+1; i++) {
            for (int j = 0; j < len2+1; j++) {
                std::cout << dp[i][j] << ", ";
            }
            std::cout << "\n";
        }

        // free memory
        for (int i = 0; i < len1+1; i++) {
            delete[] dp[i];
        }
        delete[] dp;
    }

    // DP
    // 滚动数组
    static void Run_V3(int nums1[], int len1,
                       int nums2[], int len2) {
        //dp array: nums1的前i个元素与nums2的前j个元素最长公共子序列长度
        //dp[0,j],dp[i,0]都是0
        int** dp = new int*[2]; //只有两行
        for (int i = 0; i < 2; i++) {
            dp[i] = new int[len2+1]{0}; //初始化0，默认不为0
        }
        for (int i = 1; i <= len1; i++) {
            for (int j = 1; j <= len2; j++) {
                if (nums1[i-1] == nums2[j-1]) {
                    dp[i%2][j] = dp[(i-1)%2][j-1] + 1;
                } else {
                    dp[i%2][j] = std::max(dp[i%2][j-1], dp[(i-1)%2][j]);
                }
            }
        }
        std::cout << "result = " << dp[len1%2][len2] << "\n";

        // free memory
        for (int i = 0; i < 2; i++) {
            delete[] dp[i];
        }
        delete[] dp;
    }

    // DP
    // 滚动数组
    // 用位运算替代取模操作
    static void Run_V4(int nums1[], int len1,
                       int nums2[], int len2) {
        //dp array: nums1的前i个元素与nums2的前j个元素最长公共子序列长度
        //dp[0,j],dp[i,0]都是0
        int** dp = new int*[2]; //只有两行
        for (int i = 0; i < 2; i++) {
            dp[i] = new int[len2+1]{0}; //初始化0，默认不为0
        }
        for (int i = 1; i <= len1; i++) {
            for (int j = 1; j <= len2; j++) {
                if (nums1[i-1] == nums2[j-1]) {
                    dp[i & 1][j] = dp[(i-1) & 1][j-1] + 1;
                } else {
                    dp[i & 1][j] = std::max(dp[i & 1][j-1], dp[(i-1) & 1][j]);
                }
            }
        }
        std::cout << "result = " << dp[len1 & 1][len2] << "\n";

        // free memory
        for (int i = 0; i < 2; i++) {
            delete[] dp[i];
        }
        delete[] dp;
    }

    // DP
    // 滚动数组
    // 用位运算替代取模操作
    // 只使用一行
    static void Run_V5(int nums1[], int len1,
                       int nums2[], int len2) {
        //dp array: nums1的前i个元素与nums2的前j个元素最长公共子序列长度
        //dp[0,j],dp[i,0]都是0
        int* dp = new int[len2+1]{0}; //只有1行，上一行的结果
        int left = 0, oldLeft = 0, oldTop = 0, oldLeftTop = 0;
        for (int i = 1; i <= len1; i++) {
            left = 0;
            for (int j = 1; j <= len2; j++) {
                oldLeftTop = oldTop;//上一行的[i-1][j-1]
                oldLeft = left;  //当前行[i][j-1]
                if (nums1[i-1] == nums2[j-1]) {
                    left = oldLeftTop + 1;
                } else {
                    left = std::max(oldLeft, dp[j]);
                }
                oldTop = dp[j];//更新dp数组之前，先要保留oldTop，因为下一次这个值作为oldLeftTop
                dp[j] = left;
            }
        }
        std::cout << "result = " << dp[len2] << "\n";

        // free memory
        delete[] dp;
    }

    // DP
    // 滚动数组
    // 用位运算替代取模操作
    // 只使用一行
    // 比V5更简洁
    static void Run_V6(int nums1[], int len1,
                       int nums2[], int len2) {
        //dp array: nums1的前i个元素与nums2的前j个元素最长公共子序列长度
        //dp[0,j],dp[i,0]都是0
        int* dp = new int[len2+1]{0}; //只有1行，上一行的结果
        int leftTop = 0, cur;
        for (int i = 1; i <= len1; i++) {
            cur = 0;
            for (int j = 1; j <= len2; j++) {
                leftTop = cur;//更新dp数组之前，先要保留oldTop，因为下一次这个值作为oldLeftTop
                cur = dp[j];
                if (nums1[i-1] == nums2[j-1]) {
                    dp[j] = leftTop + 1;
                } else {
                    dp[j] = std::max(dp[j-1], dp[j]);
                }
            }
        }
        std::cout << "result = " << dp[len2] << "\n";

        // free memory
        delete[] dp;
    }

    // DP
    // 滚动数组
    // 用位运算替代取模操作
    // 只使用一行
    // V6基础上进一步降低空间复杂度，用短的那一行开辟空间
    static void Run_V7(int nums1[], int len1,
                       int nums2[], int len2) {
        int *arr1 = nullptr, *arr2 = nullptr;
        int rows = 0, cols = 0;
        if (len1 > len2) {
            rows = len1;
            cols = len2;
            arr1 = nums1;
            arr2 = nums2;
        } else {
            rows = len2;
            cols = len1;
            arr1 = nums2;
            arr2 = nums1;
        }
        int* dp = new int[cols+1]{0}; //只有1行，上一行的结果
        int leftTop = 0, cur;
        for (int i = 1; i <= rows; i++) {
            cur = 0;
            for (int j = 1; j <= cols; j++) {
                leftTop = cur;//更新dp数组之前，先要保留oldTop，因为下一次这个值作为oldLeftTop
                cur = dp[j];
                if (arr1[i-1] == arr2[j-1]) {
                    dp[j] = leftTop + 1;
                } else {
                    dp[j] = std::max(dp[j-1], dp[j]);
                }
            }
        }
        std::cout << "result = " << dp[cols] << "\n";

        // free memory
        delete[] dp;
    }

    // DP
    // 滚动数组
    // 用位运算替代取模操作
    // 只使用一行
    // V6基础上进一步降低空间复杂度，用短的那一行开辟空间
    // V7基础上输出公共子序列 (TODO)
    // O(n*m), O(k)
    static void Run_V8(int nums1[], int len1,
                       int nums2[], int len2) {
        int *arr1 = nullptr, *arr2 = nullptr;
        int rows = 0, cols = 0;
        if (len1 > len2) {
            rows = len1;
            cols = len2;
            arr1 = nums1;
            arr2 = nums2;
        } else {
            rows = len2;
            cols = len1;
            arr1 = nums2;
            arr2 = nums1;
        }
        int* dp = new int[cols+1]{0}; //只有1行，上一行的结果
        int leftTop = 0, cur;
        for (int i = 1; i <= rows; i++) {
            cur = 0;
            for (int j = 1; j <= cols; j++) {
                leftTop = cur;//更新dp数组之前，先要保留oldTop，因为下一次这个值作为oldLeftTop
                cur = dp[j];
                if (arr1[i-1] == arr2[j-1]) {
                    dp[j] = leftTop + 1;
                } else {
                    dp[j] = std::max(dp[j-1], dp[j]);
                }
            }
        }
        std::cout << "result = " << dp[cols] << "\n";

        // free memory
        delete[] dp;
    }
};

// 最长公共子串, 连续的
class LongestCommonSubStr
{
public:
    //直接从LongestCommonSubSeq的Run_V8修改
    static void Run(const string& str1,
                    const string& str2) {
        int len1 = str1.length();
        int len2 = str2.length();
        const char *arr1 = nullptr, *arr2 = nullptr;
        int rows = 0, cols = 0;
        if (len1 > len2) {
            rows = len1;
            cols = len2;
            arr1 = str1.data();
            arr2 = str2.data();
        } else {
            rows = len2;
            cols = len1;
            arr1 = str2.data();
            arr2 = str1.data();
        }
        int* dp = new int[cols+1]{0}; //只有1行，上一行的结果
        int leftTop = 0, cur;
        int max = 0;
        int lastIndex = -1; // 记录最近最后一个公共字符的位置
        for (int i = 1; i <= rows; i++) {
            cur = 0;
            for (int j = 1; j <= cols; j++) {
                leftTop = cur;//更新dp数组之前，先要保留oldTop，因为下一次这个值作为oldLeftTop
                cur = dp[j];
                if (arr1[i-1] == arr2[j-1]) {
                    dp[j] = leftTop + 1;
                } else {
                    dp[j] = 0;
                }

                // 这里如果是>，则会返回最左边的最长公共子串
                // 这里如果是>=，则会返回最右边的最长公共子串
                if (dp[j] > max) {
                    max = dp[j];
                    lastIndex = i;
                }
            }
        }
        std::cout << "result = " << max << "\n";
        if (max > 0 && lastIndex >= 0 && lastIndex <= rows) {
            std::cout << "common substring is: ";
            for (int i = (lastIndex - max); i < lastIndex; i++) {
                std::cout << arr1[i];
            }
            std::cout << "\n";
        } else {
            std::cout <<"No common substring.\n";
        }

        // free memory
        delete[] dp;
    }

    //直接从LongestCommonSubSeq的Run_V8修改
    //改成从右往左算，主要看依赖于上一行的哪些数据
    static void Run_V2(const string& str1,
                       const string& str2) {
        int len1 = str1.length();
        int len2 = str2.length();
        const char *arr1 = nullptr, *arr2 = nullptr;
        int rows = 0, cols = 0;
        if (len1 > len2) {
            rows = len1;
            cols = len2;
            arr1 = str1.data();
            arr2 = str2.data();
        } else {
            rows = len2;
            cols = len1;
            arr1 = str2.data();
            arr2 = str1.data();
        }
        int* dp = new int[cols+1]{0}; //只有1行，上一行的结果
        int max = 0;
        int lastIndex = -1; // 记录最近最后一个公共字符的位置
        for (int i = 1; i <= rows; i++) {
            for (int j = cols; j >= 1; j--) {
                if (arr1[i-1] == arr2[j-1]) {
                    dp[j] = dp[j-1] + 1;
                } else {
                    dp[j] = 0;
                }

                // 这里如果是>，则会返回最左边的最长公共子串
                // 这里如果是>=，则会返回最右边的最长公共子串
                if (dp[j] > max) {
                    max = dp[j];
                    lastIndex = i;
                }
            }
        }
        std::cout << "result = " << max << "\n";
        if (max > 0 && lastIndex >= 0 && lastIndex <= rows) {
            std::cout << "common substring is: ";
            for (int i = (lastIndex - max); i < lastIndex; i++) {
                std::cout << arr1[i];
            }
            std::cout << "\n";
        } else {
            std::cout <<"No common substring.\n";
        }

        // free memory
        delete[] dp;
    }
};


// 01背包问题，DP
class Knapsack_DP
{
public:
    // 求背包能装的最大价值
    static void Run(int values[],  //物品价值
                    int weights[], //物品重量
                    int length,    //物品数量
                    int capacity   //背包容量
                    ) {
        // dp(i,j):最大承重为j,有前i件物品可选的最大价值
        // dp(i,0) = 0 没有背包，没有价值
        // dp(0,j) = 0 没有物品可选，没有价值
        // 如果第i件物品重量超过了容量，j < weights[i-1], dp(i,j) = dp(i-1,j)
        // 如果不选择第i件物品：dp(i,j) = dp(i-1, j)
        // 如果选择第i件物品：dp(i,j) = values[i-1] + dp(i-1, j-weights[i-1])
        // dp(i,j) = max[dp(i-1, j), values[i-1] + dp(i-1, j-weights[i-1])]

        if (capacity < 1) return;

        int **dp = new int*[length + 1];
        for (int i = 0; i < length+1; i++) {
            dp[i] = new int[capacity + 1]{0};
        }

        for (int i = 1; i < length+1; i++) {
            for (int j = 1; j < capacity+1; j++) {
                if (j < weights[i-1]) { // 最后一件物品放不下了
                    dp[i][j] = dp[i-1][j];
                } else {
                    dp[i][j] = std::max(dp[i-1][j], values[i-1] + dp[i-1][j - weights[i-1]]);
                }
            }
        }

        std::cout << "result = " << dp[length][capacity] << "\n";
        // print dp array
        for (int i = 0; i < length+1; i++) {
            for (int j = 0; j < capacity+1; j++) {
                std::cout << dp[i][j] << ", ";
            }
            std::cout << "\n";
        }

        // free memory
        for (int i = 0; i < length+1; i++) {
            delete[] dp[i];
        }
        delete[] dp;
    }

    // 降低空间复杂度
    static void Run_V2(int values[],  //物品价值
                       int weights[], //物品重量
                       int length,    //物品数量
                       int capacity   //背包容量
                       ) {
        if (capacity < 1) return;

        int *dp = new int[capacity + 1]{0};
        for (int i = 1; i < length+1; i++) {
            for (int j = capacity; j>=1; j--) { //从右向左
                if (j < weights[i - 1]) continue;
                dp[j] = std::max(dp[j],
                                 values[i-1] + dp[j - weights[i-1]]);
            }
        }
        std::cout << "result = " << dp[capacity] << "\n";

        delete[] dp;

    }

    // 降低空间复杂度
    static void Run_V3(int values[],  //物品价值
                       int weights[], //物品重量
                       int length,    //物品数量
                       int capacity   //背包容量
                       ) {
        if (capacity < 1) return;

        int *dp = new int[capacity + 1]{0};
        for (int i = 1; i < length+1; i++) {
            for (int j = capacity; j>=weights[i - 1]; j--) { //从右向左
                dp[j] = std::max(dp[j], values[i-1] + dp[j - weights[i-1]]);
            }
        }
        std::cout << "result = " << dp[capacity] << "\n";

        delete[] dp;

    }

    // 降低空间复杂度
    // 恰好装满
    static void Run_V4(int values[],  //物品价值
                       int weights[], //物品重量
                       int length,    //物品数量
                       int capacity   //背包容量
                       ) {
        if (capacity < 1) return;

        //int *dp = new int[capacity + 1]{INT_MIN}; // 第一个元素初始化为INT_MIN，后面全0
        int *dp = new int[capacity + 1]{0}; // 初始化为全0
        for (int i = 1 ; i < capacity+1; i++) {
            dp[i] = INT_MIN;
        }

        for (int i = 1; i < length+1; i++) {
            for (int j = capacity; j>=weights[i - 1]; j--) { //从右向左
                dp[j] = std::max(dp[j], values[i-1] + dp[j - weights[i-1]]);
            }
        }
        std::cout << "result = " << (dp[capacity] < 0 ? -1 : dp[capacity]) << "\n";

        delete[] dp;

    }
};

}





#endif // MYPROBLEMS_H
