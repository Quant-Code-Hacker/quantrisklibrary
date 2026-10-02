#pragma once

#include <string>
#include <vector>

namespace quantforge {

// Tag types. Each has a matching compute() free function found via ADL in
// src/risk/metrics/*.cpp. Adding a new metric means adding a new tag +
// implementation file — nothing else in the engine changes.
struct VaR        { static constexpr const char* name = "VaR"; };
struct CVaR       { static constexpr const char* name = "CVaR"; };
struct Volatility { static constexpr const char* name = "Volatility"; };
struct Drawdown   { static constexpr const char* name = "Drawdown"; };

// Every metric implementation has this shape:
//   double compute(const std::vector<double>& losses, double confidence);
double compute(VaR, const std::vector<double>& losses, double confidence);
double compute(CVaR, const std::vector<double>& losses, double confidence);

} // namespace quantforge
