#ifndef MYSORT_H
#define MYSORT_H

// 十大排序算法

#include "MyCommon.h"
#include <iostream>
#include <vector>

namespace sort_algorithms {

void PrintArray(int arr[], int length) {
    for(int i = 0; i < length; i++) {
        std::cout << arr[i] << ", ";
    }
    std::cout << "\n";
}

void BubbleSortV1(int arr[], int length) {
    int i,j;
    for(i=length-1; i>0; i--) {
        for(j=1; j<=i; j++) {
            if (arr[j] < arr[j-1]) {
                int tmp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = tmp;
            }
        }
    }
}

void BubbleSortV2(int arr[], int length) {
    int i,j;
    bool sorted = false;
    for(i=length-1; i>0; i--) {
        sorted = true;      //记录当前循环中是否已经排好序了，从前到后
        for(j=1; j<=i; j++) {
            if (arr[j] < arr[j-1]) {
                int tmp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = tmp;
                sorted = false;
            }
        }
        if (sorted) {
            break;
        }
    }
}

void BubbleSortV3(int arr[], int length) {
    int i,j;
    for(i=length-1; i>0; i--) {
        int lastIndex = 1;   //记录最后一次比较的地址，减少比较次数，从后到前
        for(j=1; j<=i; j++) {
            if (arr[j] < arr[j-1]) {
                int tmp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = tmp;
                lastIndex = j;
            }
        }
        i = lastIndex;
    }
}

void SelectionSortV1(int arr[], int length) {
    int i, j;
    int maxInd;
    for(i = length-1; i > 1; i--) {
        maxInd = i;  //默认最后一个元素为最大值
        for(j = 0; j < i; j++) {  //从第一个位置遍历到最后一个位置的前一个位置
            if (arr[j] > arr[maxInd]) {  //如果前面的数组中有元素比最大元素大，记录其位置
                maxInd = j;
            }
        }

        //将最后一个元素和最大元素进行交换
        int tmp = arr[i];
        arr[i] = arr[maxInd];
        arr[maxInd] = tmp;
    }
}


template <typename E>
class Sort {
public:
    E* data;
    int length;
    int cmpCount;
    int swapCount;

    Sort(E *unsorted_data, int data_length) {
        //std::cout << "Sort::Sort()\n";
        data = new E[data_length];
        length = data_length;
        cmpCount = 0;
        swapCount = 0;

        for(int i = 0; i < length; i++) {
            data[i] = unsorted_data[i];
        }
    }
    virtual ~Sort() {
        //std::cout << "Sort::~Sort()\n";
        delete[] data;
    }

    int Compare(int i1, int i2) {
        cmpCount++;
        return data[i1] - data[i2];
    }

    int CompareElement(E e1, E e2) {
        cmpCount++;
        return e1 - e2;
    }

    void Swap(int i1, int i2) {
        swapCount++;
        E tmp = data[i1];
        data[i1] = data[i2];
        data[i2] = tmp;
    }

    virtual void DoSort() = 0;

    void DoPrint() {
        for(int i = 0; i < length; i++) {
            std::cout << data[i] << ", ";
        }
        std::cout << "\n";
    }
};

template <typename E>
class BubbleSort : public Sort<E> {
public:
    BubbleSort(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {
        DoSort();
        this->DoPrint();
    }

    void DoSort() {
        int i,j;
        for(i=this->length-1; i>0; i--) {
            int lastIndex = 1;   //记录最后一次比较的地址，减少比较次数，从后到前
            for(j=1; j<=i; j++) {
                if (this->Compare(j, j-1) < 0) {
                    this->Swap(j, j-1);
                    lastIndex = j;
                }
            }
            i = lastIndex;
        }

        std::cout << "CmpCount: " << this->cmpCount << "\n";
        std::cout << "SwapCount: " << this->swapCount << "\n";
    }
};

template <typename E>
class SelectionSort : public Sort<E> {
public:
    SelectionSort(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {
        //std::cout << "SelectionSort::SelectionSort()\n";
        DoSort();
        this->DoPrint();
    }

    void DoSort() {
        std::cout << "DoSort\n";
        int i, j;
        int maxInd;
        for(i = this->length-1; i > 1; i--) {
            maxInd = i;  //默认最后一个元素为最大值
            for(j = 0; j < i; j++) {  //从第一个位置遍历到最后一个位置的前一个位置
                if (this->Compare(j, maxInd) > 0) {  //如果前面的数组中有元素比最大元素大，记录其位置
                    maxInd = j;
                }
            }
            //将最后一个元素和最大元素进行交换
            this->Swap(i, maxInd);
        }

        std::cout << "CmpCount: " << this->cmpCount << "\n";
        std::cout << "SwapCount: " << this->swapCount << "\n";
    }
};


template <typename E>
class HeapSort : public Sort<E> {
public:
    int heapSize;

