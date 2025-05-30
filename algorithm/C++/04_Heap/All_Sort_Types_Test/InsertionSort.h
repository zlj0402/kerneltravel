//
// Created by liangj.zhang on 13/4/2025
// 

#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

template<typename T>
void insertionSort(T arr[], int n) {

    for (int i = 1; i < n; i++) {

        T e = arr[i];
        int j; // j保存元素e应该插入的位置
        for (j = i; j > 0 && arr[j - 1] > e; j--)
            arr[j] = arr[j - 1];
        arr[j] = e;
    }

    return;
}

template<typename T>
void insertionSort(T arr[], int l, int r) {

    for (int i = l + 1; i <= r; i++) {
        
        T e = arr[i];
        int j;
        for (j = i; j > l && arr[j - 1] > e; j--)
            arr[j] = arr[j - 1];
        arr[j] = e;
    }

    return;
}

#endif	//INSERTION_SORT_H
