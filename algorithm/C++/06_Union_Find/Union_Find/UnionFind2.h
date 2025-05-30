//
// Created by liangj.zhang on 20/5/2025
//

#ifndef UNIONFIND2_H
#define UNIONFIND2_H

#include <cassert>


namespace UF2 {
	
	class UnionFind {

	private:
		int* parent;
		int count;

	public:
		UnionFind(int count) {

			this->count = count;
			parent = new int[count];

			for (int i = 0; i < count; i++)
				parent[i] = i;
		}

		~UnionFind() {
			delete[] parent;
		}

		int find(int p) {

			assert(p >= 0 && p < count);
			while (p != parent[p]) {
				p = parent[p];
			}
			return p;
		}

		bool isConnected(int p, int q) {
			return find(p) == find(q);
		}

		void unionElements(int p, int q) {

			int pRoot = find(p);
			int qRoot = find(q);

			if (pRoot != qRoot) {
				parent[pRoot] = qRoot;
			}
		}
	};
}

#endif	//UNIONFIND2_H
