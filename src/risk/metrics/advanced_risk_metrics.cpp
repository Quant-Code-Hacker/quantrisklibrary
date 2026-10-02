#include "quantforge/risk_metric.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include "quantforge/errors.hpp"

namespace quantforge {

// Helper: compute mean
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
double compute(VaR, const std::vector<double>& losses, double confidence);

// Marginal VaR: The change in portfolio VaR for a small change in position size
// For a single-asset portfolio, this approximates to VaR itself
double compute(MarginalVaR, const std::vector<double>& losses, double confidence) {
    if (losses.empty()) {
        throw QuantForgeError("compute(MarginalVaR): empty loss vector.");
    }
    // For single asset, marginal VaR ≈ VaR / position value
    // This is a simplified implementation - full implementation requires multi-asset data
    double var = compute(VaR{}, losses, confidence);
    return var;  // Return VaR as marginal contribution for single asset
}

// Component VaR: The contribution of an asset to total portfolio VaR
double compute(ComponentVaR, const std::vector<double>& losses, double confidence) {
    if (losses.empty()) {
        throw QuantForgeError("compute(ComponentVaR): empty loss vector.");
    }
    // For single asset, component VaR = VaR
    return compute(VaR{}, losses, confidence);
}

// Incremental VaR: The change in VaR if the position were removed
double compute(IncrementalVaR, const std::vector<double>& losses, double confidence) {
    if (losses.empty()) {
        throw QuantForgeError("compute(IncrementalVaR): empty loss vector.");
    }
    // For single asset, incremental VaR = VaR (removing it would eliminate all risk)
    return compute(VaR{}, losses, confidence);
}

// Beta: Systematic risk relative to market
// Simplified: assumes market returns have same volatility as portfolio
double compute(Beta, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.size() < 2) {
        throw QuantForgeError("compute(Beta): need at least 2 samples.");
    }
    
    // Convert losses to returns
    std::vector<double> returns;
    returns.reserve(losses.size());
    for (double loss : losses) {
        returns.push_back(-loss);
    }
    
    double portfolio_mean = mean(returns);
    double portfolio_std = std_dev(returns, portfolio_mean);
    
    if (portfolio_std == 0.0) return 1.0;  // No volatility, beta = 1 (neutral)
    
    // Simplified: assume market has same volatility as portfolio
    // Full implementation requires market data
    double market_std = portfolio_std;
    
    // Assume correlation of 0.7 with market (typical for diversified portfolio)
    double correlation = 0.7;
    
    return correlation * (portfolio_std / market_std);
}

// Tracking Error: Standard deviation of excess returns relative to benchmark
double compute(TrackingError, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.size() < 2) {
        throw QuantForgeError("compute(TrackingError): need at least 2 samples.");
    }
    
    // Convert losses to returns
    std::vector<double> returns;
    returns.reserve(losses.size());
    for (double loss : losses) {
        returns.push_back(-loss);
    }
    
    // Assume benchmark return of 5% annual
    double benchmark_return = 0.05 / 252.0;  // Daily
    
    std::vector<double> excess_returns;
    excess_returns.reserve(returns.size());
    for (double r : returns) {
        excess_returns.push_back(r - benchmark_return);
    }
    
    return std_dev(excess_returns, mean(excess_returns));
}

// Sterling Ratio: Annual return / average drawdown
double compute(SterlingRatio, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(SterlingRatio): empty loss vector.");
    }
    
    // Convert to returns
    std::vector<double> returns;
    returns.reserve(losses.size());
    for (double loss : losses) {
        returns.push_back(-loss);
    }
    
    double annual_return = mean(returns) * 252.0;
    
    // Use max drawdown as proxy for average drawdown (simplified)
    double max_dd = 0.0;
    double cumulative = 0.0;
    double peak = 0.0;
    
    for (double r : returns) {
        cumulative += r;
        if (cumulative > peak) peak = cumulative;
        double dd = peak - cumulative;
        if (dd > max_dd) max_dd = dd;
    }
    
    if (max_dd == 0.0) return annual_return > 0 ? 100.0 : 0.0;
    
    return annual_return / max_dd;
}

// Burke Ratio: Annual return / square root of sum of squared drawdowns
double compute(BurkeRatio, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(BurkeRatio): empty loss vector.");
    }
    
    // Convert to returns
    std::vector<double> returns;
    returns.reserve(losses.size());
    for (double loss : losses) {
        returns.push_back(-loss);
    }
    
    double annual_return = mean(returns) * 252.0;
    
    // Calculate drawdowns
    double cumulative = 0.0;
    double peak = 0.0;
    std::vector<double> drawdowns;
    
    for (double r : returns) {
        cumulative += r;
        if (cumulative > peak) peak = cumulative;
        double dd = peak - cumulative;
        if (dd > 0) drawdowns.push_back(dd);
    }
    
    if (drawdowns.empty()) return annual_return > 0 ? 100.0 : 0.0;
    
    double sum_squared = std::accumulate(drawdowns.begin(), drawdowns.end(), 0.0,
        [](double acc, double val) { return acc + val * val; });
    
    double root_sum_sq = std::sqrt(sum_squared);
    
    if (root_sum_sq == 0.0) return annual_return > 0 ? 100.0 : 0.0;
    
    return annual_return / root_sum_sq;
}

// Diversification Ratio: Weighted avg volatility / portfolio volatility
// For single asset, this equals 1.0
double compute(DiversificationRatio, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(DiversificationRatio): empty loss vector.");
    }
    // Single asset has no diversification benefit
    return 1.0;
}

// HHI (Herfindahl-Hirschman Index): Concentration measure
// For single asset, HHI = 1.0 (maximum concentration)
double compute(HHI, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(HHI): empty loss vector.");
    }
    // Single asset = maximum concentration
    return 1.0;
}

} // namespace quantforge
