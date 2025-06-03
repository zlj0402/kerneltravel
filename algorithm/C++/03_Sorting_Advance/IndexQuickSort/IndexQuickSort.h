#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <ctime>

template<typename T>
int __partition(T arr[], int indexes[], int l, int r) {

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

template<typename T>
void __quickSort(T arr[], int indexes[], int l, int r) {

    if (l >= r)
        return;

    int p = __partition(arr, indexes, l, r);

    __quickSort(arr, indexes, l, p - 1);
    __quickSort(arr, indexes, p + 1, r);
}

template<typename T>
void quickSort(T arr[], int indexes[], int n) {

    srand(time(nullptr));
    __quickSort(arr, indexes, 0, n - 1);
}

#endif  //QUICKSORT_H
