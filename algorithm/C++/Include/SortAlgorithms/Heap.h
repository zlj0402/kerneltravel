//
// Created by liangj.zhang on 20/4/2025 -- updated on 21/4/2025 
// Partially referenced from liuyubobobo's course on GitHub
// URL: https://github.com/liuyubobobo/Play-with-Algorithms/blob/master/04-Heap/Course%20Code%20(C%2B%2B)/03-Shift-Up/main.cpp
//

#ifndef HEAP_H
#define HEAP_H

#include <string>
#include <cmath>
#include <cassert>
#include <iostream>

template<typename Item>
class MaxHeap {

public:
    // 构造函数, 构造一个空堆, 可容纳capacity个元素
    MaxHeap(int capacity) : capacity(capacity), count(0) {

        data = new Item[capacity + 1];
    }

    // 构造函数, 通过一个给定数组创建一个最大堆
    // 该构造堆的过程, 时间复杂度为O(n)
    MaxHeap(Item arr[], int n) : capacity(n), count(n) {

        data = new Item[n + 1];
        // capacity = n;
        for (int i = 0; i < n; i++)
            data[i + 1] = arr[i];
        // count = n;

        for (int i = count >> 1; i >= 1; i--)
            shiftDown(i);
    }

    ~MaxHeap() {
        delete[] data;
    }

    // 返回堆中的元素个数
    int size() const {
        return count;
    }

    // 返回一个布尔值, 表示堆中是否为空
    bool isEmpty() const {
        return count == 0;
    }

    // 向最大堆中插入一个新的元素 item
    void insert(Item item) {

        assert(count + 1 <= capacity);

        data[++count] = item;
        shiftUp(count);
    }

    // 从最大堆中取出堆顶元素, 即堆中所存储的最大数据
    Item extractMax() {

        assert(count > 0);

        Item top = data[1];
        swap(data[count--], data[1]);
        shiftDown(1);
        return top;
    }

    static void shiftDown_idx0(Item arr[], int n, int idx) {
        
        Item e = arr[idx];
        int child;          // n - 1是堆数组最后一个下标;
        while ((child = (idx << 1) + 1) <= n - 1) {     // 原来错误的写法：while ((child = idx << 1 + 1) <= n - 1); 加号+优先级高于左移运算符<<
            
            if (child < n - 1 && arr[child + 1] > arr[child]) child++;
            if (arr[child] > e) {
                arr[idx] = arr[child];
                idx = child;
            }
            else break;
        }
        arr[idx] = e;
    }

    // 以树状打印整个堆结构
    void testPrint();

private:
    Item* data;
    const int capacity;
    int count;

    void shiftUp(int idx) {

        Item tmp = data[idx];
        while (idx > 1 && (data[idx >> 1] < tmp))
        {
            data[idx] = data[idx >> 1];
            idx >>= 1;
        }
        data[idx] = tmp;
    }

    void shiftDown(int idx) {

        Item tmp = data[idx];
        int child;
        while ((child = idx << 1) <= count) {

            if (child < count && data[child + 1] > data[child]) child++;
            if (data[child] > tmp) {
                data[child >> 1] = data[child];
                idx = child;
            }
            else break;
        }
        data[idx] = tmp;
    }

    void putNumberInLine(int num, std::string& line, int index_cur_level, int cur_tree_width, bool isLeft);

    void putBranchInLine(std::string& line, int index_cur_level, int cur_tree_width);
};

template<typename Item>
void MaxHeap<Item>::testPrint() {

    // 我们的testPrint只能打印100个元素以内的堆的树状信息
    if (size() >= 100) {
        cout << "This print function can only work for less than 100 int";
        return;
    }

    // 我们的testPrint只能处理整数信息
    if (typeid(Item) != typeid(int)) {
        cout << "This print function can only work for int item";
        return;
    }

    cout << "The max heap size is: " << size() << endl;
    cout << "Data in the max heap: ";
    for (int i = 1; i <= size(); i++) {
        // 我们的testPrint要求堆中的所有整数在[0, 100)的范围内
        assert(data[i] >= 0 && data[i] < 100);
        cout << data[i] << " ";
    }
    cout << endl;
    cout << endl;

    int n = size();
    int max_level = 0;
    int number_per_level = 1;
    while (n > 0) {
        max_level += 1;
        n -= number_per_level;
        number_per_level *= 2;
    }

    int max_level_number = int(pow(2, max_level - 1));
    int cur_tree_max_level_number = max_level_number;
    int index = 1;
    for (int level = 0; level < max_level; level++) {
        string line1 = string(max_level_number * 3 - 1, ' ');

        int cur_level_number = min(count - int(pow(2, level)) + 1, int(pow(2, level)));
        bool isLeft = true;
        for (int index_cur_level = 0; index_cur_level < cur_level_number; index++, index_cur_level++) {
            putNumberInLine(data[index], line1, index_cur_level, cur_tree_max_level_number * 3 - 1, isLeft);
            isLeft = !isLeft;
        }
        cout << line1 << endl;

        if (level == max_level - 1)
            break;

        string line2 = string(max_level_number * 3 - 1, ' ');
        for (int index_cur_level = 0; index_cur_level < cur_level_number; index_cur_level++)
            putBranchInLine(line2, index_cur_level, cur_tree_max_level_number * 3 - 1);
        cout << line2 << endl;

        cur_tree_max_level_number /= 2;
    }
}

template<typename Item>
void MaxHeap<Item>::putNumberInLine(int num, std::string& line, int index_cur_level, int cur_tree_width, bool isLeft) {

    int sub_tree_width = (cur_tree_width - 1) / 2;
    int offset = index_cur_level * (cur_tree_width + 1) + sub_tree_width;
    assert(offset + 1 < line.size());
    if (num >= 10) {
        line[offset + 0] = '0' + num / 10;
        line[offset + 1] = '0' + num % 10;
    }
    else {
        if (isLeft)
            line[offset + 0] = '0' + num;
        else
            line[offset + 1] = '0' + num;
    }
}

template<typename Item>
void MaxHeap<Item>::putBranchInLine(std::string& line, int index_cur_level, int cur_tree_width) {

    int sub_tree_width = (cur_tree_width - 1) / 2;
    int sub_sub_tree_width = (sub_tree_width - 1) / 2;
    int offset_left = index_cur_level * (cur_tree_width + 1) + sub_sub_tree_width;
    assert(offset_left + 1 < line.size());
    int offset_right = index_cur_level * (cur_tree_width + 1) + sub_tree_width + 1 + sub_sub_tree_width;
    assert(offset_right < line.size());

    line[offset_left + 1] = '/';
    line[offset_right + 0] = '\\';
}

#endif	//HEAP_H
