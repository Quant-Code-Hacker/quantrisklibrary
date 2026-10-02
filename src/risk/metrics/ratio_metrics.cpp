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

// Forward declarations
double compute(Volatility, const std::vector<double>& losses, double confidence);
double compute(DownsideDeviation, const std::vector<double>& losses, double confidence);
double compute(MaxDrawdown, const std::vector<double>& losses, double confidence);

double compute(SharpeRatio, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(SharpeRatio): empty loss vector.");
    }
    
    // Assume losses are actually returns (negative loss = positive return)
    // Convert to returns
    std::vector<double> returns;
    returns.reserve(losses.size());
    for (double loss : losses) {
        returns.push_back(-loss);  // Convert loss to return
    }
    
    double mean_return = mean(returns);
    double risk_free_rate = 0.02;  // Default 2% annual risk-free rate
    double excess_return = mean_return - risk_free_rate;
    double volatility = std_dev(returns, mean_return);
    
    if (volatility == 0.0) return 0.0;
    
    return excess_return / volatility;
}

double compute(SortinoRatio, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(SortinoRatio): empty loss vector.");
    }
    
    // Convert to returns
    std::vector<double> returns;
    returns.reserve(losses.size());
    for (double loss : losses) {
        returns.push_back(-loss);
    }
    
    double mean_return = mean(returns);
    double risk_free_rate = 0.02;
    double excess_return = mean_return - risk_free_rate;
    
    // Downside deviation (using minimum acceptable return = risk-free rate)
    std::vector<double> downside_returns;
    downside_returns.reserve(returns.size());
    for (double r : returns) {
        if (r < risk_free_rate) {
            downside_returns.push_back(risk_free_rate - r);
        }
    }
    
    if (downside_returns.empty()) {
        // All returns above risk-free rate
        return excess_return > 0 ? 100.0 : 0.0;
    }
    
    double downside_dev = std_dev(downside_returns, mean(downside_returns));
    
    if (downside_dev == 0.0) return 0.0;
    
    return excess_return / downside_dev;
}

double compute(CalmarRatio, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(CalmarRatio): empty loss vector.");
    }
    
    // Convert to returns
    std::vector<double> returns;
    returns.reserve(losses.size());
    for (double loss : losses) {
        returns.push_back(-loss);
    }
    
    double mean_return = mean(returns);
    double max_dd = compute(MaxDrawdown{}, losses, 0.0);
    
    if (max_dd == 0.0) return 0.0;
    
    // Annualize the return (assuming daily returns)
    double annual_return = mean_return * 252.0;
    
    return annual_return / max_dd;
}

double compute(InformationRatio, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(InformationRatio): empty loss vector.");
    }
    
    // Convert to returns
    std::vector<double> returns;
    returns.reserve(losses.size());
    for (double loss : losses) {
        returns.push_back(-loss);
    }
    
    double mean_return = mean(returns);
    double benchmark_return = 0.05;  // Assume 5% benchmark return
    double excess_return = mean_return - benchmark_return;
    
    // Tracking error = std dev of excess returns
    std::vector<double> excess_returns;
    excess_returns.reserve(returns.size());
    for (double r : returns) {
        excess_returns.push_back(r - benchmark_return);
    }
    
    double tracking_error = std_dev(excess_returns, mean(excess_returns));
    
    if (tracking_error == 0.0) return 0.0;
    
    return excess_return / tracking_error;
}

double compute(OmegaRatio, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(OmegaRatio): empty loss vector.");
    }
    
    // Convert to returns
    std::vector<double> returns;
    returns.reserve(losses.size());
    for (double loss : losses) {
        returns.push_back(-loss);
    }
    
    double threshold = 0.0;  // Zero return threshold
    
    // Calculate gains above threshold and losses below threshold
    double gains = 0.0;
    double losses_below = 0.0;
    
    for (double r : returns) {
        if (r > threshold) {
            gains += r - threshold;
        } else {
            losses_below += threshold - r;
        }
    }
    
    if (losses_below == 0.0) {
        return gains > 0 ? 100.0 : 1.0;
    }
    
    return gains / losses_below;
}

double compute(TreynorRatio, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(TreynorRatio): empty loss vector.");
    }
    
    // Convert to returns
    std::vector<double> returns;
    returns.reserve(losses.size());
    for (double loss : losses) {
        returns.push_back(-loss);
    }
    
    double mean_return = mean(returns);
    double risk_free_rate = 0.02;
    double excess_return = mean_return - risk_free_rate;
    
    // Beta would require market data - using placeholder
    // In a real implementation, this would be calculated against a benchmark
    double beta = 1.0;  // Placeholder
    
    if (beta == 0.0) return 0.0;
    
    return excess_return / beta;
}

} // namespace quantforge
