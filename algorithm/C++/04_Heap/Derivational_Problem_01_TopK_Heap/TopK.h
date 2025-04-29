//
// Created by liangj.zhang on 27/4/2025 -- last updated on 28/4/2025
// updated on 27/4/2025: add quickSort way to solve topK issue
// updated on 28/4/2025: add two heap ways to solve topK issue
//

#include "Heap.h"

// >>>>>>>>>>>>>>>>>>>>> TopK -- The Kth number -- Heap Way1(pop & re-insert) <<<<<<<<<<<<<<<<<<<<<<

template<typename T>
class TopK_Heap {
	
private:
	Heap<T> minH;
	Heap<T> maxH;
	const int capacity;
	T topK;

public:
	TopK_Heap(T* arr, int n) : 
		minH(arr, n, false), maxH(arr, n, true), capacity(n), topK(T{}) {

	}

	// 时间复杂度为：O(klogN)
	T heapK(int k, bool topMax) {

		if (k <= capacity / 2) {
			
			Heap<T>* h = topMax ? &maxH : &minH;
			T* arr = new T[k];

			int i;
			for (i = 0; i < k; i++)
				arr[i] = (*h).pop();
			topK = arr[i - 1];

			for (i = i - 1; i >= 0; i--)
				(*h).insert(arr[i]);
			delete[] arr;
		}
		else {

			Heap<T>* h = topMax ? &minH : &maxH;
			int size = capacity - k + 1;
			T* arr = new T[size];

			int i;
			for (i = 0; i < size; i++)
				arr[i] = (*h).pop();
			topK = arr[i - 1];

			for (i = i - 1; i >= 0; i--)
				(*h).insert(arr[i]);
			delete[] arr;
		}

		return topK;
	}

};

// >>>>>>>>>>>>>>>>>>>>> TopK -- The Kth number -- Heap Way2(heap capacity: K, traverse & insert all) <<<<<<<<<<<<<<<<<<<<<<

template<typename T>
class TopK_Heap2 {

private:
	Heap<T> h;
	int count;
	int capacity;
	T* src;
	T topK;

public:

	TopK_Heap2(T* arr, int n, bool topMax) : capacity(0), count(n), src(arr), topK(T{}), h(topMax ? false : true) {

		// Max: 前 top K 用小顶堆，取堆顶得到第 K 个值；
		// Min: 前 top K 用大顶堆，取堆顶得到第 K 个值；
		//h = Heap<T>(topMax ? false : true);
	}

	// 时间复杂度：O(nlogk)
	T heapK(int k) {

		if (h.getCapacity() == 0)
			h.setCapacity(k);
		else
			h.resetHeap(k);

		for (int i = 0; i < count; i++)
			h.topInsert(src[i]);

		topK = h.getTop();
		return topK;
	}

};

// >>>>>>>>>>>>>>>>>>>>> TopK -- The Kth number -- Quick Sort Way <<<<<<<<<<<<<<<<<<<<<<

template<typename T>
class TopK_QuickSort {

private:
	T* arr;
	int n;

	int k;
	T topK;
	bool find;

	static bool Cmp0(const T& e, const T& pivot) {
		return e < pivot;
	}

	static bool Cmp1(const T& e, const T& pivot) {
		return e > pivot;
	}

	bool (*lCmp)(const T& e, const T& pivot);

	bool (*rCmp)(const T& e, const T& pivot);

	void __quickSort(int l, int r);

	int __partition(int l, int r);

	void __quickSort3Ways(int l, int r);

public:

	TopK_QuickSort(T arr[], int n) : arr(arr), n(n), find(false), k(1), topK(T{}), lCmp(Cmp0), rCmp(Cmp1) { }

	T quickSort(int k, bool ascending);
};


template<typename T>
T TopK_QuickSort<T>::quickSort(int k, bool ascending) {

	srand(time(NULL));

	this->k = k - 1;	// 原来错误的写法：k = k - 1;
	find = false;
	topK = T{};

	// 升序 topK 是找最小值; 降序 topK 是找最大值;
	lCmp = ascending ? Cmp0 : Cmp1;
	rCmp = ascending ? Cmp1 : Cmp0;
	//__quickSort(0, n - 1);
	__quickSort3Ways(0, n - 1);

	return topK;
}

