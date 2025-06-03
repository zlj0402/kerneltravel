#include "../../Include/SortTestHelper.h"
#include "IndexQuickSort.h"
#include "IndexQuickSortClass.h"

#include "load.h"

using namespace std;

template<typename T>
void printIndexArray(T arr[], int indexes[], int n) {

	cout << "indexes: ";
	for (int i = 0; i < n; i++) {
		cout << indexes[i] << " ";
	}

	cout << endl << "values: ";
	for (int i = 0; i < n; i++) {
		cout << arr[indexes[i]] << " ";
	}
	cout << endl;
}

int main() {
	
	int n = 10;
	int* arr = SortTestHelper::generateRandomArray(n, 0, n);

	SortTestHelper::printArray(arr, n);
	cout << endl;

	cout << endl << "------------------------" << endl << endl;

	int* arr1 = SortTestHelper::copyIntArray(arr, n);
	IndexQuickSort<int> iqs(arr1, n);
	int* indexes = iqs.quickSort();

	cout << "cout by IndexQuickSort.h: " << endl;
	printIndexArray(arr1, indexes, n);

	cout << endl << "------------------------" << endl << endl;

	int* arr2 = SortTestHelper::copyIntArray(arr, n);
	int* indexes2 = new int[n];
	for (int i = 0; i < n; i++)
		indexes2[i] = i;

	quickSort(arr2, indexes2, n);

	cout << "cout by IndexQuickSort.h: " << endl;
	printIndexArray(arr2, indexes2, n);

}
