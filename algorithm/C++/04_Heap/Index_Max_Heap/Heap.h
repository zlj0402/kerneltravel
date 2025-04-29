//
// Created by liangj.zhang on 20/4/2025 -- last updated on 22/4/2025 
// Partially referenced from liuyubobobo's course on GitHub
// URL: https://github.com/liuyubobobo/Play-with-Algorithms/blob/master/04-Heap/Course%20Code%20(C%2B%2B)/09-Index-Heap-Advance/main.cpp
//

#ifndef HEAP_H
#define HEAP_H

#include <string>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>

template<typename Item>
class IndexMaxHeap {

private:
    Item* data;
    int* indexes;
    int* reverse;

    const int capacity;
    int count;

public:
    // 构造函数, 构造一个空堆, 可容纳capacity个元素
    IndexMaxHeap(int capacity) : capacity(capacity), count(0) {

        data = new Item[capacity + 1];
        indexes = new int[capacity + 1];
        reverse = new int[capacity + 1];
        for (int i = 0; i <= capacity; i++)
            reverse[i] = 0;
    }

    ~IndexMaxHeap() {
        delete[] data;
        delete[] indexes;
        delete[] reverse;
    }

    // 返回堆中的元素个数
    int size() const {
        return count;
    }

    // 返回一个布尔值, 表示堆中是否为空
    bool isEmpty() const {
        return count == 0;
    }

    // 传入的 i 对用户而言，是从 0 索引的; 在 data[] 中，用户视角的下标
    void insert(int i, Item item) {

        assert(count + 1 <= capacity);
        assert(i + 1 >= 1 && i + 1 <= capacity);

        // 再插入一个新元素前,还需要保证索引i所在的位置是没有元素的。
        assert(!contains(i));

        data[++i] = item;   // ++i 是为了用户视角的 i 在 Heap 当中真正起始的位置要，右偏移1
        indexes[++count] = i;
        reverse[i] = count; // 记录 item 元素在 indexes 中的位置，虽然后面马上就可能在 shiftUp 中改变;

        shiftUp(count);
    }

    // 从最大堆中取出堆顶元素, 即堆中所存储的最大数据
    Item extractMax() {

        assert(count > 0);

        Item top = data[indexes[1]];

        std::swap(indexes[count], indexes[1]);
        reverse[indexes[1]] = 1;
        reverse[indexes[count--]] = 0;

        shiftDown(1);
        return top;
    }

    int extractMaxIndex() {

        assert(count > 0);

        int ret = indexes[1] - 1;
        Item top = data[indexes[1]];

        std::swap(indexes[count], indexes[1]); // count--，但 data 当中该值还是存在的
        reverse[indexes[1]] = 1;
        reverse[indexes[count--]] = 0;

        shiftDown(1);
        return ret;
    }

    // 获取最大索引堆中的堆顶元素
    Item getMax() {

        assert(count > 0);
        return data[indexes[1]];
    }

    // 获取最大索引堆中的堆顶元素的索引
    int getMaxIndex() {

        assert(count > 0);
        return indexes[1] - 1;
    }

    bool contains(int i) {

        assert(i + 1 >= 1 && i + 1 <= capacity);
        return reverse[i + 1] != 0;
    }

    // 获取最大索引堆中索引为i的元素
    Item getItem(int i) {

        assert(contains(i));
        return data[i + 1];
    }

    // 将最大索引堆中索引为i的元素修改为newItem
    // 遍历：O(n)，heapify：O(logn)，相加还是O(n)
    // change -> O(n)，所有元素都 change，那就是 O(n^2)
    void change(int i, Item newItem) {

        assert(contains(i));
        
        i += 1;
        data[i] = newItem;

        int j = reverse[i];
        shiftUp(j);
        shiftDown(j);
    }

private:

