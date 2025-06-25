//
// Created by liangj.zhang on 26/5/2025
//

#ifndef GRAPH_SPARSEGRAPH_H
#define GRAPH_SPARSEGRAPH_H

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

// 稀疏图 - 链接表
class SparseGraph {

private:
	int n, m;	// n 是节点数，m 是边数
	bool directed;
	vector<vector<int>> g;

public:
	// 邻接表的实现，这里使用的是 vector<vector<int>>
	// 使用链表也很方便，在删除一个连接点时比较方便；
	// 这里没有涉及删除，直接用的是 vector<vector<int>>
	SparseGraph(int n, bool directed) : m(0), directed(directed) {
		
		assert(n >= 0);
		this->n = n;
		for (int i = 0; i < n; i++)
			// g初始化为n个空的vector, 表示每一个g[i]都为空, 即没有任和边
			g.push_back(vector<int>());
	}

	~SparseGraph() {}

	int V() { return n; }
	int E() { return m; }

	// 向图中添加一个边
	void addEdge(int v, int w) {

		assert(v >= 0 && v < n);
		assert(w >= 0 && w < n);

		// 理论上 (v,w) 只会被调用一次；不然会有重边，及重的自环边；
		g[v].push_back(w);
		if (v != w && !directed)	// v != w => 避免有重的自环边
			g[w].push_back(v);

		m++;
	}

	// 验证图中是否有从v到w的边
	// 暂时并没有使用，时间效率低
	bool hasEdge(int v, int w) {	// 用来避免平行边

		assert(v >= 0 && v < n);
		assert(w >= 0 && w < n);

		for (int i = 0; i < g[v].size(); i++)
			if (g[v][i] == w)
				return true;
		return false;
	}

	void show() {

		for (int i = 0; i < n; i++) {

			cout << "vertex " << i << ":\t";
			for (int j = 0; j < g[i].size(); j++)
				cout << g[i][j] << "\t";
			cout << endl;
		}
	}

	class adjIterator {
	private:
		SparseGraph& G;
		int v;
		int index;

	public:
		adjIterator(SparseGraph &graph, int v) : G(graph), index(0) {
			
			assert(v >= 0 && v < G.n);
			this->v = v;
		}

		int begin() {

			index = 0;
			if (G.g[v].size())
				return G.g[v][index];
			return -1;
		}

		int next() {
			
			index++;
			if (index < G.g[v].size())
				return G.g[v][index];
			return -1;
		}

		bool end() {
			return index >= G.g[v].size();
		}
	};
};

#endif
