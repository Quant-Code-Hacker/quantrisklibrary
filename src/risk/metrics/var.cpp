#include "quantforge/risk_metric.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "quantforge/errors.hpp"

namespace quantforge {

double compute(VaR, const std::vector<double>& losses, double confidence) {
    if (losses.empty()) {
        throw QuantForgeError("compute(VaR): empty loss vector.");
    }

    // Empirical VaR: the confidence-quantile of the simulated loss
    // distribution. e.g. confidence=0.99 -> the loss such that 99% of
    // simulated outcomes are no worse than this.
    std::vector<double> sorted = losses;
    std::sort(sorted.begin(), sorted.end());

    double rank = confidence * static_cast<double>(sorted.size() - 1);
    std::size_t lower = static_cast<std::size_t>(std::floor(rank));
    std::size_t upper = static_cast<std::size_t>(std::ceil(rank));
    double frac = rank - static_cast<double>(lower);

    if (upper >= sorted.size()) upper = sorted.size() - 1;

    // Linear interpolation between the two bracketing order statistics.
    return sorted[lower] + frac * (sorted[upper] - sorted[lower]);
}

} // namespace quantforge
