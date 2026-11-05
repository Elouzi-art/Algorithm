# Data Mining Algorithms

This repository includes two complementary data-mining techniques.

## Apriori — association-rule mining

Apriori discovers frequent itemsets in transactional data.

- **Support** measures how often an itemset occurs: (support(X)=count(X)/N).
- The **Apriori property** states that every subset of a frequent itemset must also be frequent.
- Candidate itemsets are generated level by level; candidates with an infrequent subset are pruned.

The implementation prints frequent itemsets and their support. It does not yet generate confidence/lift-ranked association rules.

Compile and run:

```bash
g++ -std=c++17 -Wall -Wextra -O2 cpp/apriori.cpp -o apriori
./apriori
```

## DBSCAN — density-based clustering

DBSCAN groups points in dense regions and labels isolated points as noise.

- `eps`: neighborhood radius.
- `minPoints`: minimum number of points (including the point itself) required to form a dense region.
- A **core point** has at least `minPoints` points in its epsilon-neighborhood.
- A **border point** is reachable from a core point but is not itself a core point.
- A **noise point** is not assigned to any cluster.

Compile and run:

```bash
g++ -std=c++17 -Wall -Wextra -O2 cpp/dbscan.cpp -o dbscan
./dbscan
```

## Complexity

- Apriori can require exponential candidate generation in the worst case; its practical cost depends heavily on transaction count, item count, and minimum support.
- This simple DBSCAN implementation performs a linear scan for each neighborhood query, giving (O(n^2)) time and (O(n)) label storage, excluding the input points.

## Choosing between them

Use Apriori to discover co-occurrence patterns in transaction data. Use DBSCAN to discover clusters in spatial or numeric data without specifying the number of clusters in advance.
