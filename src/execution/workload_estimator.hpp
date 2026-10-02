#pragma once

#include <cstdint>
#include "quantforge/portfolio.hpp"
#include "quantforge/risk_config.hpp"

namespace quantforge::execution {

// Cheap, synchronous analysis of what a run is about to cost — no actual
// execution happens here. Feeds BackendSelector and BatchPlanner.
struct WorkloadProfile {
    std::uint64_t num_simulations;
    std::size_t   portfolio_size;
    std::uint64_t estimated_bytes;      // rough working-set size
    bool          memory_bound;         // true if estimated_bytes is large relative to typical GPU memory
};

WorkloadProfile estimate_workload(const Portfolio& portfolio, const RiskConfig& config);

} // namespace quantforge::execution
