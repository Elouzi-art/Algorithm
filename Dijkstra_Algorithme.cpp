/**
 * @file shortest_path.cpp
 * @brief Implementation of an undirected weighted graph and Dijkstra's algorithm.
 *
 * This program demonstrates graph generation, shortest path computation,
 * and average path length analysis using a priority‑queue based Dijkstra.
 */

#include <iostream>      // std::cout, std::endl
#include <vector>        // std::vector
#include <cstdlib>       // rand()
#include <queue>         // std::priority_queue
#include <limits>        // std::numeric_limits
#include <functional>    // std::greater
#include <algorithm>     // std::reverse

using namespace std;

/** Global constant for graph size (unused but kept for legacy). */
const int N = 50;
/** Sentinel value indicating no edge. */
const double NO_EDGE = -1.0;

// ============================================================================
//  Graph Class
// ============================================================================

/**
 * @class Graph
 * @brief Represents an undirected weighted graph using an adjacency list.
 *
 * The graph is stored as a vector of vectors of (neighbour, weight) pairs.
 * It supports edge addition, removal, weight query, and random generation.
 */
class Graph {
private:
    int Vertices;                          ///< Number of vertices.
    int Edges;                             ///< Number of edges.
    vector<vector<pair<int, double>>> AdjacencyList; ///< Adjacency list.

public:
    /** Constructor: initialises a graph with @p numberOfVertices isolated vertices. */
    explicit Graph(int numberOfVertices);

    /** @return the number of vertices. */
    int V() const;
    /** @return the number of edges. */
    int E() const;

    /**
     * Checks whether edge (x, y) exists and returns the positions of the
     * edge in both adjacency lists.
     * @param x       First vertex.
     * @param y       Second vertex.
     * @param index_x Output: index of y in AdjacencyList[x] (or -1 if not found).
     * @param index_y Output: index of x in AdjacencyList[y] (or -1 if not found).
     * @return true if the edge exists, false otherwise.
     */
    bool adjacent(int x, int y, int &index_x, int &index_y) const;

    /**
     * Adds an undirected edge between x and y with the given cost.
     * @param x    First vertex.
     * @param y    Second vertex.
     * @param cost Weight of the edge.
     * @pre 0 <= x,y < Vertices and no duplicate edge is added.
     */
    void add(int x, int y, double cost);

    /**
     * Removes the undirected edge between x and y, if it exists.
     * @param x First vertex.
     * @param y Second vertex.
     */
    void remove(int x, int y);

    /**
     * Retrieves the weight of edge (x, y).
     * @param x First vertex.
     * @param y Second vertex.
     * @return The edge weight if it exists, otherwise NO_EDGE.
     */
    double get_edge_value(int x, int y) const;

    /**
     * Updates the weight of edge (x, y) to @p value.
     * Does nothing if the edge does not exist.
     * @param x     First vertex.
     * @param y     Second vertex.
     * @param value New weight.
     */
    void set_edge_value(int x, int y, double value);

    /**
     * Returns a copy of the adjacency list of vertex x.
     * @param x The vertex.
     * @return Vector of (neighbour, weight) pairs.
     */
    vector<pair<int, double>> neighbors(int x) const;

    /**
     * Generates a random graph according to the given parameters.
     * For each pair (i,j) with i<j, an edge is added with probability @p density.
     * Its weight is uniformly distributed in [@p minCost, @p maxCost].
     * @param density  Edge probability (0.0 to 1.0).
     * @param minCost  Lower bound for edge weights.
     * @param maxCost  Upper bound for edge weights.
     */
    void generateRandomGraph(double density, double minCost, double maxCost);
};

// --- Graph Implementation ----------------------------------------------------

Graph::Graph(int numberOfVertices)
    : Vertices(numberOfVertices), Edges(0)
{
    AdjacencyList.resize(Vertices);
}

int Graph::V() const { return Vertices; }
int Graph::E() const { return Edges; }

bool Graph::adjacent(int x, int y, int &index_x, int &index_y) const {
    index_x = -1;
    index_y = -1;

    // Search for y in x's list
    for (int i = 0; i < (int)AdjacencyList[x].size(); ++i) {
        if (AdjacencyList[x][i].first == y) {
            index_x = i;
            break;
        }
    }
    // Search for x in y's list
    for (int i = 0; i < (int)AdjacencyList[y].size(); ++i) {
        if (AdjacencyList[y][i].first == x) {
            index_y = i;
            return true;   // both exist => adjacent
        }
    }
    return false;
}

void Graph::add(int x, int y, double cost) {
    // Assumes valid vertices and no duplicate edge
    AdjacencyList[x].push_back({y, cost});
    AdjacencyList[y].push_back({x, cost});
    Edges++;
}

void Graph::remove(int x, int y) {
    int index_x, index_y;
    if (adjacent(x, y, index_x, index_y)) {
        // Erase the entry from x's list
        AdjacencyList[x].erase(AdjacencyList[x].begin() + index_x);
        // Erase the entry from y's list
        AdjacencyList[y].erase(AdjacencyList[y].begin() + index_y);
        Edges--;
    }
}

double Graph::get_edge_value(int x, int y) const {
    int index_x, index_y;
    if (adjacent(x, y, index_x, index_y)) {
        return AdjacencyList[x][index_x].second;
    }
    return NO_EDGE;
}

