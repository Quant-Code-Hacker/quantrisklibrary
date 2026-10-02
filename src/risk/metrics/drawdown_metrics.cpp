#include "quantforge/risk_metric.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>
#include "quantforge/errors.hpp"

namespace quantforge {

double compute(MaxDrawdown, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(MaxDrawdown): empty loss vector.");
    }
    
    // Convert losses to cumulative P&L series (assuming losses are from initial value)
    // Start at 0 (no loss), then accumulate losses
    std::vector<double> cumulative;
    cumulative.reserve(losses.size() + 1);
    cumulative.push_back(0.0);  // Starting point
    
    double cumulative_loss = 0.0;
    for (double loss : losses) {
        cumulative_loss += loss;
        cumulative.push_back(cumulative_loss);
    }
    
    // Calculate drawdowns
    double peak = cumulative[0];
    double max_dd = 0.0;
    
    for (double value : cumulative) {
        if (value > peak) {
            peak = value;
        }
        double drawdown = peak - value;
        if (drawdown > max_dd) {
            max_dd = drawdown;
        }
    }
    
    return max_dd;
}

double compute(AverageDrawdown, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(AverageDrawdown): empty loss vector.");
    }
    
    // Convert to cumulative series
    std::vector<double> cumulative;
    cumulative.reserve(losses.size() + 1);
    cumulative.push_back(0.0);
    
    double cumulative_loss = 0.0;
    for (double loss : losses) {
        cumulative_loss += loss;
        cumulative.push_back(cumulative_loss);
    }
    
    // Calculate all drawdowns
    double peak = cumulative[0];
    std::vector<double> drawdowns;
    
    for (double value : cumulative) {
        if (value > peak) {
            peak = value;
        }
        double drawdown = peak - value;
        if (drawdown > 0) {
            drawdowns.push_back(drawdown);
        }
    }
    
    if (drawdowns.empty()) return 0.0;
    
    return std::accumulate(drawdowns.begin(), drawdowns.end(), 0.0) / static_cast<double>(drawdowns.size());
}

double compute(DrawdownDuration, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(DrawdownDuration): empty loss vector.");
    }
    
    // Convert to cumulative series
    std::vector<double> cumulative;
    cumulative.reserve(losses.size() + 1);
    cumulative.push_back(0.0);
    
    double cumulative_loss = 0.0;
    for (double loss : losses) {
        cumulative_loss += loss;
        cumulative.push_back(cumulative_loss);
    }
    
    // Find longest drawdown duration
    double peak = cumulative[0];
    double max_duration = 0.0;
    double current_duration = 0.0;
    
    for (double value : cumulative) {
        if (value > peak) {
            // New peak, reset duration
            peak = value;
            current_duration = 0.0;
        } else {
            // In drawdown
            current_duration += 1.0;
            if (current_duration > max_duration) {
                max_duration = current_duration;
            }
        }
    }
    
    return max_duration;
}

double compute(UlcerIndex, const std::vector<double>& losses, double /*confidence*/) {
    if (losses.empty()) {
        throw QuantForgeError("compute(UlcerIndex): empty loss vector.");
    }
    
    // Convert to cumulative series
    std::vector<double> cumulative;
    cumulative.reserve(losses.size() + 1);
    cumulative.push_back(0.0);
    
    double cumulative_loss = 0.0;
    for (double loss : losses) {
        cumulative_loss += loss;
        cumulative.push_back(cumulative_loss);
    }
    
    // Calculate Ulcer Index: RMS of drawdown percentages
    double peak = cumulative[0];
    std::vector<double> drawdown_pct;
    
    for (double value : cumulative) {
        if (value > peak) {
            peak = value;
        }
        if (peak > 0) {
            double dd_pct = (peak - value) / peak * 100.0;
            drawdown_pct.push_back(dd_pct);
        }
    }
    
    if (drawdown_pct.empty()) return 0.0;
    
    double sum_sq = std::accumulate(drawdown_pct.begin(), drawdown_pct.end(), 0.0,
        [](double acc, double val) { return acc + val * val; });
    
    return std::sqrt(sum_sq / static_cast<double>(drawdown_pct.size()));
}

} // namespace quantforge