// >>>>>>>>>>>>>>>>>>>>> 随机化 + 双路快速排序 <<<<<<<<<<<<<<<<<<<<<<<<

// 对arr[l...r]部分进行快速排序
template<typename T>
void TopK_QuickSort<T>::__quickSort(int l, int r) {

	// 原来错误的写法：if (l >= r); 
	// 当 l 和 r 之间只有一个元素时，而这个元素恰好是第 k 个位置，这时候 return; find 没有被标记，topK 也没有取值; 耗时两个多小时的调试，真是要命；
	if (l > r) 
		return;

	int p = __partition(l, r);

	if (k < p) {
		__quickSort(l, p - 1);
	}
	else if (k > p)
		__quickSort(p + 1, r);
	else {
		topK = arr[p];
		find = true;
		return;
	}
}

// 双路快速排序的partition
// 对arr[l...r]部分进行partition操作
// 返回p, 使得arr[l+1...i] =< arr[p] ; arr[j...r] >= arr[p]
template<typename T>
int TopK_QuickSort<T>::__partition(int l, int r) {

	// 随机在arr[l...r]的范围中, 选择一个数值作为标定点pivot
	swap(arr[l], arr[rand() % (r - l + 1) + l]);
	T v = arr[l];

	// arr[l+1...i) <= v; arr(j...r] >= v
	int i = l + 1, j = r;
	while (true) {

		while (i <= r && lCmp(arr[i], v)) i++;
		while (j >= l + 1 && rCmp(arr[j], v)) j--;
		if (i >= j) break;
		swap(arr[i++], arr[j--]);
	}
	swap(arr[l], arr[j]);	// swap(arr[l], arr[i]); 是有可能跟arr[r+1] -> arr[n - 1 + 1] -> arr[n]交换的;

	return j;
}

// >>>>>>>>>>>>>>>>>>>>> 随机化 + 三路快速排序 <<<<<<<<<<<<<<<<<<<<<<<<

// 对arr[l...r]部分进行快速排序
template<typename T>
void TopK_QuickSort<T>::__quickSort3Ways(int l, int r) {

	if (l > r)	// 同 2Ways 一样的错误写法：if (l >= r)
		return;

	// partition
	// 随机在arr[l...r]的范围中, 选择一个数值作为标定点pivot
	swap(arr[l], arr[rand() % (r - l + 1) + l]);
	int v = arr[l];

	int lt = l;		// [l+1, lt] < v  after swap => [l, lt] < v
	int gt = r + 1;	// [gt, r] > v
	int i = lt + 1;	// [lt+1, i) == v

	while (i < gt) {

		if (lCmp(arr[i], v)) {
			// 当 [l, r] 之间，元素没有一个重复的，i 左边一定是满足 Cmp 的条件，此时 lt 和 i 是相邻的（lt + 1 == i），没必要换；
			// 此处能减少 arr 几乎没有重复元素时的交换耗时；三路快排，本身是在相同元素多的时候，效率显著；加上此处修改，另一方面不同元素时，效率也得到提升；
			if (i != lt + 1)
				swap(arr[i], arr[lt + 1]);
			lt++;
			i++;
			//swap(arr[i++], arr[++lt]);	// 一步搞定
		}
		else if (arr[i] == v) {			// 原来错误写法：else if (!Cmp(arr[i], v))，（以降序排列为例）将 > 的条件放前面没错，错误的是取反 Cmp 来进行判定此时是 <（还有 ==）；将 == 的判断放前面，可避免此处错误
			i++;
		}
		else {
			swap(arr[i], arr[gt - 1]);
			gt--;
			//swap(arr[i], arr[--gt]);	// 一步搞定
		}
	}

	swap(arr[l], arr[lt--]);

	if (k <= lt)
		__quickSort3Ways(l, lt);
	else if (k >= gt)
		__quickSort3Ways(gt, r);
	else {
		topK = arr[lt + 1];
		find = true;
		return;
	}
}

