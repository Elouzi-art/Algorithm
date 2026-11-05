#include <iostream>
#include <vector>
#include <stack>
#include <stdexcept>

using Graph = std::vector<std::vector<int>>;

// Iterative DFS: returns vertices reachable from source in discovery order.
std::vector<int> dfs(const Graph& graph, int source) {
    const int n = static_cast<int>(graph.size());
    if (source < 0 || source >= n) {
        throw std::out_of_range("Source vertex is outside the graph.");
    }

    std::vector<bool> visited(n, false);
    std::vector<int> order;
    std::stack<int> pending;
    pending.push(source);

    while (!pending.empty()) {
        const int vertex = pending.top();
        pending.pop();
        if (visited[vertex]) continue;

        visited[vertex] = true;
        order.push_back(vertex);

        // Reverse push order to preserve adjacency-list order during traversal.
        for (auto it = graph[vertex].rbegin(); it != graph[vertex].rend(); ++it) {
            if (!visited[*it]) pending.push(*it);
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
        for (int vertex : dfs(graph, source)) std::cout << vertex << ' ';
        std::cout << '\n';
    } catch (const std::out_of_range& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
