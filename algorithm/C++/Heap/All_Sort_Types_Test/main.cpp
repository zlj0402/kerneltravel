#include <iostream>
#include <algorithm>
#include "../../SortTestHelper.h"
#include "SelectionSort.h"
#include "InsertionSort.h"
#include "BubbleSort.h"
#include "ShellSort.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "HeapSort.h"

#include "load.h"

using namespace std;

template<typename T>
void testSort(T* arr, int n);

template<typename T>
void testSort_nlogn(T* arr, int n);

//int main(int argc, char* argv[]) {
//    
//    //int n = 6;
//    //int* arr = SortTestHelper::generateRandomArray(n, 0, n);
//    //int* arr1 = SortTestHelper::copyIntArray(arr, n);
//    //SortTestHelper::printArray(arr, n);
//    //SortTestHelper::printArray(arr1, n);
//    //heapSort3(arr1, n);
//    //SortTestHelper::printArray(arr1, n);
//
//    //bool ordered = SortTestHelper::isSorted(arr1, n);
//    //bool isPermutation = SortTestHelper::isPermutation(arr, arr1, n, false);
//    //cout << "ordered: " << ordered << endl;
//    //cout << "isPermutation: " << isPermutation << endl;
//
//    //delete[] arr;
//    //delete[] arr1;
//
//    int n = 6;
//    int arr[] = { 5, 0, 4, 1, 5, 2 };
//    SortTestHelper::printArray(arr, n);
//    heapSort3(arr, n);
//    SortTestHelper::printArray(arr, n);
//}

int main(int argc, char* argv[]) {

    int n = 10000000;

    // 测试1 一般测试
    cout << "Test for random array, size = " << n << ", random range [0, " << n << "]" << endl;
    int* arr = SortTestHelper::generateRandomArray(n, 0, n);
    testSort_nlogn(arr, n);


    // 测试2 有序性更强的测试
    cout << "Test for more ordered random array, size = " << n << ", random range [0, 3]" << endl;
    arr = SortTestHelper::generateRandomArray(n, 0, 3);
    testSort_nlogn(arr, n);


    // 测试3 测试近乎有序的数组
    int swapTimes = 100;
    cout << "Test for nearly ordered array, size = " << n << ", swap time = " << swapTimes << endl;
    arr = SortTestHelper::generateNearlyOrderedArray(n, swapTimes);
    testSort_nlogn(arr, n);

    // 测试4 测试升序的数组
    cout << "Test for ascending array, size = " << n << endl;
    arr = SortTestHelper::generateOrderedArray(n, true);
    testSort_nlogn(arr, n);

    // 测试5 测试降序的数组
    cout << "Test for desending array, size = " << n << endl;
    arr = SortTestHelper::generateOrderedArray(n, false);
    testSort_nlogn(arr, n);

    return 0;
}

