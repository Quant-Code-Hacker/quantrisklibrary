#include "risk/convergence.hpp"

#include <algorithm>
#include <cmath>

namespace quantforge::risk {

ConvergenceResult check_convergence(const std::vector<double>& losses_so_far,
                                     double confidence,
                                     double target_relative_std_error) {
    if (losses_so_far.size() < 200) {
        // Too few samples for the standard-error estimate to be meaningful.
        return ConvergenceResult{false, 1.0};
    }

    std::vector<double> sorted = losses_so_far;
    std::sort(sorted.begin(), sorted.end());

    double rank = confidence * static_cast<double>(sorted.size() - 1);
    std::size_t threshold_idx = static_cast<std::size_t>(std::floor(rank));
    double var_threshold = sorted[threshold_idx];

    // Collect the tail (losses >= VaR threshold) and compute the standard
    // error of their mean — this mean is the CVaR estimate, and its
    // stability is a reasonable proxy for whether VaR itself has converged
    // too, since both come from the same tail region.
    std::vector<double> tail;
    for (double loss : sorted) {
        if (loss >= var_threshold) tail.push_back(loss);
    }

    if (tail.size() < 30) {
        // Tail sample too small (e.g. very high confidence, few sims) —
        // keep simulating.
        return ConvergenceResult{false, 1.0};
    }

    double mean = 0.0;
    for (double v : tail) mean += v;
    mean /= static_cast<double>(tail.size());

    double variance = 0.0;
    for (double v : tail) variance += (v - mean) * (v - mean);
    variance /= static_cast<double>(tail.size() - 1);

    double std_error = std::sqrt(variance / static_cast<double>(tail.size()));
    double relative_std_error = (mean != 0.0) ? std::abs(std_error / mean) : std_error;

    ConvergenceResult result;
    result.relative_std_error = relative_std_error;
    result.converged = relative_std_error <= target_relative_std_error;
    return result;
}

} // namespace quantforge::risk
