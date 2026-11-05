#include <cmath>
#include <iostream>
#include <vector>

struct Point {
    double x;
    double y;
};

// Cluster labels: -2 = unvisited, -1 = noise, 0..k-1 = cluster IDs.
double distance(const Point& a, const Point& b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

std::vector<int> regionQuery(const std::vector<Point>& points, int index, double eps) {
    std::vector<int> neighbors;
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (distance(points[index], points[i]) <= eps) {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

std::vector<int> dbscan(
    const std::vector<Point>& points,
    double eps,
    int minPoints
) {
    const int UNVISITED = -2;
    const int NOISE = -1;
    std::vector<int> labels(points.size(), UNVISITED);
    int clusterId = 0;

    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (labels[i] != UNVISITED) continue;

        std::vector<int> neighbors = regionQuery(points, i, eps);
        if (static_cast<int>(neighbors.size()) < minPoints) {
            labels[i] = NOISE;
            continue;
        }

        labels[i] = clusterId;
        for (std::size_t cursor = 0; cursor < neighbors.size(); ++cursor) {
            const int candidate = neighbors[cursor];

            if (labels[candidate] == NOISE) {
                labels[candidate] = clusterId;
            }
            if (labels[candidate] != UNVISITED) continue;

            labels[candidate] = clusterId;
            const auto candidateNeighbors = regionQuery(points, candidate, eps);
            if (static_cast<int>(candidateNeighbors.size()) >= minPoints) {
                for (int neighbor : candidateNeighbors) {
                    neighbors.push_back(neighbor);
                }
            }
        }
        ++clusterId;
    }
    return labels;
}

int main() {
    const std::vector<Point> points = {
        {1.0, 1.0}, {1.2, 1.1}, {0.8, 0.9}, {1.1, 0.8},
        {5.0, 5.0}, {5.2, 5.1}, {4.8, 4.9}, {5.1, 4.8},
        {9.0, 1.0}
    };

    const double eps = 0.5;
    const int minPoints = 3;
    const std::vector<int> labels = dbscan(points, eps, minPoints);

    for (std::size_t i = 0; i < points.size(); ++i) {
        std::cout << "Point (" << points[i].x << ", " << points[i].y << ") -> ";
        if (labels[i] == -1) {
            std::cout << "noise";
        } else {
            std::cout << "cluster " << labels[i];
        }
        std::cout << '\n';
    }
}
