# Graph Traversal: DFS and BFS

Both algorithms traverse the vertices reachable from a chosen source in a graph represented by an adjacency list. These implementations use **undirected graphs** and vertices numbered from 0 to n-1.

## Depth-First Search (DFS)

DFS explores one path as far as possible before backtracking. The implementation is iterative and uses an explicit stack, avoiding recursion-depth limitations.

- Data structure: stack (LIFO).
- Typical uses: connected-component exploration, cycle detection, topological-sort foundations, maze exploration.
- Time complexity: **O(V + E)**.
- Auxiliary space: **O(V)**.

## Breadth-First Search (BFS)

BFS explores the graph level by level: the source, its neighbors, then vertices at distance two, and so on.

- Data structure: queue (FIFO).
- Typical uses: shortest path in an unweighted graph, level-order traversal, reachability.
- Time complexity: **O(V + E)**.
- Auxiliary space: **O(V)**.

BFS finds a minimum-edge path in an unweighted graph. DFS does not guarantee a shortest path.

## Build

From the repository root:

```bash
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/graphs/dfs.cpp -o dfs
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/graphs/bfs.cpp -o bfs
```

## Input format

Both programs read:
1. n m: number of vertices and edges.
2. m lines containing an undirected edge u v.
3. The source vertex.

Example input:

```text
6 5
0 1
0 2
1 3
1 4
2 5
0
```

Run with:

```bash
./dfs < graph.txt
./bfs < graph.txt
```

The traversal order depends on neighbor insertion order. Each program visits only vertices reachable from the selected source; disconnected components are not traversed automatically.
