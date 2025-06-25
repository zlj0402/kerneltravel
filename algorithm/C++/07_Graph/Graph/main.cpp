#include <ctime>
#include "SparseGraph.h"
#include "DenseGraph.h"
#include "ReadGraph.h"
#include "Component.h"
#include "Path.h"
#include "ShortestPath.h"

int main() {

	string filename = "sample1.txt";
	SparseGraph g = SparseGraph(13, false);
	ReadGraph<SparseGraph> readGraph(g, filename);
	g.show();
	cout << endl;

	Path<SparseGraph> dfs(g, 0);
	cout << "DFS : ";
	dfs.showPath(6);

	ShortestPath<SparseGraph> bfs(g, 0);
	cout << "BFS : ";
	bfs.showPath(6);

	return 0;
}

// 算法框架 测试 + 连通分量 测试
//int main() {
//
//	string filename = "sample1.txt";
//
//	SparseGraph g1(13, false);
//	ReadGraph<SparseGraph> readGraph1(g1, filename);
//	Component<SparseGraph> component1(g1);
//	cout << filename << ", Component Count: " << component1.count() << endl;
//	g1.show();
//
//	cout << endl;
//
//	string filename2 = "sample2.txt";
//	DenseGraph g2(6, false);
//	ReadGraph<DenseGraph> readGraph2(g2, filename2);
//	Component<DenseGraph> component2(g2);
//	cout << filename2 << ", Component Count: " << component2.count() << endl;
//	g2.show();
//}

// 稀疏图和稠密图的迭代器测试
//int main() {
//
//	int N = 20;
//	int M = 100;
//
//	srand(time(NULL));
//
//	// Sparse Graph
//	SparseGraph g1(N, false);
//	for (int i = 0; i < M; i++) {
//
//		int a = rand() % N;
//		int b = rand() % N;
//		g1.addEdge(a, b);
//	}
//
//	// 时间复杂度：O(E)
//	for (int v = 0; v < N; v++) {
//
//		cout << v << " : ";
//		SparseGraph::adjIterator adj(g1, v);
//		for (int w = adj.begin(); !adj.end(); w = adj.next())
//			cout << w << " ";
//		cout << endl;
//	}
//
//	cout << endl;
//
//	// DenseGraph Graph
//	DenseGraph g2(N, false);
//	for (int i = 0; i < M; i++) {
//
//		int a = rand() % N;
//		int b = rand() % N;
//		g2.addEdge(a, b);
//	}
//
//	// 时间复杂度：O(V^2)
//	for (int v = 0; v < N; v++) {
//
//		cout << v << " : ";
//		DenseGraph::adjIterator adj(g2, v);
//		for (int w = adj.begin(); !adj.end(); w = adj.next())
//			cout << w << " ";
//		cout << endl;
//	}
//}
