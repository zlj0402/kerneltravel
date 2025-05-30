//
// Created by liangj.zhang on 13/5/2025
//

#ifndef UNIONFIND_UNIONFINDTESTHELPER_H
#define UNIONFIND_UNIONFINDTESTHELPER_H

#include <iostream>
#include <ctime>
#include "UnionFind1.h"
#include "UnionFind2.h"
#include "UnionFind3.h"
#include "UnionFind4.h"
#include "UnionFind5.h"
#include "UnionFind6.h"

using std::cout;
using std::endl;

namespace UnionFindTestHelper {
	
	void testUF1(int n) {
		
		srand(time(nullptr));
		UF1::UnionFind uf = UF1::UnionFind(n);

		time_t startTime = clock();

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.unionElements(a, b);
		}

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.isConnected(a, b);
		}
		time_t endTime = clock();

		cout << "UF1, " << 2 * n << " ops, " << double(endTime - startTime) / CLOCKS_PER_SEC << " s" << endl;
	}

	void testUF2(int n) {

		srand(time(nullptr));
		UF2::UnionFind uf = UF2::UnionFind(n);

		time_t startTime = clock();

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.unionElements(a, b);
		}

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.isConnected(a, b);
		}
		time_t endTime = clock();

		cout << "UF2, " << 2 * n << " ops, " << double(endTime - startTime) / CLOCKS_PER_SEC << " s" << endl;
	}

	void testUF3(int n) {

		srand(time(nullptr));
		UF3::UnionFind uf = UF3::UnionFind(n);

		time_t startTime = clock();

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.unionElements(a, b);
		}

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.isConnected(a, b);
		}
		time_t endTime = clock();

		cout << "UF3, " << 2 * n << " ops, " << double(endTime - startTime) / CLOCKS_PER_SEC << " s" << endl;
	}

	void testUF4(int n) {

		srand(time(nullptr));
		UF4::UnionFind uf = UF4::UnionFind(n);

		time_t startTime = clock();

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.unionElements(a, b);
		}

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.isConnected(a, b);
		}
		time_t endTime = clock();

		cout << "UF4, " << 2 * n << " ops, " << double(endTime - startTime) / CLOCKS_PER_SEC << " s" << endl;
	}

	void testUF5(int n) {

		srand(time(nullptr));
		UF5::UnionFind uf = UF5::UnionFind(n);

		time_t startTime = clock();

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.unionElements(a, b);
		}

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.isConnected(a, b);
		}
		time_t endTime = clock();

		cout << "UF5, " << 2 * n << " ops, " << double(endTime - startTime) / CLOCKS_PER_SEC << " s" << endl;
	}

	void testUF6(int n) {

		srand(time(nullptr));
		UF6::UnionFind uf = UF6::UnionFind(n);

		time_t startTime = clock();

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.unionElements(a, b);
		}

		for (int i = 0; i < n; i++) {
			int a = rand() % n;
			int b = rand() % n;
			uf.isConnected(a, b);
		}
		time_t endTime = clock();

		cout << "UF6, " << 2 * n << " ops, " << double(endTime - startTime) / CLOCKS_PER_SEC << " s" << endl;
	}
}

#endif	//UNIONFIND_UNIONFINDTESTHELPER_H
