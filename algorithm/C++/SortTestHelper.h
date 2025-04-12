//
// Created by liangj.zhang on 13/4/2025
// Partially referenced from liuyubobobo's course on GitHub
// URL: https://github.com/liuyubobobo/Play-with-Algorithms/tree/master/02-Sorting-Basic/Course%20Code%20(C%2B%2B)
//

#ifndef SORTTESTHELPER_H
#define SORTTESTHELPER_H

#include <iostream>
#include <algorithm>
#include <string>
#include <ctime>
#include <cassert>
#include <string>
#include <map>

using namespace std;


namespace SortTestHelper {

    // 生成有n个元素的随机数组,每个元素的随机范围为[rangeL, rangeR]
    int* generateRandomArray(int n, int range_l, int range_r) {

        int* arr = new int[n];

        srand(time(NULL));
        for (int i = 0; i < n; i++)
            arr[i] = rand() % (range_r - range_l + 1) + range_l;
        return arr;
    }

    // 生成一个近乎有序的数组
    // 首先生成一个含有[0...n-1]的完全有序数组, 之后随机交换swapTimes对数据
    // swapTimes定义了数组的无序程度:
    // swapTimes == 0 时, 数组完全有序
    // swapTimes 越大, 数组越趋向于无序
    int* generateNearlyOrderedArray(int n, int swapTimes) {

        int* arr = new int[n];
        for (int i = 0; i < n; i++)
            arr[i] = i;

        srand(time(NULL));
        for (int i = 0; i < swapTimes; i++) {
            int posx = rand() % n;
            int posy = rand() % n;
            swap(arr[posx], arr[posy]);
        }

        return arr;
    }

    // 拷贝整型数组a中的所有元素到一个新的数组, 并返回新的数组
    template<typename T>
    int* copyIntArray(T a[], int n) {

        T* arr = new T[n];
        //* 在VS中, copy函数被认为是不安全的, 请大家手动写一遍for循环:)
        // 浅拷贝: 对于基本类型和不包含指针成员的类没有影响
        copy(a, a + n, arr);
        return arr;
    }

    // 打印arr数组的所有内容
    template<typename T>
    void printArray(T arr[], int n) {

        for (int i = 0; i < n; i++)
            cout << arr[i] << " ";
        cout << endl;

        return;
    }

    // 判断arr数组是否有序
    template<typename T>
    bool isSorted(T arr[], int n) {

        for (int i = 0; i < n - 1; i++)
            if (arr[i] > arr[i + 1])
                return false;

        return true;
    }

    // 排序操作是否有错误
    template<typename T>
    bool isPermutation(T origin[], T src[], int n, bool debug) {

        bool ret = false;
        
        // 判断操作之后的，src每个值的个数，是否和之前origin值的个数相同
        map<T, int> ori_map;
        map<T, int> src_map;

        for (int i = 0; i < n; i++) {
            ++ori_map[origin[i]];   // 下标 [] 自动初始化为 0，无需判断是否存在
            ++src_map[src[i]];
        }

        ret = (ori_map == src_map);
        if (!ret && debug) {
            int cnt = 0;
            cout << "----- ori_map -----" << endl;
            for (const auto& pair : ori_map) {
                cout << pair.first << " => " << pair.second << '\n';
                cnt += pair.second;
            }
            cout << "total count: " << cnt << endl;
            cnt = 0;

            cout << "----- src_map -----" << endl;
            for (const auto& pair : src_map) {
                cout << pair.first << " => " << pair.second << '\n';
                cnt += pair.second;
            }
            cout << "total count: " << cnt << endl;
        }

        return ret;
    }

    // 测试sort排序算法排序arr数组所得到结果的正确性和算法运行时间
    template<typename T>
    void testSort(const string& sortName, void (*sort)(T[], int), T arr[], int n) {

        clock_t startTime = clock();
        sort(arr, n);
        clock_t endTime = clock();
        cout << sortName << " : " << double(endTime - startTime) / CLOCKS_PER_SEC << " s" << endl;

        assert(isSorted(arr, n));

        return;
    }

    // 测试sort排序算法排序arr数组所得到结果的正确性和算法运行时间 -- 确定相同值
    template<typename T>
    void testSort(const string& sortName, void (*sort)(T[], int), T origin[], T src[], int n) {

        clock_t startTime = clock();
        sort(src, n);
        clock_t endTime = clock();
        cout << sortName << " : " << double(endTime - startTime) / CLOCKS_PER_SEC << " s" << endl;

        assert(isSorted(src, n));
        assert(isPermutation(origin, src, n, false));

        return;
    }
};

#endif //SORTTESTHELPER_H