    HeapSort(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {
        DoSort();
        this->DoPrint();
    }

    void DoSort() {
        heapSize = this->length;

        //原地建堆
        for (int index = (heapSize >> 1) - 1; index >= 0; index--) {
            SiftDown(index);
        }

        //将堆顶元素和最后一个元素交换，heapSize减1，对第一个元素进行下滤操作
        while (heapSize > 1) {
            this->Swap(0, --heapSize);
            SiftDown(0); //恢复堆的性质

        }

        std::cout << "CmpCount: " << this->cmpCount << "\n";
        std::cout << "SwapCount: " << this->swapCount << "\n";
    }

    void SiftDown(int index) {
        E element = this->data[index];
        int half = heapSize >> 1;
        while (index < half) {//将比index处元素大的元素不断上移，直到找到一个合适的插入位置
            int childIndex = (index << 1) + 1;
            E child = this->data[childIndex];

            int rightIndex = childIndex + 1;
            if (rightIndex < heapSize && this->CompareElement(this->data[rightIndex], child) > 0) {
                childIndex = rightIndex;
                child = this->data[childIndex];
            }

            if (this->CompareElement(element, child) >= 0) break;
            this->data[index] = child;
            index = childIndex;
        }
        this->data[index] = element;
    }
};

template <typename E>
class InsertionSortV1 : public Sort<E> {
public:
    InsertionSortV1(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {
        DoSort();
        this->DoPrint();
    }

    void DoSort() {
        int begin;
        int cur;
        for (begin = 1; begin < this->length; begin++) {
            cur = begin;
            while (cur>0 && this->Compare(cur, cur-1) < 0) {
                this->Swap(cur, cur-1);
                cur--;
            }
        }
        std::cout << "CmpCount: " << this->cmpCount << "\n";
        std::cout << "SwapCount: " << this->swapCount << "\n";
    }
};

template <typename E>
class InsertionSortV2 : public Sort<E> {
public:
    InsertionSortV2(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {
        DoSort();
        this->DoPrint();
    }

    void DoSort() {
        int begin;
        int cur;
        for (begin = 1; begin < this->length; begin++) {
            cur = begin;
            E v = this->data[cur];
            while (cur>0 && this->CompareElement(v, this->data[cur-1]) < 0) {
                this->data[cur] = this->data[cur-1];
                cur--;
            }
            this->data[cur] = v;
        }
        std::cout << "CmpCount: " << this->cmpCount << "\n";
        std::cout << "SwapCount: " << this->swapCount << "\n";
    }
};

template <typename E>
class InsertionSortV3 : public Sort<E> {
public:
    InsertionSortV3(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {
        DoSort();
        this->DoPrint();
    }

    void DoSort() {
        int begin;
        int cur;
        for (begin = 1; begin < this->length; begin++) {
            cur = begin;
            E v = this->data[cur];
            int pos = binary_search(0, begin, v);  //先查找排好序数组中第一个比当前元素大的位置
            int len = begin - pos;  //右边需要挪动的长度
            while (len > 0) {
                this->data[cur] = this->data[cur-1]; //向右挪动
                len--;
                cur--;
            }
            this->data[cur] = v;
        }
        std::cout << "CmpCount: " << this->cmpCount << "\n";
        std::cout << "SwapCount: " << this->swapCount << "\n";
    }

    int binary_search(int begin, int end, const E& element) {
        //在[begin,end)范围内查找第一个比element大的元素的位置
        while (begin < end) {
            int mid = (begin + end) / 2;
            int cmp = this->CompareElement(element, this->data[mid]);
            if (cmp >= 0) {
                begin = mid + 1;
            } else if (cmp < 0) {
                end = mid;
            }
        }
        return begin;
    }
};


template <typename E>
class MergeSort : public Sort<E> {
public:

