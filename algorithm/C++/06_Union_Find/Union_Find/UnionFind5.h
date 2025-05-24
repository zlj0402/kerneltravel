//
// Created by liangj.zhang on 22/5/2025，路径压缩 1
// 只在 find() 中添加一句，parent[p] = parent[parent[p]]; 让节点 p 为根的关系树，接在了节点 p 的父亲的父亲；
//

#ifndef UNIONFIND5_H
#define UNIONFIND5_H

#include <cassert>


namespace UF5 {

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

				parent[p] = parent[parent[p]];
				p = parent[p];
			}
			return p;	// return 时 p 一定是 root 节点
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

#endif	//UNIONFIND5_H
