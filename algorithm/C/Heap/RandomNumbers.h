// 
// Created by liangj.zhang on 11/3/2023
// 

#ifndef TESTFORSET_RANDOMNUMBERS_H
#define TESTFORSET_RANDOMNUMBERS_H

#include <iostream>
//#include <ctime>
#include <cassert>

using namespace std;

namespace RandomNumbers {

	// 生成有n个元素的随机数组，每个元素的随机范围为 [rangeL, rangeR]
	int* getRandomArray(int n, int rangeL, int rangeR) {
		assert(rangeL <= rangeR);
		int* arr = new int[n];
		for (int i = 0; i < n; i++)
			arr[i] = rand() % (rangeR - rangeL + 1) + rangeL;
		return arr;
	}

	// 返回一个 [rangeL, rangeR] 之间的数字
	int getRandomNumber(int rangeL, int rangeR) {
		int l = rangeL < rangeR ? rangeL : rangeR;
		int r = rangeL < rangeR ? rangeR : rangeL;
		return rand() % (r - l + 1) + l;
	}

	template<typename T>
	void printSet(T n_set) {
		cout << endl;
		int i = 0;
		for (auto it : n_set) {
			cout << "\t" << "(" << it->id << ", " << it->value << ")";
			if (++i % 2 == 0) cout << endl;
		}
		cout << endl;
	}
}

#endif
