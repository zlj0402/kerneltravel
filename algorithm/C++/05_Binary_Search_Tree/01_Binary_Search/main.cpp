#include "../../Include/SortTestHelper.h"
#include "../../Include/SortAlgorithms/MergeSort.h"
#include "load.h"

// 二分查找法,在有序数组arr中,查找target
// 如果找到target,返回相应的索引index
// 如果没有找到target,返回-1
template<typename T>
int binarySearch(T arr[], int n, T target) {

	// 在arr[l...r]之中查找target
	int l = 0;
	int r = n - 1;
	while (l <= r) {

		int mid = l + (r - l) / 2;	// 防止 (l + r) / 2，可能存在溢出

		if (target == arr[mid])
			return mid;
		else if (target < arr[mid])
			r = mid - 1;
		else
			l = mid + 1;
	}
	return -1;
}

// 用递归的方式写二分查找法
template<typename T>
int __binarySearch2(T arr[], int l, int r, T& target) {

	if (l > r)
		return -1;

	int mid = l + (r - l) / 2;
	if (arr[mid] == target)
		return mid;
	else if (target < arr[mid])
		return __binarySearch2(arr, l, mid - 1, target);	// 原来漏了 return
	else
		return __binarySearch2(arr, mid + 1, r, target);	// 原来漏了 return
}

template<typename T>
int binarySearch2(T arr[], int n, T& target) {

	return __binarySearch2(arr, 0, n - 1, target);
}

int main() {

	int n = 10;
	int* arr = SortTestHelper::generateRandomArray(n, 0, n);

	int* arr1 = SortTestHelper::copyIntArray(arr, n);
	mergeSortBU(arr1, n);
	SortTestHelper::printArray(arr1, n);

	int r_idx = rand() % n;
	int target = arr1[r_idx];
	std::cout << "r_idx: " << r_idx << ", target: " << target << std::endl << std::endl;

	int find_idx = binarySearch(arr1, n, target);
	std::cout << "binarySearch: idx: " << find_idx << ", value: " << arr1[find_idx] << std::endl;

	find_idx = binarySearch2(arr1, n, target);
	std::cout << "binarySearch2: idx: " << find_idx << ", value: " << arr1[find_idx] << std::endl;

	delete[] arr;
	delete[] arr1;
}
