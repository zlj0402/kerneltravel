//
// Created by liangj.zhang on 27/4/2025 -- last updated: 27/4/2025
// updated on 27/4/2025: completed TopK problem
// 					   _2: #include TopK.h && TopK_QuickSort class support both top smallest and top largest
// updated on 28/4/2025: add Heap Way test in main.cpp
//

#include "../../Include/SortTestHelper.h"
// MergeSort is for sorting help to confirm the result of TopK problem
#include "../../Include/SortAlgorithms/MergeSort.h"
#include "TopK.h"
#include <vector>

#include "load.h"

int main(int argc, char* argv[]) {
	
	int n = 10000;
	int k = 3333;
	int* arr = SortTestHelper::generateRandomArray(n, 0, 20 * n);
	int* arr1 = SortTestHelper::copyIntArray(arr, n);
	int* arr2 = SortTestHelper::copyIntArray(arr, n);
	int* arr3 = SortTestHelper::copyIntArray(arr, n);
	int* arr4 = SortTestHelper::copyIntArray(arr, n);


	//SortTestHelper::printArray(arr, n);
	std::cout << std::endl;

	mergeSortBU(arr1, n);
	//SortTestHelper::printArray(arr1, n);
	std::cout << "top K(" << k << "): " << arr1[k - 1]<< std::endl;
	std::cout << "top K(" << k + 1 << "): " << arr1[k] << std::endl;
	std::cout << "isPermutation: " << SortTestHelper::isPermutation(arr, arr1, n, false) << std::endl << std::endl;

	TopK_QuickSort<int> tkqs = TopK_QuickSort<int>(arr2, n);

	std::cout << "top K(" << k << "): " << tkqs.quickSort(k, true) << std::endl;
	//SortTestHelper::printArray(arr2, n);
	std::cout << "isPermutation: " << SortTestHelper::isPermutation(arr, arr2, n, false) << std::endl << std::endl;
	std::cout << "top K(" << k + 1 << "): " << tkqs.quickSort(k + 1, true) << std::endl;
	//SortTestHelper::printArray(arr2, n);
	std::cout << "isPermutation: " << SortTestHelper::isPermutation(arr, arr2, n, false) << std::endl << std::endl;

	//Heap<int> tkh = Heap<int>(arr3, n, false);
	//while (!tkh.isEmpty())
	//	std::cout << tkh.pop() << " ";
	//std::cout << std::endl;

	TopK_Heap<int> tkh = TopK_Heap<int>(arr3, n);

	std::cout << "top K(" << k << "): " << tkh.heapK(k, false) << std::endl;
	std::cout << "top K(" << k + 1 << "): " << tkh.heapK(k + 1, false) << std::endl << std::endl;

	TopK_Heap2<int> tkh2 = TopK_Heap2<int>(arr4, n, false);

	std::cout << "top K(" << k << "): " << tkh2.heapK(k) << std::endl;
	std::cout << "top K(" << k + 1 << "): " << tkh2.heapK(k + 1) << std::endl << std::endl;

	delete[] arr;
	delete[] arr1;
	delete[] arr2;
	delete[] arr3;
	delete[] arr4;
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
