/*********************************************
 * file:	~\code4b\graph.h                  *
 * remark:implementation of undirected graphs *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <format>
#include <cassert>     // assert
#include <limits>      // std::numeric_limits

#include "graph.h"
#include "dsets.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

// Create a graph with n vertices and no vertices
Graph::Graph(int n) : table(size_t(n) + 1), size{n}, n_edges{0} {
    assert(n >= 1);
}

Graph::Graph(const std::vector<Edge> &V, int n) : Graph{n} {
    for (auto e : V) {
        insertEdge(e);
    }
}

// -- MEMBER FUNCTIONS

// insert undirected edge e
// update weight if edge e is present
void Graph::insertEdge(const Edge &e) {
    assert(e.head >= 1 && e.head <= size);
    assert(e.tail >= 1 && e.tail <= size);

    auto edge_insertion = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        if (auto it = std::find_if(begin(T[e1.head]), end(T[e1.head]),
                                   [e1](const Edge &ed) { return e1.links_same_nodes(ed); });
            it == end(T[e1.head])) {
            T[e1.head].push_back(e1);  // insert new edge e1
            ++n;                       // increment the counter of edges
        } else {
            it->weight = e1.weight;  // update the weight
        }
    };

    edge_insertion(e);
    edge_insertion(e.reverse());
}

// remove undirected edge e
void Graph::removeEdge(const Edge &e) {
    assert(e.head >= 1 && e.head <= size);
    assert(e.tail >= 1 && e.tail <= size);

    auto edgeRemoval = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        auto it = std::find_if(begin(T[e1.head]), end(T[e1.head]),
                               [e1](const Edge &ed) { return e1.links_same_nodes(ed); });

        assert(it != end(T[e1.head]));
        T[e1.head].erase(it);  // remove edge e1
        --n;                   // decrement the counter of edges
    };

    edgeRemoval(e);
    edgeRemoval(e.reverse());
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const {
    std::vector<int> dist(size_t(size) + 1, std::numeric_limits<int>::max());
    std::vector<int> path(size_t(size) + 1, 0);
    std::vector<bool> done(size_t(size) + 1, false);

    // *** TODO ***
    int start = 1;
    dist[start] = 0;
    done[start] = true;
    int v = start;

    while (true) {
        for (const Edge& e : table[v]) {
            int u = e.tail;
            if (!done[u] && dist[u] > e.weight) {
                dist[u] = e.weight;
                path[u] = v;
            }
        }
        v = find_smallest_undone_distance_vertex(done, dist);
        // if no such vertex is found, we're done
        if (v == -1) {
            break;
        }
        done[v] = true;
    }

    int totalWeight = 0;
    for (int i = 1; i <= size; i++) {
        if (path[i] > 0) {
            std::cout << "( " << i << ", " << path[i] << ", " << dist[i] << " )" << std::endl;
            totalWeight += dist[i];
        }
    }
    std::cout << "Total weight = " << totalWeight << std::endl;
}

int Graph::find_smallest_undone_distance_vertex(std::vector<bool>& done, std::vector<int>& dist) const
{
    // find vertex u with smallest dist[u] among all vertices not yet marked as done
    int smallest = std::numeric_limits<int>::max();
    int v = -1;
    for (int i = 1; i <= size; ++i) {
        if (!done[i] && dist[i] < smallest) {
            smallest = dist[i];
            v = i;
        }
    }
    return v;
}

// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const {

    // *** TODO ***
    std::vector<Edge> edges;
    std::vector<Edge> mst_edges;
    for(int v = 1; v < table.size(); v++) {
        for (const Edge& e : table[v]) {
            edges.push_back(e);
        }
    }

    std::make_heap(edges.begin(), edges.end(), std::greater<>());

    DSets sets(n_edges);

    while (edges.size() > 0) {
        std::pop_heap(edges.begin(), edges.end(), std::greater<>());
        Edge e = edges.back();
        edges.pop_back();
        int s1 = sets.find(e.head), s2 = sets.find(e.tail);
        if (s1 != s2) {
            mst_edges.push_back(e);
            sets.join(s1, s2);
        }
    }
    int totalWeight = 0;
    for (const Edge& e : mst_edges) {
        std::cout << "( " << e.head << ", " << e.tail << ", " << e.weight << " )" << std::endl;
        totalWeight += e.weight;
    }
    std::cout << "Total weight = " << totalWeight << std::endl;
}

// print graph
void Graph::printGraph() const {
    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Vertex  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    for (int v = 1; v <= size; v++) {
        std::cout << std::format("{:4} : ", v);
        for (auto const &e : table[v]) {
            std::cout << std::format("({:2}, {:2}) ", e.tail, e.weight);
        }
        std::cout << "\n";
    }
    std::cout << std::format("{:-<66}\n", '-');
}
