//
// Created by liangj.zhang on 16/4/2025
//

#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <stdlib.h>
#include <ctime>
#include "InsertionSort.h"

// >>>>>>>>>>>>>>>>>>>>> 最经典基础的 原地递归快排 <<<<<<<<<<<<<<<<<<<<<<<<
// 对arr[l...r]部分进行partition操作
// 返回p, 使得arr[l...p-1] < arr[p] ; arr[p+1...r] > arr[p]
template<typename T>
int __partition(T arr[], int l, int r) {

	T e = arr[l];
	int j = l;
	for (int i = l + 1; i <= r; i++) {
		
		if (arr[i] < e) {
			j++;	// j -> 小于部分的最右端; j+1 -> 大于部分的最左端;
			swap(arr[j], arr[i]);	// 交换一下，发现小值的位置和大于部分最左端的值，那么交换后，j又是小端最右边值
		}
	}
	swap(arr[j], arr[l]);	// 原来错误的写法：swap(arr[j], e);

	return j;
}

// 对arr[l...r]部分进行快速排序
template<typename T>
void __quickSort(T arr[], int l, int r) {

	if (l >= r)
		return;

	int p = __partition(arr, l, r);
	__quickSort(arr, l, p - 1);
	__quickSort(arr, p + 1, r);
}

template<typename T>
void quickSort(T arr[], int n) {
	__quickSort(arr, 0, n - 1);
}

// >>>>>>>>>>>>>>>>>>>>> 随机化快速排序 <<<<<<<<<<<<<<<<<<<<<<<<
// 对arr[l...r]部分进行partition操作
// 返回p, 使得arr[l...p-1] < arr[p] ; arr[p+1...r] > arr[p]
template<typename T>
int __partition2(T arr[], int l, int r) {
	
	swap(arr[l], arr[rand() % (r - l + 1) + l]);

	T e = arr[l];
	int j = l;
	for (int i = l + 1; i <= r; i++) {

		if (arr[i] < e) {
			j++;	// j -> 小于部分的最右端; j+1 -> 大于部分的最左端;
			swap(arr[j], arr[i]);	// 交换一下，发现小值的位置和大于部分最左端的值，那么交换后，j又是小端最右边值
		}
	}
	swap(arr[j], arr[l]);	// 原来错误的写法：swap(arr[j], e);

	return j;
}

// 对arr[l...r]部分进行快速排序
template<typename T>
void __quickSort2(T arr[], int l, int r) {

	//if (l >= r)
	//	return;
	if (r - l <= 15) {
		insertionSort(arr, l, r);
		return;
	}

	int p = __partition2(arr, l, r);
	__quickSort2(arr, l, p - 1);
	__quickSort2(arr, p + 1, r);
}

template<typename T>
void quickSort2(T arr[], int n) {

	srand(time(NULL));
	__quickSort2(arr, 0, n - 1);
}

#endif	//QUICK_SORT_H