void Graph::set_edge_value(int x, int y, double value) {
    int index_x, index_y;
    if (adjacent(x, y, index_x, index_y)) {
        AdjacencyList[x][index_x].second = value;
        AdjacencyList[y][index_y].second = value;
    }
}

vector<pair<int, double>> Graph::neighbors(int x) const {
    return AdjacencyList[x];   // return a copy
}

void Graph::generateRandomGraph(double density, double minCost, double maxCost) {
    // Iterate over all unordered vertex pairs
    for (int i = 0; i < Vertices; ++i) {
        for (int j = i + 1; j < Vertices; ++j) {
            double random_density = (rand() % 101) / 100.0;   // [0.0, 1.0]
            if (density > random_density) {
                // Uniform random weight in [minCost, maxCost]
                double random_cost = minCost + (maxCost - minCost) * ((rand() % 101) / 100.0);
                add(i, j, random_cost);
            }
        }
    }
}

// ============================================================================
//  ShortestPath Class
// ============================================================================

/**
 * @class ShortestPath
 * @brief Computes shortest paths from a source vertex using Dijkstra's algorithm.
 *
 * It stores the results (distances and parent tree) for later queries:
 *   - path_size(destination) returns the shortest distance.
 *   - path(destination) reconstructs the actual vertex sequence.
 *   - AVG() computes the average shortest path length from the source
 *     to all reachable vertices.
 */
class ShortestPath {
private:
    Graph &graph;                 ///< Reference to the graph (non‑const for potential modifications).
    vector<double> distance;      ///< Shortest distance from source to each vertex.
    vector<int> parent;           ///< Parent of each vertex in the shortest path tree.
    vector<bool> visited;         ///< Marks vertices already processed by Dijkstra.

public:
    /** Constructor: stores a reference to the graph. */
    explicit ShortestPath(Graph &g);

    /**
     * Runs Dijkstra's algorithm from the given source vertex.
     * @param source The starting vertex.
     * @pre All edge weights must be non‑negative.
     */
    void dijkstra(int source);

    /**
     * Returns the shortest distance from the source to the destination.
     * @param destination The target vertex.
     * @return The distance, or infinity if unreachable.
     */
    double path_size(int destination) const;

    /**
     * Reconstructs the shortest path from source to destination as a vector.
     * @param destination The target vertex.
     * @return Vector of vertices along the path (source first, destination last).
     *         If the destination is unreachable, returns an empty vector.
     */
    vector<int> path(int destination) const;

    /**
     * Computes the average shortest path length from the source over all
     * reachable vertices (excluding the source itself).
     * @return The average distance, or 0.0 if no other vertex is reachable.
     */
    double AVG() const;
};

// --- ShortestPath Implementation ---------------------------------------------

ShortestPath::ShortestPath(Graph &g) : graph(g) {}

void ShortestPath::dijkstra(int source) {
    const double INF = numeric_limits<double>::infinity();

    // Initialise data structures
    distance.assign(graph.V(), INF);
    parent.assign(graph.V(), -1);
    visited.assign(graph.V(), false);

    distance[source] = 0.0;

    // Min‑heap priority queue: pair(distance, vertex)
    priority_queue<pair<double, int>,
                   vector<pair<double, int>>,
                   greater<pair<double, int>>> pq;
    pq.push({0.0, source});

    while (!pq.empty()) {
        double dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // If already finalised, skip duplicate entries
        if (visited[u]) continue;
        visited[u] = true;

        // Relax all outgoing edges
        for (const auto &neighbor : graph.neighbors(u)) {
            int v = neighbor.first;
            double weight = neighbor.second;
            if (!visited[v] && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                parent[v] = u;
                pq.push({distance[v], v});
            }
        }
    }
}

double ShortestPath::path_size(int destination) const {
    return distance[destination];
}

vector<int> ShortestPath::path(int destination) const {
    vector<int> path_dest;
    // If unreachable, distance is INF, but we still build a path
    // that ends at the source? Actually we must handle unreachable.
    if (distance[destination] == numeric_limits<double>::infinity())
        return {};   // empty path

    int tmp = destination;
    while (tmp != -1) {
        path_dest.push_back(tmp);
        if (tmp == 0) break;  // source (we assume source is 0, but we could store it)
        tmp = parent[tmp];
    }
    reverse(path_dest.begin(), path_dest.end());
    return path_dest;
}

double ShortestPath::AVG() const {
    double sum = 0.0;
    int count = 0;
    const double INF = numeric_limits<double>::infinity();

    for (int i = 0; i < graph.V(); ++i) {
        if (i == 0) continue;   // skip the source (assuming source = 0)
        double d = distance[i];
        if (d != INF) {
            sum += d;
            ++count;
        }
    }
    return (count == 0) ? 0.0 : (sum / count);
}

// ============================================================================
//  Main Program
// ============================================================================

int main() {
    // ---- First graph: density 0.2 ----
    Graph g10(50);
    g10.generateRandomGraph(0.2, 1.0, 10.0);

    ShortestPath sp10(g10);
    sp10.dijkstra(0);   // source = vertex 0

    double avg10 = sp10.AVG();
    cout << "Average shortest path (density 0.2): " << avg10 << endl;

    // ---- Second graph: density 0.4 ----
    Graph g20(50);
    g20.generateRandomGraph(0.4, 1.0, 10.0);

    ShortestPath sp20(g20);
    sp20.dijkstra(0);

    double avg20 = sp20.AVG();
    cout << "Average shortest path (density 0.4): " << avg20 << endl;

    return 0;
}