    E* leftArray;

    MergeSort(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {
        leftArray = new E[data_length >> 1];

        DoSort();
        this->DoPrint();
    }

    virtual ~MergeSort() {
        delete[] leftArray;
    }

    void DoSort() {
        DoSort(0, this->length); //左闭右开

        std::cout << "CmpCount: " << this->cmpCount << "\n";
        std::cout << "SwapCount: " << this->swapCount << "\n";
    }

    void DoSort(int begin, int end) {
        if (end - begin < 2) return;

        int mid = (begin + end) >> 1;
        DoSort(begin, mid);
        DoSort(mid, end);
        Merge(begin, mid, end);
    }

    //对[begin,mid), [mid,end)进行合并
    void Merge(int begin, int mid, int end) {
        int li = 0, le = mid - begin;
        int ri = mid, re = end;
        int ai = begin;

        // 备份左边数组
        for (int i = li; i < le; i++) {
            leftArray[i] = this->data[begin + i];
        }

        //如果左边还没有结束
        while (li < le) {
            if (ri < re && this->CompareElement(this->data[ri], leftArray[li]) < 0) { //<=0会失去稳定性
                //如果右边还没结束，且左边的小一些
                this->data[ai++] = this->data[ri++];
            } else {
                this->data[ai++] = leftArray[li++];
            }
        }

    }

};



template <typename E>
class QuickSort : public Sort<E> {
public:

    QuickSort(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {

        DoSort();
        this->DoPrint();
    }


    void DoSort() {
        DoSort(0, this->length); //左闭右开

        std::cout << "CmpCount: " << this->cmpCount << "\n";
        std::cout << "SwapCount: " << this->swapCount << "\n";
    }

    // 对[begin,end)进行排序
    void DoSort(int begin, int end) {
        if (end - begin < 2) return;

        //确定轴点位置 O(n)
        int mid = PivotIndex(begin, end);
        //对子序列进行排序
        DoSort(begin, mid);
        DoSort(mid+1, end);//不包括轴点
    }

    int PivotIndex(int begin, int end) {
        //随机选择一个位置与begin位置元素交换（改善性能）
        this->Swap(begin, begin + (int)((end - begin) * rand() / RAND_MAX));

        //备份begin位置元素
        E pivot = this->data[begin];
        end--;

        while (begin < end) {
            while (begin < end) {
                if (this->CompareElement(pivot, this->data[end]) < 0) {//右边元素大于轴点元素，这里如果写等于，会出现分割不均匀情况
                    end--;
                } else {//右边元素<=pivot
                    this->data[begin++] = this->data[end];
                    break; //换方向
                }
            }

            while (begin < end) {
                if (this->CompareElement(pivot, this->data[begin]) > 0) {//左边元素比轴点小
                    begin++;
                } else {//左边元素>=轴点元素
                    this->data[end--] = this->data[begin];
                    break;//换方向
                }
            }
        }

        this->data[begin] = pivot;

        return begin;
    }
};

template <typename E>
class ShellSort : public Sort<E> {//不稳定排序
public:

