//
// Created by liangj.zhang on 27/4/2025 -- last updated: 27/4/2025
// updated on 27/4/2025: completed TopK problem
//

#include "../../SortTestHelper.h"
// MergeSort is for sorting help to confirm the result of TopK problem
#include "../All_Sort_Types_Test/MergeSort.h"

// >>>>>>>>>>>>>>>>>>>>> TopK -- The Kth smallest number <<<<<<<<<<<<<<<<<<<<<<

// 双路快速排序的partition
// 对arr[l...r]部分进行partition操作
// 返回p, 使得arr[l+1...i] =< arr[p] ; arr[j...r] >= arr[p]
template<typename T>
int __partition(T arr[], int l, int r) {

	// 随机在arr[l...r]的范围中, 选择一个数值作为标定点pivot
	swap(arr[l], arr[rand() % (r - l + 1) + l]);
	T v = arr[l];

	// arr[l+1...i) <= v; arr(j...r] >= v
	int i = l + 1, j = r;
	while (true) {

		while (i <= r && arr[i] < v) i++;
		while (j >= l + 1 && arr[j] > v) j--;
		if (i >= j) break;
		swap(arr[i++], arr[j--]);
	}
	swap(arr[l], arr[j]);	// swap(arr[l], arr[i]); 是有可能跟arr[r+1] -> arr[n - 1 + 1] -> arr[n]交换的;

	return j;
}

// 对arr[l...r]部分进行快速排序
template<typename T>
void __quickSort(T arr[], int l, int r, int k, bool *find, T* ret) {

	if (l > r)	// 原来错误的写法：if (l >= r); 当 l 和 r 之间只有一个元素时，而这个元素恰好是第 k 个位置，这时候 return; flag 没有被标记，ret 也没有取值; 耗时两个多小时的调试，真是要命；
		return;
	//if (r - l <= 15) {
	//	insertionSort(arr, l, r);
	//	return;
	//}

	int p = __partition(arr, l, r);

	if (k < p) {
		__quickSort(arr, l, p - 1, k, find, ret);
	}
	else if (k > p)
		__quickSort(arr, p + 1, r, k, find, ret);
	else {
		*ret = arr[p];
		*find = true;
		return;
	}
}

template<typename T>
T quickSort(T arr[], int n, int k) {

	srand(time(NULL));

	bool find = false;
	T ret;
	__quickSort(arr, 0, n - 1, k - 1, &find, &ret);

	return ret;
}

int main(int argc, char* argv[]) {
	
	int n = 250;
	int k = 5;
	int* arr = SortTestHelper::generateRandomArray(n, 0, 1000);
	int* arr1 = SortTestHelper::copyIntArray(arr, n);
	int* arr2 = SortTestHelper::copyIntArray(arr, n);


	SortTestHelper::printArray(arr, n);
	std::cout << std::endl;

	mergeSortBU(arr1, n);
	SortTestHelper::printArray(arr1, n);
	std::cout << "top K(" << k << "): " << arr1[k - 1]<< std::endl;
	std::cout << "top K(" << k + 1 << "): " << arr1[k] << std::endl;
	std::cout << "isPermutation: " << SortTestHelper::isPermutation(arr, arr1, n, false) << std::endl << std::endl;

	std::cout << "top K(" << k << "): " << quickSort(arr2, n, k) << std::endl;
	SortTestHelper::printArray(arr2, n);
	std::cout << "isPermutation: " << SortTestHelper::isPermutation(arr, arr2, n, false) << std::endl << std::endl;
	std::cout << "top K(" << k + 1 << "): " << quickSort(arr2, n, k + 1) << std::endl;
	SortTestHelper::printArray(arr2, n);
	std::cout << "isPermutation: " << SortTestHelper::isPermutation(arr, arr2, n, false) << std::endl << std::endl;

	delete[] arr;
	delete[] arr1;
	delete[] arr2;
}

