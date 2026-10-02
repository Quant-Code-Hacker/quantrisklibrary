#include "execution/workload_estimator.hpp"

namespace quantforge::execution {

namespace {
// Threshold above which we consider a workload "memory bound" enough that
// batching should be considered even before checking real GPU memory.
constexpr std::uint64_t kMemoryBoundThresholdBytes = 2ULL * 1024 * 1024 * 1024; // 2 GiB
} // namespace

WorkloadProfile estimate_workload(const Portfolio& portfolio, const RiskConfig& config) {
    WorkloadProfile profile;
    profile.num_simulations = config.simulations;
    profile.portfolio_size  = portfolio.size();

    // One double per path for terminal loss is the dominant cost; a real
    // multi-asset model would multiply by portfolio_size for per-asset paths.
    profile.estimated_bytes = config.simulations * sizeof(double);
    profile.memory_bound    = profile.estimated_bytes > kMemoryBoundThresholdBytes;

    return profile;
}

} // namespace quantforge::execution
