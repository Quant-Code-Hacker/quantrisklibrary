#include "openmp/openmp_backend.hpp"

#include <omp.h>
#include <chrono>
#include <cmath>
#include <random>
#include "backend/backend_registry.hpp"
#include "core/rng_manager.hpp"

namespace quantforge::openmp_backend {

SimulationResult OpenMPBackend::execute(const SimulationTask& task) {
    auto start = std::chrono::steady_clock::now();

    SimulationResult result;
    result.terminal_losses.resize(task.num_paths);
    result.backend_used = Backend::OpenMP;

    const double drift_term = task.drift - 0.5 * task.volatility * task.volatility;

    #pragma omp parallel
    {
        // Each thread gets its own deterministic sub-seed derived from the
        // task seed + thread id, so results are reproducible for a fixed
        // thread count (documented caveat: changing thread count changes
        // the RNG partition, not the statistical distribution).
        int tid = omp_get_thread_num();
        quantforge::core::RngManager thread_rng(task.seed);
        std::mt19937_64 gen(thread_rng.seed_for_batch(static_cast<std::uint64_t>(tid)));
        std::normal_distribution<double> normal(0.0, 1.0);

        #pragma omp for schedule(static)
        for (std::int64_t i = 0; i < static_cast<std::int64_t>(task.num_paths); ++i) {
            double z = normal(gen);
            double terminal_value = task.initial_value * std::exp(drift_term + task.volatility * z);
            result.terminal_losses[static_cast<std::size_t>(i)] = task.initial_value - terminal_value;
        }
    }

    auto end = std::chrono::steady_clock::now();
    result.elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
    return result;
}

BackendCapabilities OpenMPBackend::capabilities() const {
    BackendCapabilities caps;
    caps.kind = Backend::OpenMP;
    caps.available = true;
    caps.parallelism_hint = omp_get_max_threads();
    caps.max_memory_bytes = 0;
    return caps;
}

} // namespace quantforge::openmp_backend

QUANTFORGE_REGISTER_BACKEND(quantforge::openmp_backend::OpenMPBackend)
