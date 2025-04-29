#ifndef INDEX_QUICKSORT_H
#define INDEX_QUICKSORT_H

#include <ctime>
#include <iostream>

// >>>>>>>>>>>>>>>>>>>>>>>>>> 双路快速排序 -- index version <<<<<<<<<<<<<<<<<<<<<<<<<<<
template<typename T>
class IndexQuickSort {

private:
	const int capacity;
	int count;
	T* data;
	T* indexes;

public:
	IndexQuickSort(T arr[], int n) : capacity(n), count(n) {
		
		data = new T[capacity];
		indexes = new int[capacity];

		std::cout << "   data: ";
		for (int i = 0; i < n; i++) {

			data[i] = arr[i];
			indexes[i] = i;
			std::cout << data[i] << "\t ";
		}
		std::cout << std::endl;

		std::cout << "indexes: ";
		for (int i = 0; i < n; i++)
			std::cout << indexes[i] << "\t ";
		std::cout << std::endl;

		quickSort();
	}

	~IndexQuickSort() {

		delete[] data;
		delete[] indexes;
	}

	void printArray() const {

		std::cout << std::endl;

		std::cout << "   data: ";
		for (int i = 0; i < count; i++) 
			std::cout << data[indexes[i]] << "\t ";
		std::cout << std::endl;

		std::cout << "indexes: ";
		for (int i = 0; i < count; i++) 
			std::cout << indexes[i] << "\t ";
		std::cout << std::endl;
	}

	// 判断data数组是否有序
	bool isSorted() const {

		for (int i = 0; i < count - 1; i++)
			if (data[indexes[i]] > data[indexes[i + 1]])
				return false;

		return true;
	}

private:

	int partition(int l, int r) {
		
		std::swap(indexes[l], indexes[rand() % (r - l + 1) + l]);
		T v = data[indexes[l]];

		int i = l + 1, j = r;
		while (true) {
			
			while (i <= r && data[indexes[i]] < v) i++;
			while (j > l && data[indexes[j]] > v) j--;
			if (i >= j) break;
			std::swap(indexes[i++], indexes[j--]);
		}
		std::swap(indexes[l], indexes[j]);

		return j;
	}

	void __quickSort(int l, int r) {
		
		if (l >= r)
			return;

		int p = partition(l, r);
		__quickSort(l, p - 1);
		__quickSort(p + 1, r);
	}

	void quickSort() {
		srand(time(NULL));
		__quickSort(0, count - 1);
	}
};

#endif	//INDEX_QUICKSORT_H
