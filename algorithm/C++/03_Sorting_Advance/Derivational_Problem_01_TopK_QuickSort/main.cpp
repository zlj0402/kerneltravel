//
// Created by liangj.zhang on 27/4/2025 -- last updated: 27/4/2025
// updated on 27/4/2025: completed TopK problem
// 					   _2: #include TopK.h && TopK_QuickSort class support both top smallest and top largest
//

#include "../../Include/SortTestHelper.h"
// MergeSort is for sorting help to confirm the result of TopK problem
#include "../All_Sort_Types_Test/MergeSort.h"
#include "TopK.h"
#include <vector>


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

	TopK_QuickSort<int> tkqs = TopK_QuickSort<int>(arr2, n);

	std::cout << "top K(" << k << "): " << tkqs.quickSort(k, true) << std::endl;
	SortTestHelper::printArray(arr2, n);
	std::cout << "isPermutation: " << SortTestHelper::isPermutation(arr, arr2, n, false) << std::endl << std::endl;
	std::cout << "top K(" << k + 1 << "): " << tkqs.quickSort(k + 1, true) << std::endl;
	SortTestHelper::printArray(arr2, n);
	std::cout << "isPermutation: " << SortTestHelper::isPermutation(arr, arr2, n, false) << std::endl << std::endl;

	delete[] arr;
	delete[] arr1;
	delete[] arr2;
}

//int main(int argc, char* argv[]) {
//
//	//int n = 9;
//	//int arr[] = { 3,2,1,5,6,4 };
//	vector<int> a = { 3,2,3,1,2,4,5,5,6 };
//	int n = a.size();
//	int* arr = a.data();
//	int* arr1 = SortTestHelper::copyIntArray(arr, n);
//	int* arr2 = SortTestHelper::copyIntArray(arr, n);
//
//	SortTestHelper::printArray(arr, n);
//	mergeSortBU(arr1, n);
//	SortTestHelper::printArray(arr1, n);
//
//	TopK_QuickSort<int> tkqs = TopK_QuickSort<int>(arr2, n);
//	SortTestHelper::printArray(arr2, n);
//
//	std::cout << tkqs.quickSort(2, false) << std::endl;
//}
