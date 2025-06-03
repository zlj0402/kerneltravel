#ifndef INDEXQUICKSORTCLASS_H
#define INDEXQUICKSORTCLASS_H

#include <ctime>

template<typename T>
class IndexQuickSort {
private:
    int n;
    T* arr;
    int* indexes;

public:
    IndexQuickSort(T* arr, int n) : arr(arr), n(n) {

        indexes = new int[n];
        for (int i = 0; i < n; i++)
            indexes[i] = i;
    }

    ~IndexQuickSort() {

        delete[] indexes;
    }

    int* getIndexes() {

        return indexes;
    }

    int* quickSort() {

        srand(time(nullptr));
        __quickSort(0, n - 1);

        return getIndexes();
    }

private:
    int __partition(int l, int r) {

        swap(indexes[l], indexes[rand() % (r - l + 1) + l]);
        T v = arr[indexes[l]];

        int i = l + 1;
        int j = r;
        while (1) {

            while (i <= r && arr[indexes[i]] < v) i++;
            while (j >= l + 1 && arr[indexes[j]] > v) j--;
            if (i >= j) break;
            swap(indexes[i++], indexes[j--]);
        }

        swap(indexes[l], indexes[j]);

        return j;
    }

    void __quickSort(int l, int r) {

        if (l >= r)
            return;

        int p = __partition(l, r);

        __quickSort(l, p - 1);
        __quickSort(p + 1, r);
    }
};

#endif  //INDEXQUICKSORTCLASS_H
