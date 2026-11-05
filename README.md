# Algorithms

A multilingual collection of algorithm implementations, experiments, and concise technical notes. The repository is organized by **language** first and **algorithmic domain** second.

## Repository structure

```text
Algorithm/
├── src/
│   └── cpp/
│       ├── graphs/
│       │   └── dijkstra.cpp
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
│   ├── data_mining/
│   ├── image_processing/
│   ├── probabilistic_models/
│   └── programming_languages/
├── assets/
│   └── polynomial_roots/
│       └── Polynome_root.zip
├── LICENSE
└── README.md
```

## Algorithm index

| Algorithm / project | Domain | Implementation |
|---|---|---|
| Dijkstra | Graphs | `src/cpp/graphs/dijkstra.cpp` |
| Hidden Markov Model | Probabilistic models | `src/cpp/probabilistic_models/hidden_markov_model.cpp` |
| Sobel | Image processing | `src/cpp/image_processing/sobel.cpp` |
| Apriori | Data mining | `src/cpp/data_mining/apriori.cpp` |
| DBSCAN | Data mining | `src/cpp/data_mining/dbscan.cpp` |
| K-means (1D) | Data mining | `src/cpp/data_mining/kmeans.c` |
| Hex | Game / graph traversal | `src/cpp/games/hex/hex_game.cpp` |
| Hex Monte Carlo AI | Game AI / Monte Carlo | `src/cpp/games/hex/monte_carlo_ai.cpp` |

## Build

The examples use standard C++17 unless otherwise specified. Run commands from the repository root.

```bash
# C++17 algorithms
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/graphs/dijkstra.cpp -o dijkstra
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/probabilistic_models/hidden_markov_model.cpp -o hmm
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/image_processing/sobel.cpp -o sobel
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/data_mining/apriori.cpp -o apriori
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/data_mining/dbscan.cpp -o dbscan
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/games/hex/hex_game.cpp -o hex
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/games/hex/monte_carlo_ai.cpp -o hex_ai

# C11 implementation (math library)
gcc -std=c11 -Wall -Wextra -O2 src/cpp/data_mining/kmeans.c -lm -o kmeans
```

Sobel usage:

```bash
./sobel input.pgm edges.pgm 100
```

The Sobel implementation currently supports ASCII grayscale PGM (P2) images.

## Documentation

See [docs/README.md](docs/README.md) for the documentation index. Notes cover the main concepts, assumptions, usage, and complexity where applicable.

## Repository conventions

1. Put source code under `src/<language>/<domain>/`.
2. Use descriptive, lowercase file names with ASCII characters.
3. Put algorithm documentation under `docs/<domain>/`.
4. Put sample inputs under `examples/` and tests under `tests/` when those are added.
5. Put supporting archives and resources under `assets/<topic>/`.
6. Update this README and the relevant documentation index when adding a new item.

## License

MIT
