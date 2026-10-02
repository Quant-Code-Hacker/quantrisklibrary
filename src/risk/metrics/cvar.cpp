#include "quantforge/risk_metric.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include "quantforge/errors.hpp"

namespace quantforge {

// Declared in var.cpp's translation unit via the shared header; re-invoke
// through the public dispatch function rather than duplicating the sort.
double compute(VaR, const std::vector<double>& losses, double confidence);

double compute(CVaR, const std::vector<double>& losses, double confidence) {
    if (losses.empty()) {
        throw QuantForgeError("compute(CVaR): empty loss vector.");
    }

    double var_threshold = compute(VaR{}, losses, confidence);

    // CVaR (Expected Shortfall) = average loss among all outcomes at or
    // beyond the VaR threshold — this is what makes it a coherent risk
    // measure, unlike plain VaR.
    double sum = 0.0;
    std::size_t count = 0;
    for (double loss : losses) {
        if (loss >= var_threshold) {
            sum += loss;
            ++count;
        }
    }

    if (count == 0) {
        // Degenerate case (extremely small sample / all losses identical).
        return var_threshold;
    }
    return sum / static_cast<double>(count);
}

} // namespace quantforge
