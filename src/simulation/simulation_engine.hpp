#pragma once

#include <string>
#include "quantforge/portfolio.hpp"
#include "quantforge/risk_config.hpp"
#include "quantforge/backend.hpp"

namespace quantforge::simulation {

// SimulationEngine turns (portfolio, config, model_name) into a
// SimulationTask, hands it to whatever ExecutionBackend the orchestrator
// selected, and returns raw per-path losses. It has zero knowledge of
// what risk metric will be computed downstream — see docs/architecture.md
// for why that separation matters.
class SimulationEngine {
public:
    // `backend` is resolved by the orchestrator before this is called —
    // by the time SimulationEngine runs, backend selection is already done.
    SimulationResult run(const Portfolio& portfolio,
                          const RiskConfig& config,
                          const std::string& model_name,
                          ExecutionBackend& backend);

private:
    SimulationTask build_task(const Portfolio&, const RiskConfig&, std::uint64_t batch_seed,
                               std::uint64_t paths_this_batch, std::uint64_t offset) const;
};

} // namespace quantforge::simulation
