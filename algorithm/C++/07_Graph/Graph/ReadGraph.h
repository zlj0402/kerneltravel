//
// refered to liuyubobobo, created on 28/5/2025
//

#ifndef GRAPH_READGRAPH_H
#define GRAPH_READGRAPH_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

using std::ifstream;
using std::string;
using std::stringstream;
using std::getline;

template<typename Graph>
class ReadGraph {

public:
	ReadGraph(Graph& graph, const string& filename) {
	
		ifstream file(filename);
		string line;
		int V, E;

		assert(file.is_open());
		assert(getline(file, line));

		stringstream ss(line);
		ss >> V >> E;

		assert(V == graph.V());

		for (int i = 0; i < E; i++) {

			assert(getline(file, line));
			//stringstream ss(line);
			ss.clear();
			ss.str(line);

			int a, b;
			ss >> a >> b;

			assert(a >= 0 && a < V);
			assert(b >= 0 && b < V);
			graph.addEdge(a, b);
		}
	}
};

#endif
