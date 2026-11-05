#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>

using Graph = std::vector<std::vector<int>>;

// BFS: returns vertices reachable from source in breadth-first order.
std::vector<int> bfs(const Graph& graph, int source) {
    const int n = static_cast<int>(graph.size());
    if (source < 0 || source >= n) {
        throw std::out_of_range("Source vertex is outside the graph.");
    }

    std::vector<bool> visited(n, false);
    std::vector<int> order;
    std::queue<int> pending;

    visited[source] = true;
    pending.push(source);

    while (!pending.empty()) {
        const int vertex = pending.front();
        pending.pop();
        order.push_back(vertex);

        for (int neighbor : graph[vertex]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true; // Mark on enqueue to prevent duplicates.
                pending.push(neighbor);
            }
        }
    }
    return order;
}

int main() {
    int n, m;
    if (!(std::cin >> n >> m) || n <= 0 || m < 0) {
        std::cerr << "Invalid graph size. Expected n > 0 and m >= 0.\n";
        return 1;
    }

    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        if (!(std::cin >> u >> v) || u < 0 || u >= n || v < 0 || v >= n) {
            std::cerr << "Invalid edge: vertices must be in [0, n-1].\n";
            return 1;
        }
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    int source;
    if (!(std::cin >> source)) {
        std::cerr << "Missing source vertex.\n";
        return 1;
    }

    try {
        for (int vertex : bfs(graph, source)) std::cout << vertex << ' ';
        std::cout << '\n';
    } catch (const std::out_of_range& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
