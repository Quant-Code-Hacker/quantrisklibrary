#pragma once

#include <cstdint>
#include <vector>

namespace quantforge::risk {

struct ConvergenceResult {
    bool   converged;
    double relative_std_error; // std error of the tail-mean estimate, relative to its value
};

// Estimates whether the current accumulated losses give a stable enough
// tail estimate to stop simulating, using the standard error of the mean
// of the losses beyond the `confidence` quantile (the CVaR tail), relative
// to the tail mean itself. This is what backs RiskConfig::adaptive
// (Sec. 13) — a real statistical criterion rather than "looks stable".
ConvergenceResult check_convergence(const std::vector<double>& losses_so_far,
                                     double confidence,
                                     double target_relative_std_error);

} // namespace quantforge::risk