template<typename T>
void testSort(T* arr, int n) {
    int* arr1 = SortTestHelper::copyIntArray(arr, n);
    int* arr2 = SortTestHelper::copyIntArray(arr, n);
    int* arr3 = SortTestHelper::copyIntArray(arr, n);
    int* arr4 = SortTestHelper::copyIntArray(arr, n);
    int* arr5 = SortTestHelper::copyIntArray(arr, n);
    int* arr6 = SortTestHelper::copyIntArray(arr, n);
    int* arr7 = SortTestHelper::copyIntArray(arr, n);
    int* arr8 = SortTestHelper::copyIntArray(arr, n);
    int* arr9 = SortTestHelper::copyIntArray(arr, n);
    //int* arr10 = SortTestHelper::copyIntArray(arr, n);
    int* arr11 = SortTestHelper::copyIntArray(arr, n);
    int* arr12 = SortTestHelper::copyIntArray(arr, n);
    int* arr13 = SortTestHelper::copyIntArray(arr, n);
    int* arr14 = SortTestHelper::copyIntArray(arr, n);
    int* arr15 = SortTestHelper::copyIntArray(arr, n);
    int* arr16 = SortTestHelper::copyIntArray(arr, n);

    SortTestHelper::testSort("Insertion Sort", insertionSort, arr, arr1, n);
    SortTestHelper::testSort("Shell Sort", shellSort, arr, arr2, n);
    SortTestHelper::testSort("Shell Sort2", shellSort2, arr, arr3, n);
    SortTestHelper::testSort("Selection Sort", selectionSort, arr, arr4, n);
    SortTestHelper::testSort("Selection Sort2", selectionSort2, arr, arr5, n);
    SortTestHelper::testSort("Bubble Sort", bubbleSort, arr, arr6, n);
    SortTestHelper::testSort("Merge Sort", mergeSort, arr, arr7, n);
    SortTestHelper::testSort("Merge Sort2", mergeSort2, arr, arr8, n);
    SortTestHelper::testSort("Merge Sort BU", mergeSortBU, arr, arr9, n);
    //SortTestHelper::testSort("Quick Sort", quickSort, arr, arr10, n);
    SortTestHelper::testSort("Quick Sort2", quickSort2, arr, arr11, n);
    SortTestHelper::testSort("Quick Sort3", quickSort3, arr, arr12, n);
    SortTestHelper::testSort("Quick Sort4", quickSort4, arr, arr13, n);
    SortTestHelper::testSort("Heap Sort", heapSort, arr, arr14, n);
    SortTestHelper::testSort("Heap Sort2", heapSort2, arr, arr15, n);
    SortTestHelper::testSort("Heap Sort3", heapSort3, arr, arr16, n);

    delete[] arr;
    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
    delete[] arr4;
    delete[] arr5;
    delete[] arr6;
    delete[] arr7;
    delete[] arr8;
    delete[] arr9;
    //delete[] arr10;
    delete[] arr11;
    delete[] arr12;
    delete[] arr13;
    delete[] arr14;
    delete[] arr15;
    delete[] arr16;

    cout << endl;
}

template<typename T>
void testSort_nlogn(T* arr, int n) {
    int* arr1 = SortTestHelper::copyIntArray(arr, n);
    int* arr2 = SortTestHelper::copyIntArray(arr, n);
    int* arr3 = SortTestHelper::copyIntArray(arr, n);
    int* arr4 = SortTestHelper::copyIntArray(arr, n);
    int* arr5 = SortTestHelper::copyIntArray(arr, n);
    //int* arr6 = SortTestHelper::copyIntArray(arr, n);
    //int* arr7 = SortTestHelper::copyIntArray(arr, n);
    int* arr8 = SortTestHelper::copyIntArray(arr, n);
    int* arr9 = SortTestHelper::copyIntArray(arr, n);
    int* arr10 = SortTestHelper::copyIntArray(arr, n);
    int* arr11 = SortTestHelper::copyIntArray(arr, n);
    int* arr12 = SortTestHelper::copyIntArray(arr, n);

    SortTestHelper::testSort("Shell Sort", shellSort, arr, arr1, n);
    SortTestHelper::testSort("Shell Sort2", shellSort2, arr, arr2, n);
    SortTestHelper::testSort("Merge Sort", mergeSort, arr, arr3, n);
    SortTestHelper::testSort("Merge Sort2", mergeSort2, arr, arr4, n);
    SortTestHelper::testSort("Merge Sort BU", mergeSortBU, arr, arr5, n);
    //SortTestHelper::testSort("Quick Sort", quickSort, arr, arr6, n);
    //SortTestHelper::testSort("Quick Sort2", quickSort2, arr, arr7, n);
    SortTestHelper::testSort("Quick Sort3", quickSort3, arr, arr8, n);
    SortTestHelper::testSort("Quick Sort4", quickSort4, arr, arr9, n);
    SortTestHelper::testSort("Heap Sort", heapSort, arr, arr10, n);
    SortTestHelper::testSort("Heap Sort2", heapSort2, arr, arr11, n);
    SortTestHelper::testSort("Heap Sort3", heapSort3, arr, arr12, n);

    delete[] arr;
    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
    delete[] arr4;
    delete[] arr5;
    //delete[] arr6;
    //delete[] arr7;
    delete[] arr8;
    delete[] arr9;
    delete[] arr10;
    delete[] arr11;
    delete[] arr12;

    cout << endl;
}
