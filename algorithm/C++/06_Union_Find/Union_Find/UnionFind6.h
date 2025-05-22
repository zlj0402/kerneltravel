//
// Created by liangj.zhang on 22/5/2025，路径压缩 2
// 在 find() 中使用递归，将每个节点的 parent 都设置为关系树的根节点；
//

#ifndef UNIONFIND6_H
#define UNIONFIND6_H

#include <cassert>


namespace UF6 {

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

			if (p != parent[p])
				parent[p] = find(parent[p]);
			return parent[p];
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

#endif	//UNIONFIND6_H
