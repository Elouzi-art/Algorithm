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
│       ├── data_mining/
│       │   ├── apriori.cpp
│       │   ├── dbscan.cpp
│       │   └── kmeans.c
│       └── games/
│           └── hex/
│               ├── hex_game.cpp
│               └── monte_carlo_ai.cpp
├── docs/
├── LICENSE
└── README.md
```

## Algorithm index

| Algorithm / project | Domain | Implementation |
|---|---|---|
| Dijkstra | Graphs | `src/cpp/graphs/dijkstra.cpp` |
| Depth-First Search (DFS) | Graphs | `src/cpp/graphs/dfs.cpp` |
| Breadth-First Search (BFS) | Graphs | `src/cpp/graphs/bfs.cpp` |
| Hidden Markov Model | Probabilistic models | `src/cpp/probabilistic_models/hidden_markov_model.cpp` |
| Sobel | Image processing | `src/cpp/image_processing/sobel.cpp` |
| Apriori | Data mining | `src/cpp/data_mining/apriori.cpp` |
| DBSCAN | Data mining | `src/cpp/data_mining/dbscan.cpp` |
| K-means (1D) | Data mining | `src/cpp/data_mining/kmeans.c` |
| Hex | Game / graph traversal | `src/cpp/games/hex/hex_game.cpp` |
| Hex Monte Carlo AI | Game AI / Monte Carlo | `src/cpp/games/hex/monte_carlo_ai.cpp` |

## Build


g++ -std=c++17 -Wall -Wextra -O2 src/cpp/probabilistic_models/hidden_markov_model.cpp -o hmm
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/image_processing/sobel.cpp -o sobel
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/data_mining/apriori.cpp -o apriori
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/data_mining/dbscan.cpp -o dbscan
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/games/hex/hex_game.cpp -o hex
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/games/hex/monte_carlo_ai.cpp -o hex_ai

# C11 implementation (math library)
gcc -std=c11 -Wall -Wextra -O2 src/cpp/data_mining/kmeans.c -lm -o kmeans
```

DFS and BFS input format is documented in [docs/graphs/README.md](docs/graphs/README.md). Sobel usage:

```bash
./sobel input.pgm edges.pgm 100
```

The Sobel implementation currently supports ASCII grayscale PGM (P2) images.

## Documentation



## License

MIT
