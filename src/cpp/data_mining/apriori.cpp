#include <algorithm>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using Itemset = std::vector<std::string>;
using Transaction = std::set<std::string>;

bool contains(const Transaction& transaction, const Itemset& candidate) {
    return std::all_of(candidate.begin(), candidate.end(),
        [&](const std::string& item) { return transaction.count(item) != 0; });
}

std::vector<Itemset> generateCandidates(const std::vector<Itemset>& previous) {
    std::set<Itemset> candidates;
    if (previous.empty()) return {};

    const std::size_t k = previous.front().size();
    for (std::size_t i = 0; i < previous.size(); ++i) {
        for (std::size_t j = i + 1; j < previous.size(); ++j) {
            bool samePrefix = true;
            for (std::size_t p = 0; p + 1 < k; ++p) {
                if (previous[i][p] != previous[j][p]) {
                    samePrefix = false;
                    break;
                }
            }
            if (!samePrefix || previous[i].back() >= previous[j].back()) continue;

            Itemset candidate = previous[i];
            candidate.push_back(previous[j].back());

            // Apriori pruning: every (k-1)-subset must be frequent.
            bool allSubsetsFrequent = true;
            for (std::size_t removed = 0; removed < candidate.size(); ++removed) {
                Itemset subset;
                for (std::size_t p = 0; p < candidate.size(); ++p) {
                    if (p != removed) subset.push_back(candidate[p]);
                }
                if (std::find(previous.begin(), previous.end(), subset) == previous.end()) {
                    allSubsetsFrequent = false;
                    break;
                }
            }
            if (allSubsetsFrequent) candidates.insert(candidate);
        }
    }
    return {candidates.begin(), candidates.end()};
}

int main() {
    const std::vector<Transaction> transactions = {
        {"bread", "milk"},
        {"bread", "diaper", "beer", "eggs"},
        {"milk", "diaper", "beer", "cola"},
        {"bread", "milk", "diaper", "beer"},
        {"bread", "milk", "diaper", "cola"}
    };
    const double minimumSupport = 0.4;
    const int minimumCount = static_cast<int>(
        minimumSupport * transactions.size() + 0.999999
    );

    std::set<std::string> uniqueItems;
    for (const auto& transaction : transactions) {
        uniqueItems.insert(transaction.begin(), transaction.end());
    }

    std::vector<Itemset> level;
    for (const auto& item : uniqueItems) level.push_back({item});

    std::cout << "Minimum support: " << minimumSupport << "\n\n";
    int size = 1;

    while (!level.empty()) {
        std::vector<Itemset> frequent;
        for (const auto& candidate : level) {
            int count = 0;
            for (const auto& transaction : transactions) {
                if (contains(transaction, candidate)) ++count;
            }
            if (count >= minimumCount) {
                frequent.push_back(candidate);
                std::cout << std::string(size * 2, ' ')
                          << "{ ";
                for (const auto& item : candidate) std::cout << item << ' ';
                std::cout << "} support = " << std::fixed << std::setprecision(2)
                          << static_cast<double>(count) / transactions.size()
                          << " (" << count << '/' << transactions.size() << ")\n";
            }
        }

        if (frequent.empty()) break;
        level = generateCandidates(frequent);
        ++size;
    }
}
