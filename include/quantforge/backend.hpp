#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace quantforge {

enum class Backend {
    Auto,     // orchestrator decides
    CPU,
    OpenMP,
    CUDA,
};

std::string to_string(Backend b);

// What a backend advertises about itself so the selector can reason about it
// without hardcoding backend-specific knowledge (Sec. 4).
struct BackendCapabilities {
    Backend     kind;
    bool        available          = false; // e.g. false if no GPU present
    std::size_t max_memory_bytes   = 0;
    int         parallelism_hint   = 1;      // core count / SM count, advisory only
};

// The unit of work handed to a backend. Deliberately has no notion of
// "Portfolio" or "VaR" — a backend only ever sees numeric simulation
// parameters and a place to write results.
struct SimulationTask {
    std::uint64_t num_paths      = 0;
    std::uint64_t seed           = 0;
    double        drift          = 0.0;
    double        volatility     = 0.0;
    double        initial_value  = 0.0;
    std::uint64_t batch_offset   = 0;  // for adaptive batching / resumability
};

struct SimulationResult {
    std::vector<double> terminal_losses; // one value per simulated path
    double               elapsed_ms = 0.0;
    Backend               backend_used = Backend::CPU;
};

// The interface every backend (CPU, OpenMP, CUDA, future Multi-GPU/MPI)
// implements. RiskEngine and SimulationEngine only ever talk to this.
class ExecutionBackend {
public:
    virtual ~ExecutionBackend() = default;

    virtual SimulationResult execute(const SimulationTask& task) = 0;
    virtual BackendCapabilities capabilities() const = 0;
    virtual Backend kind() const = 0;
};

} // namespace quantforge
