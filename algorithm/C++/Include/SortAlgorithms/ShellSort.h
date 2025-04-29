//
// Created by liangj.zhang	12/4/2025
//

#ifndef SHELLSORT_H
#define SHELLSORT_H

template<typename T>
void shell(T[], int, int);

template<typename T>
void shellSort(T arr[], int n) {

	int gap = n;
	while (gap > 1) {

		gap /= 2;
		shell(arr, n, gap);
	}
}

template<typename T>
void shell(T arr[], int n, int gap) {

	for (int i = gap; i < n; i++) {
		
		T tmp = arr[i];
		int j = i - gap;
		for (; j >= 0 && tmp < arr[j]; j -= gap) 
				arr[j + gap] = arr[j];	// 原来错误的写法：arr[i] = arr[j]; 上一位是j + gap
		arr[j + gap] = tmp;
	}
}

// reference to lyb git

template<typename T>
void shellSort2(T arr[], int n) {

    // 计算 increment sequence: 1, 4, 13, 40, 121, 364, 1093...
    int h = 1;
    while (h < n / 3)
        h = 3 * h + 1;

    while (h >= 1) {

        // h-sort the array
        for (int i = h; i < n; i++) {

            // 对 arr[i], arr[i-h], arr[i-2*h], arr[i-3*h]... 使用插入排序
            T e = arr[i];
            int j;
            for (j = i; j >= h && e < arr[j - h]; j -= h)
                arr[j] = arr[j - h];
            arr[j] = e;
        }

        h /= 3;
    }
}

#endif	//SHELLSORT_H