    // idx -> in indexes
    void shiftUp(int idx) {

        Item tmp = data[indexes[idx]];
        int tmp_idx = indexes[idx];
        while (idx > 1 && (data[indexes[idx >> 1]] < tmp))
        {
            indexes[idx] = indexes[idx >> 1];
            // idx/idx>>1 -> indexes当中的下标；
            // indexes[idx >> 1] -> indexes[] 中下标为 idx>>1 对应的 data 元素在 data[] 中的位置（下标）；
            // reverse[indexes[idx >> 1]] -> indexes[] 中下标为 idx>>1 对应的 data 元素在 indexes[] 中的位置（下标）== idx>>1；
            reverse[indexes[idx]] = idx;    // 原来错误的写法：reverse[indexes[idx >> 1]] = idx >> 1;
            idx >>= 1;
        }
        indexes[idx] = tmp_idx;
        reverse[indexes[idx]] = idx;
    }

    void shiftDown(int idx) {

        Item tmp = data[indexes[idx]];
        int tmp_idx = indexes[idx];
        int child;
        while ((child = idx << 1) <= count) {

            if (child < count && data[indexes[child + 1]] > data[indexes[child]]) child++;
            if (data[indexes[child]] > tmp) {
                indexes[child >> 1] = indexes[child];
                reverse[indexes[child >> 1]] = child >> 1;
                idx = child;
            }
            else break;
        }
        indexes[idx] = tmp_idx;
        reverse[indexes[idx]] = idx;
    }

public:
    // 测试索引堆中的索引数组index和反向数组reverse
    // 注意:这个测试在向堆中插入元素以后, 不进行extract操作有效
    bool testIndexesAndReverseIndexes() {

        int* copyIndexes = new int[count + 1];
        int* copyReverseIndexes = new int[count + 1];

        for (int i = 0; i <= count; i++) {
            copyIndexes[i] = indexes[i];
            copyReverseIndexes[i] = reverse[i];
        }

        copyIndexes[0] = copyReverseIndexes[0] = 0;
        std::sort(copyIndexes, copyIndexes + count + 1);
        std::sort(copyReverseIndexes, copyReverseIndexes + count + 1);

        // 在对索引堆中的索引和反向索引进行排序后,
        // 两个数组都应该正好是1...count这count个索引
        bool res = true;
        for (int i = 1; i <= count; i++)
            if (copyIndexes[i - 1] + 1 != copyIndexes[i] ||
                copyReverseIndexes[i - 1] + 1 != copyReverseIndexes[i]) {
                res = false;
                break;
            }

        delete[] copyIndexes;
        delete[] copyReverseIndexes;

        if (!res) {
            std::cout << "Error!" << std::endl;
            return false;
        }

        for (int i = 1; i <= count; i++)
            if (reverse[indexes[i]] != i) {
                std::cout << "Error 2" << std::endl;
                return false;
            }

        return true;
    }
};

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> MaxHeap <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

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
        std::swap(data[count--], data[1]);
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
        std::cout << "This print function can only work for less than 100 int";
        return;
    }

    // 我们的testPrint只能处理整数信息
    if (typeid(Item) != typeid(int)) {
        std::cout << "This print function can only work for int item";
        return;
    }

    std::cout << "The max heap size is: " << size() << std::endl;
    std::cout << "Data in the max heap: ";
    for (int i = 1; i <= size(); i++) {
        // 我们的testPrint要求堆中的所有整数在[0, 100)的范围内
        assert(data[i] >= 0 && data[i] < 100);
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

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
        std::string line1 = std::string(max_level_number * 3 - 1, ' ');

        int cur_level_number = std::min(count - int(pow(2, level)) + 1, int(pow(2, level)));
        bool isLeft = true;
        for (int index_cur_level = 0; index_cur_level < cur_level_number; index++, index_cur_level++) {
            putNumberInLine(data[index], line1, index_cur_level, cur_tree_max_level_number * 3 - 1, isLeft);
            isLeft = !isLeft;
        }
        std::cout << line1 << std::endl;

        if (level == max_level - 1)
            break;

        std::string line2 = std::string(max_level_number * 3 - 1, ' ');
        for (int index_cur_level = 0; index_cur_level < cur_level_number; index_cur_level++)
            putBranchInLine(line2, index_cur_level, cur_tree_max_level_number * 3 - 1);
        std::cout << line2 << std::endl;

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
