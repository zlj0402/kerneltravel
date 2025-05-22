//
// Created by liangj.zhang on 13/5/2025
//

#ifndef UNIONFIND_UNIONFIND1_H
#define UNIONFIND_UNIONFIND1_H

#include <cassert>

namespace UF1 {
	
	class UnionFind {

	private:
		int* id;
		int count;

	public:
		UnionFind(int n) {

			count = n;
			id = new int[n];
			// 初始化时，每个数，都是自己一个组
			for (int i = 0; i < n; i++)
				id[i] = i;
		}

		int find(int p) {
			assert(p >= 0 && p < count);
			return id[p];
		}

		bool isConnected(int p, int q) {
			return find(p) == find(q);
		}

		void unionElements(int p, int q) {
			
			int pId = find(p);
			int qId = find(q);

			if (pId == qId)
				return;

			// 这里是将 pId 一组的，都改成 qId
			// 当然将 qId 改成 pId 也是可以的；
			for (int i = 0; i < count; i++)
				if (id[i] == pId)
					id[i] = qId;
		}
	};
}

#endif
