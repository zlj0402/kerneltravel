#include <iostream>
#include <algorithm>
#include "../../Include/SortTestHelper.h"
#include "SelectionSort.h"
#include "InsertionSort.h"
#include "BubbleSort.h"
#include "ShellSort.h"

#include "load.h"

using namespace std;

//int main(int argc, char* argv[]) {
//    
//    int n = 6000;
//    int* arr = SortTestHelper::generateRandomArray(n, 0, n);
//    int* cp_arr = SortTestHelper::copyIntArray(arr, n);
//    SortTestHelper::printArray(arr, n);
//    SortTestHelper::printArray(cp_arr, n);
//    bubbleSort_base(arr, n);
//    SortTestHelper::printArray(arr, n);
//    cout << SortTestHelper::isSorted(arr, n) << endl;
//    cout << SortTestHelper::isPermutation(cp_arr, arr, n, false) << endl;
//
//    //int n = 6;
//    //int arr[] = { 6, 5, 5, 3, 0, 2 };
//    //SortTestHelper::printArray(arr, n);
//    //bubbleSort(arr, n);
//    //SortTestHelper::printArray(arr, n);
//}

template<typename T>
void testSort(T* arr, int n) {
    int* arr1 = SortTestHelper::copyIntArray(arr, n);
    int* arr2 = SortTestHelper::copyIntArray(arr, n);
    int* arr3 = SortTestHelper::copyIntArray(arr, n);
    int* arr4 = SortTestHelper::copyIntArray(arr, n);
    int* arr5 = SortTestHelper::copyIntArray(arr, n);
    int* arr6 = SortTestHelper::copyIntArray(arr, n);

    SortTestHelper::testSort("Insertion Sort", insertionSort, arr, arr1, n);
    SortTestHelper::testSort("Shell Sort", shellSort, arr, arr2, n);
    SortTestHelper::testSort("Shell Sort2", shellSort2, arr, arr3, n);
    SortTestHelper::testSort("Selection Sort", selectionSort, arr, arr4, n);
    SortTestHelper::testSort("Selection Sort2", selectionSort2, arr, arr5, n);
    SortTestHelper::testSort("Bubble Sort", bubbleSort, arr, arr6, n);

    delete[] arr;
    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
    delete[] arr4;
    delete[] arr5;
    delete[] arr6;

    cout << endl;
}

int main(int argc, char* argv[]) {

    int n = 20000;

    // 测试1 一般测试
    cout << "Test for random array, size = " << n << ", random range [0, " << n << "]" << endl;
    int* arr = SortTestHelper::generateRandomArray(n, 0, n);
    testSort(arr, n);


    // 测试2 有序性更强的测试
    cout << "Test for more ordered random array, size = " << n << ", random range [0, 3]" << endl;
    arr = SortTestHelper::generateRandomArray(n, 0, 3);
    testSort(arr, n);


    // 测试3 测试近乎有序的数组
    int swapTimes = 100;
    cout << "Test for nearly ordered array, size = " << n << ", swap time = " << swapTimes << endl;
    arr = SortTestHelper::generateNearlyOrderedArray(n, swapTimes);
    testSort(arr, n);

    return 0;
}
