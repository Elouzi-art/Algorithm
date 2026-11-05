# Algorithms

Collection of algorithmic implementations and experiments in C, C++, Java, and Python.

## Structure

```text
Algorithms/
├── README.md
├── cpp/
│   ├── Dijkstra_Algorithme.cpp
│   ├── hidden_markov_model.cpp
│   ├── sobel.cpp
│   ├── apriori.cpp
│   ├── dbscan.cpp
│   └── ...
├── docs/
│   ├── hidden_markov_model.md
│   ├── sobel.md
│   ├── data_mining.md
│   └── ...
├── assets/
└── LICENSE
```

## Algorithms

| File | Topic | Purpose |
|---|---|---|
| `cpp/hidden_markov_model.cpp` | Probabilistic models | HMM Forward likelihood and Viterbi decoding |
| `cpp/sobel.cpp` | Image processing | Sobel edge detection on P2 PGM images |
| `cpp/apriori.cpp` | Data mining | Frequent itemset mining with Apriori |
| `cpp/dbscan.cpp` | Data mining | Density-based clustering and noise detection |

## Build

Examples use standard C++17 and do not require external libraries.

```bash
g++ -std=c++17 -Wall -Wextra -O2 cpp/hidden_markov_model.cpp -o hmm
g++ -std=c++17 -Wall -Wextra -O2 cpp/sobel.cpp -o sobel
g++ -std=c++17 -Wall -Wextra -O2 cpp/apriori.cpp -o apriori
g++ -std=c++17 -Wall -Wextra -O2 cpp/dbscan.cpp -o dbscan
```

See the `docs/` directory for the concepts, assumptions, usage, and complexity of each implementation.

## Notes

This repository is a personal algorithm and C/C++ learning playground. Implementations are educational examples and may be extended with additional algorithms and tests.

## License

MIT
