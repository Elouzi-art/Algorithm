# Algorithms

A multilingual collection of algorithm implementations, experiments, and concise technical notes. The repository is organized by **language** first and **algorithmic domain** second.

## Repository structure

```text
Algorithm/
├── src/
│   └── cpp/
│       ├── graphs/
│       │   ├── dijkstra.cpp
│       │   ├── dfs.cpp
│       │   └── bfs.cpp
│       ├── probabilistic_models/
│       │   └── hidden_markov_model.cpp
│       ├── image_processing/
│       │   └── sobel.cpp
│       └── data_mining/
│           ├── apriori.cpp
│           └── dbscan.cpp
├── docs/
│   ├── graphs/README.md
│   ├── probabilistic_models/hidden_markov_model.md
│   ├── image_processing/sobel.md
│   └── data_mining/README.md
├── tests/       (planned)
├── examples/    (planned)
├── assets/      (planned)
├── LICENSE
└── README.md
```

## Algorithm index

| Algorithm | Domain | Implementation |
|---|---|---|
| Dijkstra | Graphs | `src/cpp/graphs/dijkstra.cpp` |
| Depth-First Search (DFS) | Graphs | `src/cpp/graphs/dfs.cpp` |
| Breadth-First Search (BFS) | Graphs | `src/cpp/graphs/bfs.cpp` |
| Hidden Markov Model | Probabilistic models | `src/cpp/probabilistic_models/hidden_markov_model.cpp` |
| Sobel | Image processing | `src/cpp/image_processing/sobel.cpp` |
| Apriori | Data mining | `src/cpp/data_mining/apriori.cpp` |
| DBSCAN | Data mining | `src/cpp/data_mining/dbscan.cpp` |

## Build

The current C++ examples use **C++17** and standard-library dependencies only.

```bash
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/graphs/dfs.cpp -o dfs
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/graphs/bfs.cpp -o bfs
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/probabilistic_models/hidden_markov_model.cpp -o hmm
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/image_processing/sobel.cpp -o sobel
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/data_mining/apriori.cpp -o apriori
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/data_mining/dbscan.cpp -o dbscan
```

DFS and BFS input format is documented in [docs/graphs/README.md](docs/graphs/README.md). Sobel usage:

```bash
./sobel input.pgm edges.pgm 100
```

The Sobel implementation currently supports ASCII grayscale PGM (P2) images.

## Documentation

See [docs/README.md](docs/README.md) for the documentation index. Each domain page covers the core idea, assumptions, usage, and complexity.

## Contributing

1. Put source code under `src/<language>/<domain>/`.
2. Use descriptive, lowercase file names.
3. Add documentation under the matching `docs/<domain>/` directory.
4. Add sample inputs under `examples/` when required.
5. Add tests under `tests/` for expected and edge-case behavior.
6. Update the algorithm index and build instructions in this README.

## License

MIT
