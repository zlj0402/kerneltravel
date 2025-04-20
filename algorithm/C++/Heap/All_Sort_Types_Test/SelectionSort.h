//
// Reference by liuyubobobo -- ported on 11/4/2025
//

#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H

#include <algorithm>

// 这已经是一个优化版的，后面那个更好;
template<typename T>
void selectionSort(T arr[], int n) {

    for (int i = 0; i < n; i++) {

        int minIndex = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[minIndex])
                minIndex = j;

        swap(arr[i], arr[minIndex]);
    }
}

// optimized selection sort
// 感谢github @zhengquan45 提出, 可以对选择排序进行优化
// 在每一轮中, 可以同时找到当前未处理元素的最大值和最小值
template<typename T>
void selectionSort2(T arr[], int n) {

    int left = 0, right = n - 1;
    while (left < right) {
        int minIndex = left;
        int maxIndex = right;

        // 在每一轮查找时, 要保证arr[minIndex] <= arr[maxIndex]
        if (arr[minIndex] > arr[maxIndex])
            swap(arr[minIndex], arr[maxIndex]);

        for (int i = left + 1; i < right; i++)
            if (arr[i] < arr[minIndex])
                minIndex = i;
            else if (arr[i] > arr[maxIndex])
                maxIndex = i;

        swap(arr[left], arr[minIndex]);
        swap(arr[right], arr[maxIndex]);

        left++;
        right--;
    }

    return;
}

#endif //SELECTIONSORT_H
