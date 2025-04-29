//
// Created by liangj.zhang on 21/4/2025
//

#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "Heap.h"

// heapSort, 将所有的元素依次添加到堆中, 在将所有元素从堆中依次取出来, 即完成了排序
// 无论是创建堆的过程, 还是从堆中依次取出元素的过程, 时间复杂度均为O(nlogn)
// 整个堆排序的整体时间复杂度为O(nlogn)
template<typename T>
void heapSort(T arr[], int n) {

	MaxHeap<T> maxheap = MaxHeap<T>(n);
	for (int i = 0; i < n; i++)
		maxheap.insert(arr[i]);

	for (int i = n - 1; i >= 0; i--)
		arr[i] = maxheap.extractMax();
}

// heapSort2, 借助我们的heapify过程创建堆
// 此时, 创建堆的过程时间复杂度为O(n), 将所有元素依次从堆中取出来, 实践复杂度为O(nlogn)
// 堆排序的总体时间复杂度依然是O(nlogn), 但是比上述heapSort性能更优, 因为创建堆的性能更优
template<typename T>
void heapSort2(T arr[], int n) {
	
	MaxHeap<T> maxheap = MaxHeap<T>(arr, n);
	for (int i = n - 1; i >= 0; i--)
		arr[i] = maxheap.extractMax();
}

// 不使用一个额外的最大堆, 直接在原数组上进行原地的堆排序
template<typename T>
void heapSort3(T arr[], int n) {
	
	// 注意，此时我们的堆是从0开始索引的
	// 从(最后一个元素的索引-1)/2开始
	// 最后一个元素的索引 = n-1
	for (int i = (n - 1 - 1) >> 1; i >= 0; i--) 
		MaxHeap<T>::shiftDown_idx0(arr, n, i);

	for (int i = n - 1; i > 0; i--) {

		swap(arr[0], arr[i]);
		MaxHeap<T>::shiftDown_idx0(arr, i, 0);
	}
}

#endif	//HEAPSORT_H
