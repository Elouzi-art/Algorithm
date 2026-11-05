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
│       └── data_mining/
│           ├── apriori.cpp
│           └── dbscan.cpp
├── docs/
│   ├── probabilistic_models/
│   │   └── hidden_markov_model.md
│   ├── image_processing/
│   │   └── sobel.md
│   └── data_mining/
│       └── README.md
├── tests/
│   └── (algorithm tests, as they are added)
├── examples/
│   └── (input datasets and sample files, as they are added)
├── assets/
│   └── (supporting resources, as they are added)
├── LICENSE
└── README.md
```

Only directories containing tracked files appear in Git. The `tests/`, `examples/`, and `assets/` directories are planned extension points, not currently populated directories.

## Algorithm index

| Algorithm | Domain | Implementation |
|---|---|---|
| Dijkstra | Graphs | `src/cpp/graphs/dijkstra.cpp` |
| Hidden Markov Model | Probabilistic models | `src/cpp/probabilistic_models/hidden_markov_model.cpp` |
| Sobel | Image processing | `src/cpp/image_processing/sobel.cpp` |
| Apriori | Data mining | `src/cpp/data_mining/apriori.cpp` |
| DBSCAN | Data mining | `src/cpp/data_mining/dbscan.cpp` |

## Build

The current C++ examples use **C++17** and standard-library dependencies only.

Run these commands from the repository root:

```bash
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/probabilistic_models/hidden_markov_model.cpp -o hmm
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/image_processing/sobel.cpp -o sobel
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/data_mining/apriori.cpp -o apriori
g++ -std=c++17 -Wall -Wextra -O2 src/cpp/data_mining/dbscan.cpp -o dbscan
```

Sobel usage:

```bash
./sobel input.pgm edges.pgm 100
```

The Sobel implementation currently supports ASCII grayscale PGM (P2) images.

## Documentation

Each domain has a corresponding page under `docs/`. Documentation covers the core idea, assumptions, usage, and complexity.

## Contributing / adding an algorithm

1. Put source code under `src/<language>/<domain>/`.
2. Use descriptive, lowercase file names.
3. Add a short explanation and complexity analysis under `docs/<domain>/`.
4. Add sample inputs under `examples/` when required.
5. Add tests under `tests/` for expected and edge-case behavior.
6. Update the algorithm index and build instructions in this README.

## License

MIT
