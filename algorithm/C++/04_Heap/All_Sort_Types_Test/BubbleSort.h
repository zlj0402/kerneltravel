//
//	Created by liangj.zhang on 12/4/2025
//

#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

// 目前融合最多版本，！！！这一版就够了！！！; 后面的留着，是作为写法参考
// 融合了lyb的两个剪枝：swapped, newn
// 自己独有的剪枝：像直接插入排序一样，直到找到合适的位置，才会值插入; 不是每次交换赋值;
template<typename T>
void bubbleSort(T arr[], int n) {

    for (int j = n - 1; j > 0; j--) {

        bool swapped = false;
        int newn = 0;
        T tmp = arr[0];
        for (int i = 0; i < j; i++) {

            if (tmp > arr[i + 1]) {
                arr[i] = arr[i + 1];
                swapped = true;
                newn = i;
            }
            else {
                if (tmp ^ arr[i])   // 内存写比异或操作更耗时; 少重复写：尤其是对接近有序的序列排序;
                    arr[i] = tmp;       // 原来漏写了; 找到更大的了，先落地，再换成大的; 3 1 5 2 6 3 -> 1往前移动一位，3是tmp，找到更大的5，先让3落地在arr[i];
                tmp = arr[i + 1];   // 原来错误的写成: tmp = arr[i];
            }
        }
        arr[j] = tmp;
        j = newn + 1;

        if (!swapped)   // 防止排序了一个有序序列; 同时也可以在做一个剪枝操作，提前结束;
            break;
    }
}

// 最基础的冒泡
template<typename T>
void bubbleSort_base(T arr[], int n) {

    for (int j = n - 1; j > 0; j--) {

        for (int i = 0; i < j; i++) {

            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i+1]);
            }
        }
    }
}

// reference to liuyubo git

// 我们的第一版bubbleSort
template<typename T>
void bubbleSort_lyb(T arr[], int n) {

    bool swapped;

    do {
        swapped = false;
        for (int i = 1; i < n; i++)
            if (arr[i - 1] > arr[i]) {
                swap(arr[i - 1], arr[i]);
                swapped = true;

            }

        // 优化, 每一趟Bubble Sort都将最大的元素放在了最后的位置
        // 所以下一次排序, 最后的元素可以不再考虑
        n--;

    } while (swapped);
}


// 我们的第二版bubbleSort,使用newn进行优化
template<typename T>
void bubbleSort_lyb2(T arr[], int n) {

    int newn; // 使用newn进行优化

    do {
        newn = 0;
        for (int i = 1; i < n; i++)
            if (arr[i - 1] > arr[i]) {
                swap(arr[i - 1], arr[i]);

                // 记录最后一次的交换位置,在此之后的元素在下一轮扫描中均不考虑
                newn = i;
            }
        n = newn;
    } while (newn > 0);
}

#endif	//BUBBLE_SORT_H
