//
// Created by liangj.zhang on 26/5/2025
//

#ifndef GRAPH_DENSEGRAPH_H
#define GRAPH_DENSEGRAPH_H

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

// 稠密图 - 邻接矩阵
class DenseGraph {

private:
	int n, m;	// 节点数，边数
	bool directed;
	vector<vector<bool>> g;

public:
	DenseGraph(int n, bool directed) : m(0), directed(directed) {

		assert(n >= 0);
		this->n = n;
		for (int i = 0; i < n; i++)
			g.push_back(vector<bool>(n, false));
	}

	~DenseGraph() {}

	int V() { return n; }
	int E() { return m; }

	void addEdge(int v, int w) {

		if (hasEdge(v, w)) return;

		// 允许有自环边；但不会有重的自环边
		g[v][w] = true;
		if (!directed)
			g[w][v] = true;

		m++;
	}

	bool hasEdge(int v, int w) {

		assert(v >= 0 && v < n);
		assert(w >= 0 && w < n);

		return g[v][w];
	}

	void show() {

		for (int i = 0; i < n; i++) {

			cout << "vertex " << i << ":\t";
			for (int j = 0; j < n; j++)
				if (g[i][j] != 0)
					cout << j << "\t";
			cout << endl;
		}
	}

	class adjIterator {
	private:
		DenseGraph& G;
		int v;
		int index;

	public:
		adjIterator(DenseGraph &graph, int v) : G(graph), index(-1) {

			assert(v >= 0 && v < G.n);
			this->v = v;
		}

		int begin() {

			index = -1;
			return next();
		}

		int next() {

			for (index += 1; index < G.V(); index++)
				if (G.g[v][index])
					return index;
			return -1;
		}

		bool end() {
			return index >= G.V();
		}
	};
};

#endif	//GRAPH_DENSEGRAPH_H
