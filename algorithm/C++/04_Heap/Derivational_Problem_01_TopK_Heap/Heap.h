//
// Created by liangj.zhang on 28/4/2025 
// porting on 28/4/2025: support two heap ways for sort Kth element in an array
//

#ifndef HEAP_H
#define HEAP_H

#include <cassert>

template<typename T>
class Heap {

private:
    T* data;
    int capacity;
    int count;

    bool (*Cmp)(T e1, T e2);

    static bool Cmp0(T e1, T e2) {
        return e1 < e2;
    }
    
    static bool Cmp1(T e1, T e2) {
        return e1 > e2;
    }

public:
    // 默认构造函数
    Heap() : capacity(0), count(0), data(nullptr), Cmp(nullptr) { }

    // 只决定什么类型的堆
    Heap(bool maxHeap) : capacity(0), count(0), data(nullptr) {
        
        Cmp = maxHeap ? Cmp0 : Cmp1;
    }

    // 构造函数, 构造一个空堆, 可容纳capacity个元素
    Heap(int capacity, bool maxHeap) : capacity(capacity), count(0) {

        data = new T[capacity + 1];
        Cmp = maxHeap ? Cmp0 : Cmp1;
    }

    // 构造函数, 通过一个给定数组创建一个最大堆
    // 该构造堆的过程, 时间复杂度为O(n)
    Heap(T arr[], int n, bool maxHeap) : capacity(n), count(n) {

        data = new T[n + 1];
        // capacity = n;
        for (int i = 0; i < n; i++)
            data[i + 1] = arr[i];
        // count = n;

        Cmp = maxHeap ? Cmp0 : Cmp1;

        for (int i = count >> 1; i >= 1; i--)
            shiftDown(i);
    }

    ~Heap() {
        if (data)
            delete[] data;
    }

    // 返回堆中的元素个数
    int size() const {
        return count;
    }

    // 为只决定堆类型的构造，进行初始化
    void setCapacity(int capacity) {
        
        assert(this->capacity == 0);
        this->capacity = capacity;
        data = new T[capacity + 1];
    }

    // 为只决定堆类型的构造，重新进行堆排 topK，重置堆
    void resetHeap(int capacity) {
        
        count = 0;
        this->capacity = 0;

        if (data)
            delete[] data;

        setCapacity(capacity);
    }

    // 获取堆数组的容量
    int getCapacity() const {
        return capacity;
    }

    // 返回一个布尔值, 表示堆中是否为空
    bool isEmpty() const {
        return count == 0;
    }

    // compare with top element & insert it
    void topInsert(T item) {
        
        if (count + 1 <= capacity) {
            insert(item);
        }
        else {
            // Cmp -> 大顶堆(left < right): 为了找 Min(升序数组) top K，堆顶为堆中最大值；item 比堆顶小才插入；
            // Cmp -> 小顶堆(left > right): 为了找 Max(降序数组) top K, 堆顶为堆中最小值；item 比堆顶大才插入；
            if (Cmp(data[1], item))
                return;
            data[1] = item;
            shiftDown(1);
        }
    }

    // 向最大堆中插入一个新的元素 T
    void insert(T item) {

        assert(count + 1 <= capacity);

        data[++count] = item;
        shiftUp(count);
    }

    // 获取堆中的堆顶元素
    T getTop() const {
        
        assert(count > 0);
        return data[1];
    }

    // 从最大堆中取出堆顶元素, 即堆中所存储的最大数据
    T pop() {

        assert(count > 0);

        T top = data[1];
        swap(data[count--], data[1]);
        shiftDown(1);
        return top;
    }


private:

    void shiftUp(int idx) {

        T tmp = data[idx];
        while (idx > 1 && Cmp(data[idx >> 1], tmp))
        {
            data[idx] = data[idx >> 1];
            idx >>= 1;
        }
        data[idx] = tmp;
    }

    void shiftDown(int idx) {

        T tmp = data[idx];
        int child;
        while ((child = idx << 1) <= count) {

            if (child < count && Cmp(data[child], data[child +1 ])) child++;
            if (Cmp(tmp, data[child])) {
                data[child >> 1] = data[child];
                idx = child;
            }
            else break;
        }
        data[idx] = tmp;
    }
};

#endif
