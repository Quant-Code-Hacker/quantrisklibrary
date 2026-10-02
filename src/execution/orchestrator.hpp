#pragma once

#include <vector>
#include "quantforge/portfolio.hpp"
#include "quantforge/risk_config.hpp"
#include "quantforge/backend.hpp"

namespace quantforge::execution {

struct OrchestrationOutcome {
    std::vector<double> losses;
    Backend              backend_used;
    std::uint64_t         simulations_run;
    double                total_ms;
    double                compute_ms;
};

// The single place that turns "run this many simulations for this
// portfolio" into: workload analysis -> backend resolution (with
// fallback) -> batch planning -> execution loop -> optional adaptive
// early stop -> optional CPU cross-validation. Everything above this
// (RiskEngine) only ever calls Orchestrator::run().
class Orchestrator {
public:
    OrchestrationOutcome run(const Portfolio& portfolio, const RiskConfig& config);
};

} // namespace quantforge::execution
