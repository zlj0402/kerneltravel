//
// Created by liangj.zhang, refered to liuyubobobo, on 21/6/2025
//

#include <iostream>
#include <cassert>
using std::cout;
using std::endl;

#ifndef COMPONENT_H
#define COMPONENT_H

template <typename Graph>
class Component {

private:
	Graph& G;
	int* id;
	bool* visited;
	int ccount;	// 记录连通分量

	void dfs(int v) {

		visited[v] = true;
		id[v] = ccount;

		typename Graph::adjIterator adj(G, v);
		for (int i = adj.begin(); !adj.end(); i = adj.next()) {

			if (!visited[i])
				dfs(i);
		}
	}

public:
	Component(Graph& graph) : G(graph) {

		visited = new bool[G.V()];
		id = new int[G.V()];
		ccount = 0;
		for (int i = 0; i < G.V(); ++i) {

			visited[i] = false;
			id[i] = -1;
		}

		for (int i = 0; i < G.V(); ++i) {

			if (!visited[i]) {

				dfs(i);
				++ccount;
			}
		}
	}

	~Component() {

		delete[] visited;
		delete[] id;
	}

	int count() const {
		return ccount;
	}

	bool isConnected(int v, int w) {

		assert(v >= 0 && v < G.V());
		assert(w >= 0 && w < G.V());
		return id[v] == id[w];
	}
};


#endif	//COMPONENT_H
