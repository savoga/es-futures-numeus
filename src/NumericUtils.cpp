#include <vector>
#include <numeric>

double computeAverage(const std::vector<uint64_t>& data) {
    if (data.empty()) return 0.0;
    uint64_t sum = std::accumulate(data.begin(), data.end(), uint64_t(0));
    return static_cast<double>(sum) / data.size();
}