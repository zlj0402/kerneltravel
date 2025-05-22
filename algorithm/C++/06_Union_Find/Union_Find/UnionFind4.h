//
// Created by liangj.zhang on 22/5/2025，基于rank的优化
//

#ifndef UNIONFIND4_H
#define UNIONFIND4_H

#include <cassert>


namespace UF4 {

	class UnionFind {

	private:
		int* parent;
		int* rank;	// 存储以当前节点为根的关系树，层高
		int count;

	public:
		UnionFind(int count) {

			this->count = count;
			parent = new int[count];
			rank = new int[count];

			for (int i = 0; i < count; i++) {

				parent[i] = i;
				rank[i] = 1;
			}
		}

		~UnionFind() {

			delete[] parent;
			delete[] rank;
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

			if (pRoot == qRoot)
				return;

			if (rank[pRoot] < rank[qRoot]) {
				parent[pRoot] = qRoot;
			}
			else if (rank[qRoot] < rank[pRoot]) {
				parent[qRoot] = pRoot;
			}
			else {	// rank[qRoot] == rank[pRoot]
				parent[pRoot] = qRoot;	// 此时谁放谁下面都可
				rank[qRoot] += 1;		// 相同层数的关系树合并，相应 rank 要加 1；
			}
		}
	};
}

#endif	//UNIONFIND4_H
