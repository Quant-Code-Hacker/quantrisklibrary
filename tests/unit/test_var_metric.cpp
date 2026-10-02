#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include "quantforge/risk_metric.hpp"

int main() {
    using namespace quantforge;

    // 100 losses: 0, 1, 2, ..., 99. The 99th percentile (confidence=0.99)
    // should land near the top of the distribution — verifiable exactly
    // since it's a uniform integer sequence.
    std::vector<double> losses;
    for (int i = 0; i < 100; ++i) losses.push_back(static_cast<double>(i));

    double var99 = compute(VaR{}, losses, 0.99);
    // rank = 0.99 * 99 = 98.01 -> interpolated between losses[98]=98 and losses[99]=99
    assert(std::abs(var99 - 98.01) < 1e-9);

    double var50 = compute(VaR{}, losses, 0.50);
    // rank = 0.50 * 99 = 49.5 -> interpolated between losses[49]=49 and losses[50]=50
    assert(std::abs(var50 - 49.5) < 1e-9);

    // CVaR at a given confidence must be >= VaR at that same confidence,
    // since it's the mean of the tail beyond the VaR threshold.
    double cvar99 = compute(CVaR{}, losses, 0.99);
    assert(cvar99 >= var99);

    std::cout << "test_var_metric: all assertions passed\n";
    return 0;
}
