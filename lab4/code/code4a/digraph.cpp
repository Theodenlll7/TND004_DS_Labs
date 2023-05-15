/*********************************************
 * file:	~\code4a\digraph.cpp              *
 * remark: implementation of directed graphs  *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <limits>  //std::numeric_limits
#include <vector>
#include <cassert>
#include <queue>
#include <format>

#include "digraph.h"

using namespace std;
// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

Digraph::Digraph(int n)
	: table(size_t(n) + 1)
	, size{ n }
	, n_edges{ 0 }
	, dist(size_t(n) + 1)
	, path(size_t(n) + 1)
	, done(size_t(n) + 1) {
	assert(n >= 1);

	// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero
}

// Create a digraph with n vertices and the edges in V
Digraph::Digraph(const std::vector<Edge>& V, int n) : Digraph{ n } {
	for (auto e : V) {
		insertEdge(e);
	}
}

// -- MEMBER FUNCTIONS

// insert directed edge e = (u, v, w)
// update weight w if edge (u, v) is present
void Digraph::insertEdge(const Edge& e) {
	assert(e.head >= 1 && e.head <= size);
	assert(e.tail >= 1 && e.tail <= size);

	// Check if edge e already exists
	if (auto it = std::find_if(begin(table[e.head]), end(table[e.head]),
		[e](const Edge& ed) { return e.links_same_nodes(ed); });
		it == end(table[e.head])) {
		table[e.head].push_back(e);  // insert new edge e
		++n_edges;
	}
	else {
		it->weight = e.weight;  // update the weight
	}
}

// remove directed edge e
void Digraph::removeEdge(const Edge& e) {
	assert(e.head >= 1 && e.head <= size);
	assert(e.tail >= 1 && e.tail <= size);

	auto it = std::find_if(begin(table[e.head]), end(table[e.head]),
		[e](const Edge& ed) { return e.links_same_nodes(ed); });

	assert(it != end(table[e.head]));
	table[e.head].erase(it);
	--n_edges;
}

// construct unweighted single source shortest path-tree for start vertex s
void Digraph::uwsssp(int s) const {
	assert(s >= 1 && s <= size);

	// *** TODO ***
	// dist vector initialized with std::numeric_limits<int>::max()
	for (int v = 1; v <= size; v++) {
		dist[v] = std::numeric_limits<int>::max();
		path[v] = 0;
	}
	dist[s] = 0; //distance of source vertex is zero

	std::queue<int> Q;
	Q.push(s);

	while (!Q.empty()) {
		int v = Q.front();
		Q.pop();

		for (const Edge& e : table[v]) {
			int u = e.tail;

			if (dist[u] == std::numeric_limits<int>::max()) {
				dist[u] = dist[v] + 1;
				path[u] = v;
				Q.push(u);
			}
		}
	}

}

// construct positive weighted single source shortest path-tree for start vertex s
// Dijktra’s algorithm
void Digraph::pwsssp(int s) const {
	assert(s >= 1 && s <= size);

	// *** TODO ***
	// dist vector should be initialized with std::numeric_limits<int>::max()
	for (int v = 1; v <= size; v++) {
		dist[v] = std::numeric_limits<int>::max();
		path[v] = 0;
		done[v] = false;
	}

	dist[s] = 0;
	done[s] = true;
	int v = s;

	while (true) {
		// update dist[v] for all neighbors v of u not yet marked as done
		for (const Edge& e : table[v]) {
			int u = e.tail;

			if (!done[u] && dist[u] > dist[v] + e.weight) {
				dist[u] = dist[v] + e.weight;
				path[u] = v;
			}
		}

		// find vertex u with smallest dist[u] among all vertices not yet marked as done
		int smallest = std::numeric_limits<int>::max();
		v = -1;

		for (int i = 1; i <= size; ++i) {
			if (!done[i] && dist[i] < smallest) {
				smallest = dist[i];
				v = i;
			}
		}

		// if no such vertex is found, we're done
		if (v == -1) {
			break;
		}

		done[v] = true;
	}
}

// print graph
void Digraph::printGraph() const {
	std::cout << std::format("{:-<66}\n", '-');
	std::cout << "Vertex  adjacency lists\n";
	std::cout << std::format("{:-<66}\n", '-');

	for (int v = 1; v <= size; ++v) {
		std::cout << std::format("{:4} : ", v);
		for (auto const& e : table[v]) {
			std::cout << std::format("({:2}, {:2}) ", e.tail, e.weight);
		}
		std::cout << "\n";
	}
	std::cout << std::format("{:-<66}\n", '-');
}

// print shortest path tree for s
void Digraph::printTree() const {
	std::cout << std::format("{:-<22}\n", '-');
	std::cout << "vertex    dist    path\n";
	std::cout << std::format("{:-<22}\n", '-');

	for (int v = 1; v <= size; ++v) {
		std::cout << std::format("{:4} : {:6} {:6}\n", v,
			((dist[v] == std::numeric_limits<int>::max()) ? -1 : dist[v]),
			path[v]);
	}
	std::cout << std::format("{:-<22}\n", '-');
}

// print shortest path from s to t and the corresponding path length
// Hint: consider using recursion
void Digraph::printPath(int t) const {
	assert(t >= 1 && t <= size);

	string s = " (" + to_string(dist[t]) + ")";

	while (path[t] > 0)
	{
		s = to_string(t) + " " + s;
		t = path[t];
	}
	s = " " + to_string(t) + " " + s;

	cout << s;
}
