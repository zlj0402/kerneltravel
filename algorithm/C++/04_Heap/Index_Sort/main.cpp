#include "IndexQuickSort.h"
#include "../../Include/SortTestHelper.h"

#include "load.h"

int main(int argc, char* argv[]) {

	int n = 10;
	int* arr = SortTestHelper::generateRandomArray(n, 0, n);
	IndexQuickSort<int> iqs = IndexQuickSort<int>(arr, n);

	iqs.printArray();
	std::cout << "isSorted: " << iqs.isSorted() << std::endl;
}
