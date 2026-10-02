#include "execution/orchestrator.hpp"

#include <chrono>
#include <cmath>
#include "execution/workload_estimator.hpp"
#include "execution/backend_selector.hpp"
#include "execution/batch_planner.hpp"
#include "backend/fallback_chain.hpp"
#include "backend/backend_registry.hpp"
#include "simulation/simulation_engine.hpp"
#include "risk/convergence.hpp"
#include "cpu/reference_kernels.hpp"
#include "portfolio/validation.hpp"
#include "quantforge/errors.hpp"

namespace quantforge::execution {

OrchestrationOutcome Orchestrator::run(const Portfolio& portfolio, const RiskConfig& config) {
    auto wall_start = std::chrono::steady_clock::now();

    // 0. Validation gate — first thing that happens, before any compute
    // resource is touched (see docs/architecture.md step 1).
    portfolio::validate(portfolio, config);

    // 1. Workload analysis
    WorkloadProfile profile = estimate_workload(portfolio, config);

    // 2. Backend selection (heuristic or user-forced) + fallback resolution
    BackendSelector selector;
    Backend desired = selector.select(config.backend, profile);

    backend::FallbackChain fallback;
    ExecutionBackend& chosen_backend = fallback.resolve(desired);

    // 3. Batch planning. No hard cap here yet since CPU/OpenMP have no
    // memory ceiling worth enforcing; CUDA's batch planner call would pass
    // a real max_batch_size derived from BackendCapabilities.max_memory_bytes.
    BatchPlanner planner;
    std::uint64_t cap = (chosen_backend.kind() == Backend::CUDA) ? 2'000'000ULL : 0ULL;
    auto batches = planner.plan(config.simulations, cap);

    // 4. Execution loop, with optional adaptive early stop between batches.
    simulation::SimulationEngine sim_engine;
    std::vector<double> accumulated_losses;
    accumulated_losses.reserve(config.simulations);

    double compute_ms = 0.0;
    std::uint64_t simulations_run = 0;

    RiskConfig batch_config = config; // copy; num simulations overridden per-batch

    for (std::uint64_t batch_size : batches) {
        batch_config.simulations = batch_size;
        // NOTE: seed handling for multi-batch runs is delegated to
        // SimulationEngine/RngManager, which derives a distinct sub-seed
        // per call using the *original* config.seed — batch index is
        // implicit in call order today; a future revision should pass an
        // explicit batch index through so batches remain reproducible
        // independent of adaptive early-stop cutting the sequence short.
        SimulationResult result = sim_engine.run(portfolio, batch_config, "MonteCarlo", chosen_backend);

        accumulated_losses.insert(accumulated_losses.end(),
                                   result.terminal_losses.begin(),
                                   result.terminal_losses.end());
        compute_ms += result.elapsed_ms;
        simulations_run += batch_size;

        if (config.adaptive) {
            auto convergence = risk::check_convergence(
                accumulated_losses, config.confidence, config.target_rel_std_err);
            if (convergence.converged) {
                break;
            }
        }
    }

    // 5. Optional numerical cross-validation against the CPU reference.
    if (config.verify_against_cpu && chosen_backend.kind() != Backend::CPU) {
        SimulationTask verify_task;
        verify_task.num_paths     = std::min<std::uint64_t>(simulations_run, 100'000ULL);
        verify_task.seed          = config.seed;
        verify_task.initial_value = portfolio.total_value();
        // NOTE: drift/volatility are recomputed identically inside
        // SimulationEngine::build_task; for a real implementation this
        // task should be captured from the actual run rather than rebuilt.
        SimulationResult reference = cpu::run_monte_carlo_reference(verify_task);

        double mean_chosen = 0.0, mean_reference = 0.0;
        for (std::uint64_t i = 0; i < verify_task.num_paths && i < accumulated_losses.size(); ++i) {
            mean_chosen += accumulated_losses[i];
        }
        for (double v : reference.terminal_losses) mean_reference += v;
        mean_chosen /= static_cast<double>(verify_task.num_paths);
        mean_reference /= static_cast<double>(reference.terminal_losses.size());

        double rel_diff = std::abs(mean_chosen - mean_reference) /
                           (std::abs(mean_reference) > 1e-12 ? std::abs(mean_reference) : 1.0);
        if (rel_diff > config.verify_tolerance) {
            throw NumericalValidationError(
                "Backend result diverged from CPU reference beyond tolerance "
                "(relative difference exceeded verify_tolerance).");
        }
    }

    auto wall_end = std::chrono::steady_clock::now();

    OrchestrationOutcome outcome;
    outcome.losses          = std::move(accumulated_losses);
    outcome.backend_used    = chosen_backend.kind();
    outcome.simulations_run = simulations_run;
    outcome.total_ms        = std::chrono::duration<double, std::milli>(wall_end - wall_start).count();
    outcome.compute_ms      = compute_ms;
    return outcome;
}

} // namespace quantforge::execution
