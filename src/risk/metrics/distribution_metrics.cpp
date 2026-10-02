#include "quantforge/risk_metric.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include "quantforge/errors.hpp"

namespace quantforge {

// Helper: compute mean of a vector
static double mean(const std::vector<double>& values) {
    if (values.empty()) return 0.0;
    return std::accumulate(values.begin(), values.end(), 0.0) / static_cast<double>(values.size());
}

// Helper: compute standard deviation
static double std_dev(const std::vector<double>& values, double mean_val) {
    if (values.size() < 2) return 0.0;
    double sum_sq = 0.0;
    for (double v : values) {
        double diff = v - mean_val;
        sum_sq += diff * diff;
    }
    return std::sqrt(sum_sq / static_cast<double>(values.size() - 1));
}

double compute(Volatility, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(Volatility): empty loss vector.");
    }
    double m = mean(losses);
    return std_dev(losses, m);
}

double compute(DownsideDeviation, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(DownsideDeviation): empty loss vector.");
    }
    
    double m = mean(losses);
    std::vector<double> downside_losses;
    downside_losses.reserve(losses.size());
    
    for (double loss : losses) {
        if (loss > m) {  // Losses worse than mean
            downside_losses.push_back(loss - m);
        }
    }
    
    if (downside_losses.empty()) return 0.0;
    
    double sum_sq = std::accumulate(downside_losses.begin(), downside_losses.end(), 0.0,
        [](double acc, double val) { return acc + val * val; });
    
    return std::sqrt(sum_sq / static_cast<double>(downside_losses.size()));
}

double compute(SemiDeviation, const std::vector<double>& losses, double /*confidence*/) {
    // Semi-deviation is the same as downside deviation
    return compute(DownsideDeviation{}, losses, 0.0);
}

double compute(Skewness, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.size() < 3) {
        throw QuantForgeError("compute(Skewness): need at least 3 samples.");
    }
    
    double m = mean(losses);
    double s = std_dev(losses, m);
    
    if (s == 0.0) return 0.0;
    
    double sum_cubed = 0.0;
    for (double loss : losses) {
        double diff = (loss - m) / s;
        sum_cubed += diff * diff * diff;
    }
    
    return sum_cubed / static_cast<double>(losses.size());
}

double compute(Kurtosis, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.size() < 4) {
        throw QuantForgeError("compute(Kurtosis): need at least 4 samples.");
    }
    
    double m = mean(losses);
    double s = std_dev(losses, m);
    
    if (s == 0.0) return 0.0;
    
    double sum_quart = 0.0;
    for (double loss : losses) {
        double diff = (loss - m) / s;
        sum_quart += diff * diff * diff * diff;
    }
    
    // Excess kurtosis (subtract 3 for normal distribution)
    return (sum_quart / static_cast<double>(losses.size())) - 3.0;
}

} // namespace quantforge
