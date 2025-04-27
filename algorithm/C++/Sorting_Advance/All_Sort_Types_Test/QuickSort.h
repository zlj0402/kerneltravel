//
// Created by liangj.zhang on 16/4/2025, last updated on 27/4/2025
// updated on 27/4/2025: add annotation for quickSort2Ways comparasion condition
//

#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <stdlib.h>
#include <ctime>
#include "InsertionSort.h"

// >>>>>>>>>>>>>>>>>>>>> 最经典基础的 原地递归快排： <<<<<<<<<<<<<<<<<<<<<<<<
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

// >>>>>>>>>>>>>>>>>>>>> 随机化 + 双路快速排序 <<<<<<<<<<<<<<<<<<<<<<<<

// 双路快速排序的partition
// 对arr[l...r]部分进行partition操作
// 返回p, 使得arr[l+1...i] =< arr[p] ; arr[j...r] >= arr[p]
template<typename T>
int __partition3(T arr[], int l, int r) {

	// 随机在arr[l...r]的范围中, 选择一个数值作为标定点pivot
	swap(arr[l], arr[rand() % (r - l + 1) + l]);
	T v = arr[l];

	// arr[l+1...i) <= v; arr(j...r] >= v
	int i = l + 1 , j = r;
	while (true) {

		while (i <= r && arr[i] < v) i++;
		while (j >= l + 1 && arr[j] > v) j--;	// 不太正确的写法：while (j >= l + 1 && arr[j] >= v)，加上 ==，还是有可能会爆栈；双路随机，本就是为了解决随机快排的爆站问题；
		if (i >= j) break;
		swap(arr[i++], arr[j--]);
	}
	swap(arr[l], arr[j]);	// swap(arr[l], arr[i]); 是有可能跟arr[r+1] -> arr[n - 1 + 1] -> arr[n]交换的;

	return j;
}

// 对arr[l...r]部分进行快速排序
template<typename T>
void __quickSort3(T arr[], int l, int r) {

	//if (l >= r)
	//	return;
	if (r - l <= 15) {
		insertionSort(arr, l, r);
		return;
	}

	int p = __partition3(arr, l, r);
	__quickSort3(arr, l, p - 1);
	__quickSort3(arr, p + 1, r);
}

template<typename T>
void quickSort3(T arr[], int n) {

	srand(time(NULL));
	__quickSort3(arr, 0, n - 1);
}

// >>>>>>>>>>>>>>>>>>>>> 随机化 + 三路快速排序 <<<<<<<<<<<<<<<<<<<<<<<<

// 对arr[l...r]部分进行快速排序
template<typename T>
void __quickSort4(T arr[], int l, int r) {

	//if (l >= r)
	//	return;
	if (r - l <= 15) {
		insertionSort(arr, l, r);
		return;
	}

	// partition
	// 随机在arr[l...r]的范围中, 选择一个数值作为标定点pivot
	swap(arr[l], arr[rand() % (r - l + 1) + l]);
	int v = arr[l];

	int lt = l;		// [l+1, lt] < v  after swap => [l, lt] < v
	int gt = r + 1;	// [gt, r] > v
	int i = lt + 1;	// [lt+1, i) == v

	while (i < gt) {

		if (arr[i] < v) {
			swap(arr[i], arr[lt + 1]);
			lt++;
			i++;
			//swap(arr[i++], arr[++lt]);	// 一步搞定
		}
		else if (arr[i] > v) {
			swap(arr[i], arr[gt - 1]);
			gt--;
			//swap(arr[i], arr[--gt]);	// 一步搞定
		}
		else {
			i++;
		}
	}

	swap(arr[l], arr[lt--]);

	__quickSort4(arr, l, lt);
	__quickSort4(arr, gt, r);
}

template<typename T>
void quickSort4(T arr[], int n) {

	srand(time(NULL));
	__quickSort4(arr, 0, n - 1);
}

#endif	//QUICK_SORT_H
