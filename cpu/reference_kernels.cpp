#include "cpu/reference_kernels.hpp"

#include <chrono>
#include <cmath>
#include <random>

namespace quantforge::cpu {

SimulationResult run_monte_carlo_reference(const SimulationTask& task) {
    auto start = std::chrono::steady_clock::now();

    SimulationResult result;
    result.terminal_losses.resize(task.num_paths);
    result.backend_used = Backend::CPU;

    std::mt19937_64 rng(task.seed);
    std::normal_distribution<double> normal(0.0, 1.0);

    const double drift_term = task.drift - 0.5 * task.volatility * task.volatility;

    for (std::uint64_t i = 0; i < task.num_paths; ++i) {
        double z = normal(rng);
        double terminal_value = task.initial_value * std::exp(drift_term + task.volatility * z);
        // Loss is positive when the portfolio has lost value.
        result.terminal_losses[i] = task.initial_value - terminal_value;
    }

    auto end = std::chrono::steady_clock::now();
    result.elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
    return result;
}

} // namespace quantforge::cpu
