//
// Created by liangj.zhang on 13/4/2025 15/4/2025_BU
//

#include "InsertionSort.h"

// 将arr[l...mid]和arr[mid+1...r]两部分进行归并
template<typename T>
void __merge(T arr[], int l, int mid, int r) {

	T *tmp = new T[r - l + 1];		// 原来错误的写成 r - 1 + 1了，不是l，我明明没有照抄，自己回忆的，不知道什么时候写错了;
	for (int i = l; i <= r; i++)
		tmp[i - l] = arr[i];

	int i = 0;
	int j = mid - l + 1;
	for (int k = l; k <= r; k++) {
		
		// 如果左半部分元素已经全部处理完毕
		if (i > mid - l) {			// 原来错误的写法：i > mid
			arr[k] = tmp[j++];
		} // 如果右半部分元素已经全部处理完毕
		else if (j > r - l) {		// 原来错误的写法：j > r
			arr[k] = tmp[i++];
		} // 左半部分所指元素 < 右半部分所指元素
		else if (tmp[i] < tmp[j]) {
			arr[k] = tmp[i++];
		} // 左半部分所指元素 >= 右半部分所指元素
		else {
			arr[k] = tmp[j++];
		}
	}
	delete[] tmp;
}

// 递归使用归并排序,对arr[l...r]的范围进行排序
template<typename T>
void __mergeSort(T arr[], int l, int r) {
	
	if (l >= r)
		return;

	int mid = (l + r) / 2;
	__mergeSort(arr, l, mid);
	__mergeSort(arr, mid + 1, r);
	__merge(arr, l, mid, r);
}

template<typename T>
void mergeSort(T arr[], int n) {
	__mergeSort(arr, 0, n-1);
}

// 递归使用归并排序,对arr[l...r]的范围进行排序
template<typename T>
void __mergeSort2(T arr[], int l, int r) {

	//if (l >= r)
	//	return;
	if (r - l <= 15) {
		insertionSort(arr, l, r);
		return;
	}

	int mid = (l + r) / 2;
	__mergeSort(arr, l, mid);
	__mergeSort(arr, mid + 1, r);
	if (arr[mid] > arr[mid + 1])	// 对近乎有序的数组有效; arr[mid]是左边最大，arr[mid+1]是右边最小;
		__merge(arr, l, mid, r);
}

template<typename T>
void mergeSort2(T arr[], int n) {
	__mergeSort2(arr, 0, n - 1);
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>> 自底向上的归并排序 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

template<typename T>
void mergeSortBU(T arr[], int n) {

	for (int sz = 1; sz < n; sz += sz) {
		
		// for (int i = 0; i < n; i += sz + sz) {
		// i + sz < n，右半部分才有数据; 而左半部分是上一次排好序的（子左子右），已经不需要排序了;
		for (int i = 0; i + sz < n; i += sz + sz) {	

			if (arr[i + sz - 1] > arr[i + sz])
				// 对arr[i...i+sz-1] 和 arr[i+sz...i+2*sz-1] 进行归并
				__merge(arr, i, i + sz - 1, min(i + sz + sz - 1, n - 1));
		}
	}
}
