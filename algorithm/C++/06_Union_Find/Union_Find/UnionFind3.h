//
// Created by liangj.zhang on 21/5/2025，基于size的优化
//

#ifndef UNIONFIND3_H
#define UNIONFIND3_H

#include <cassert>


namespace UF3 {

	class UnionFind {

	private:
		int* parent;
		int* size;	// 存储以当前节点为根的关系树中，元素的个数
		int count;

	public:
		UnionFind(int count) {

			this->count = count;
			parent = new int[count];
			size = new int[count];

			for (int i = 0; i < count; i++) {

				parent[i] = i;
				size[i] = 1;
			}
		}

		~UnionFind() {

			delete[] parent;
			delete[] size;
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

			if (size[pRoot] < size[qRoot]) {

				parent[pRoot] = parent[qRoot];
				size[qRoot] += size[pRoot];
			}
			else {

				parent[qRoot] = parent[pRoot];
				size[pRoot] += size[qRoot];
			}
		}
	};
}

#endif	//UNIONFIND3_H