    ShellSort(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {

        DoSort();
        this->DoPrint();
    }


    void DoSort() {

        std::vector<int> steps = ShellStepSequence();
        for (int& step : steps) {
            DoSort(step);
        }

        std::cout << "CmpCount: " << this->cmpCount << "\n";
        std::cout << "SwapCount: " << this->swapCount << "\n";
    }

    void DoSort(int step) {
        //col，第几列
        for (int col = 0; col < step; col++) {  //与插入排序很类似
            for (int begin = col + step; begin < this->length; begin += step) {
                int cur = begin;
                while (cur > col && this->Compare(cur, cur - step) < 0) {
                    this->Swap(cur, cur - step);
                    cur -= step;
                }
            }
        }
    }

    std::vector<int> ShellStepSequence() {
        std::vector<int> steps;
        int step = this->length;
        while ((step >>= 1) > 0) {
            steps.push_back(step);
        }
        return steps;
    }
};



template <typename E>
class CountingSort : public Sort<E> {  //对整数排序
public:

    CountingSort(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {

        DoSort();
        this->DoPrint();
    }

    void DoSort() {  //改进版本
        //找出最小最大值
        int min = this->data[0];
        int max = min;
        for (int i = 1; i < this->length; i++) {
            if (this->data[i] > max) {
                max = this->data[i];
            }
            if (this->data[i] < min) {
                min = this->data[i];
            }
        }

        //存储每个元素的出现次数
        int* counts = new int[1 + max - min]{0};
        for (int i = 0; i < this->length; i++) {
            counts[this->data[i] - min]++;
        }
        for (int i = 1; i < 1 + max - min; i++) {
            counts[i] += counts[i-1];
        }

        int *newArray = new int[this->length];
        for (int i = this->length-1; i>=0; i--) {
            --counts[this->data[i]-min];
            newArray[counts[this->data[i]-min]] = this->data[i];  //保证稳定性
        }

        for (int i = 0; i < this->length; i++) {
            this->data[i] = newArray[i];
        }
        delete[] newArray;
    }

    void DoSort0() {
        //无法处理负数，分配空间比较高，是不稳定排序

        //找出最大值
        int max = this->data[0];
        for (int i = 1; i < this->length; i++) {
            if (this->data[i] > max) {
                max = this->data[i];
            }
        }

        //开辟内存空间，存储每个元素出现的次数
        int* counts = new int[1 + max]{0}; //必须初始化为0，默认是没有初始化的
        for (int i = 0; i < this->length; i++) {
            counts[this->data[i]]++;
        }

        //从小到大输出
        int index = 0;
        for (int i = 0; i < max; i++) {
            while (counts[i]-- > 0) {
                this->data[index++] = i;
            }
        }

        delete[] counts;

        std::cout << "CmpCount: " << this->cmpCount << "\n";
        std::cout << "SwapCount: " << this->swapCount << "\n";
    }
};


template <typename E>
class RadixSort : public Sort<E> {   //对整数排序   基数排序，先对个位排序，再对十位，百位，。。。
public:

    RadixSort(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {

        DoSort();
        this->DoPrint();
    }

    void DoSort() {

        //求最大值
        int max = this->data[0];
        for (int i = 1; i < this->length; i++) {
            if (this->data[i] > max) {
                max = this->data[i];
            }
        }

        for (int divider = 1; divider <= max; divider*=10) {
            DoCountingSort(divider);
        }

    }

    void DoCountingSort(int divider)  {
        int* counts = new int[10]{0};
        for (int i = 0; i < this->length; i++) {
            counts[this->data[i] / divider % 10]++;
        }
        for (int i = 1; i < 10; i++) {
            counts[i] += counts[i - 1];
        }
        int* newArray = new int[this->length];
        for(int i = this->length-1; i>=0; i--) {
            newArray[--counts[this->data[i] / divider % 10]] = this->data[i];
        }
        for(int i=0; i<this->length; i++) {
            this->data[i] = newArray[i];
        }
        delete[] newArray;
    }

};


template <typename E>
class BucketSort : public Sort<E> {   //桶排序
public:

    BucketSort(E* unsorted_data, int data_length) : Sort<E>(unsorted_data, data_length) {

        DoSort();
        this->DoPrint();
    }

    void DoSort() {

    }
};

}

#endif // MYSORT_H
