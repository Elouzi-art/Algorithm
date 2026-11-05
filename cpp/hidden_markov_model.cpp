#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using Matrix = std::vector<std::vector<double>>;

// Computes log(sum(exp(values))) without avoidable underflow.
double logSumExp(const std::vector<double>& values) {
    const double maximum = *std::max_element(values.begin(), values.end());
    if (maximum == -std::numeric_limits<double>::infinity()) {
        return maximum;
    }

    double sum = 0.0;
    for (double value : values) {
        sum += std::exp(value - maximum);
    }
    return maximum + std::log(sum);
}

double safeLog(double probability) {
    if (probability < 0.0 || probability > 1.0) {
        throw std::invalid_argument("Probabilities must be in [0, 1].");
    }
    return probability == 0.0
        ? -std::numeric_limits<double>::infinity()
        : std::log(probability);
}

void validateModel(
    const std::vector<double>& initial,
    const Matrix& transition,
    const Matrix& emission
) {
    const std::size_t states = initial.size();

    if (states == 0 || transition.size() != states || emission.size() != states) {
        throw std::invalid_argument("Inconsistent or empty HMM dimensions.");
    }

    const std::size_t symbols = emission.front().size();
    if (symbols == 0) {
        throw std::invalid_argument("The emission matrix cannot be empty.");
    }

    auto isDistribution = [](const std::vector<double>& row) {
        double sum = 0.0;
        for (double value : row) {
            if (value < 0.0 || value > 1.0) return false;
            sum += value;
        }
        return std::abs(sum - 1.0) < 1e-9;
    };

    if (!isDistribution(initial)) {
        throw std::invalid_argument("Initial probabilities must sum to 1.");
    }

    for (std::size_t i = 0; i < states; ++i) {
        if (transition[i].size() != states || !isDistribution(transition[i])) {
            throw std::invalid_argument("Each transition row must sum to 1.");
        }
        if (emission[i].size() != symbols || !isDistribution(emission[i])) {
            throw std::invalid_argument("Each emission row must sum to 1.");
        }
    }
}

// Forward algorithm: log P(observations | model).
double forwardLogLikelihood(
    const std::vector<double>& initial,
    const Matrix& transition,
    const Matrix& emission,
    const std::vector<int>& observations
) {
    validateModel(initial, transition, emission);
    if (observations.empty()) {
        throw std::invalid_argument("At least one observation is required.");
    }

    const std::size_t states = initial.size();
    const std::size_t symbols = emission.front().size();
    std::vector<double> alpha(states);

    for (int observation : observations) {
        if (observation < 0 || static_cast<std::size_t>(observation) >= symbols) {
            throw std::invalid_argument("Observation index is out of range.");
        }
    }

    for (std::size_t s = 0; s < states; ++s) {
        alpha[s] = safeLog(initial[s]) + safeLog(emission[s][observations[0]]);
    }

    for (std::size_t t = 1; t < observations.size(); ++t) {
        std::vector<double> next(states);
        for (std::size_t j = 0; j < states; ++j) {
            std::vector<double> candidates(states);
            for (std::size_t i = 0; i < states; ++i) {
                candidates[i] = alpha[i] + safeLog(transition[i][j]);
            }
            next[j] = logSumExp(candidates) + safeLog(emission[j][observations[t]]);
        }
        alpha = std::move(next);
    }

    return logSumExp(alpha);
}

// Viterbi algorithm: most likely hidden-state sequence.
std::vector<int> viterbi(
    const std::vector<double>& initial,
    const Matrix& transition,
    const Matrix& emission,
    const std::vector<int>& observations
) {
    validateModel(initial, transition, emission);
    if (observations.empty()) {
        throw std::invalid_argument("At least one observation is required.");
    }

    const std::size_t states = initial.size();
    const std::size_t symbols = emission.front().size();
    for (int observation : observations) {
        if (observation < 0 || static_cast<std::size_t>(observation) >= symbols) {
            throw std::invalid_argument("Observation index is out of range.");
        }
    }

    Matrix score(observations.size(), std::vector<double>(states));
    std::vector<std::vector<int>> previous(
        observations.size(), std::vector<int>(states, -1)
    );

    for (std::size_t s = 0; s < states; ++s) {
        score[0][s] = safeLog(initial[s]) + safeLog(emission[s][observations[0]]);
    }

    for (std::size_t t = 1; t < observations.size(); ++t) {
        for (std::size_t j = 0; j < states; ++j) {
            double best = -std::numeric_limits<double>::infinity();
            for (std::size_t i = 0; i < states; ++i) {
                const double candidate = score[t - 1][i] + safeLog(transition[i][j]);
                if (candidate > best) {
                    best = candidate;
                    previous[t][j] = static_cast<int>(i);
                }
            }
            score[t][j] = best + safeLog(emission[j][observations[t]]);
        }
    }

    int lastState = 0;
    for (std::size_t s = 1; s < states; ++s) {
        if (score.back()[s] > score.back()[lastState]) {
            lastState = static_cast<int>(s);
        }
    }

    std::vector<int> path(observations.size());
    path.back() = lastState;
    for (std::size_t t = observations.size() - 1; t > 0; --t) {
        path[t - 1] = previous[t][path[t]];
    }
    return path;
}

int main() {
    const std::vector<std::string> states = {"Rainy", "Sunny"};
    const std::vector<std::string> symbols = {"walk", "shop", "clean"};

    const std::vector<double> initial = {0.6, 0.4};
    const Matrix transition = {
        {0.7, 0.3},
        {0.4, 0.6}
    };
    const Matrix emission = {
        {0.1, 0.4, 0.5},
        {0.6, 0.3, 0.1}
    };
    const std::vector<int> observations = {0, 1, 2};

    try {
        const double logLikelihood =
            forwardLogLikelihood(initial, transition, emission, observations);
        const std::vector<int> path =
            viterbi(initial, transition, emission, observations);

        std::cout << std::fixed << std::setprecision(6);
        std::cout << "P(observations | model) = "
                  << std::exp(logLikelihood) << '\n';
        std::cout << "Most likely hidden states: ";
        for (int state : path) {
            std::cout << states[state] << ' ';
        }
        std::cout << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
