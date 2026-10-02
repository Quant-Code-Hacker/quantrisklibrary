#include "simulation/simulation_engine.hpp"

#include "core/rng_manager.hpp"

namespace quantforge::simulation {

SimulationTask SimulationEngine::build_task(const Portfolio& portfolio,
                                             const RiskConfig& config,
                                             std::uint64_t batch_seed,
                                             std::uint64_t paths_this_batch,
                                             std::uint64_t offset) const {
    // MonteCarlo model: treat the portfolio as a single aggregate position
    // with drift 0 and volatility derived from asset weights. A real
    // implementation would build a covariance matrix across assets and
    // simulate correlated paths per-asset — this is intentionally the
    // simplest model that produces a defensible loss distribution, so the
    // execution/risk layers above it have something real to exercise.
    double total_value = portfolio.total_value();
    double agg_vol = 0.0;
    if (!portfolio.assets().empty()) {
        for (const auto& a : portfolio.assets()) {
            agg_vol += (a.volatility > 0.0 ? a.volatility : 0.20); // default 20% vol
        }
        agg_vol /= static_cast<double>(portfolio.assets().size());
    }

    SimulationTask task;
    task.num_paths     = paths_this_batch;
    task.seed          = batch_seed;
    task.drift         = 0.0;
    task.volatility    = agg_vol;
    task.initial_value = total_value;
    task.batch_offset  = offset;
    return task;
}

SimulationResult SimulationEngine::run(const Portfolio& portfolio,
                                        const RiskConfig& config,
                                        const std::string& /*model_name*/,
                                        ExecutionBackend& backend) {
    // NOTE: this runs the full `config.simulations` count as a single task.
    // Adaptive batching (Sec. 5 / Sec. 13) is layered on top by
    // execution/orchestrator.cpp, which calls this repeatedly with smaller
    // batches and consults risk/convergence.hpp between calls when
    // config.adaptive == true.
    core::RngManager rng(config.seed);
    auto task = build_task(portfolio, config, rng.seed_for_batch(0), config.simulations, 0);
    return backend.execute(task);
}

} // namespace quantforge::simulation
