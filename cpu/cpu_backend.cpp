#include "cpu/cpu_backend.hpp"

#include <thread>
#include "cpu/reference_kernels.hpp"
#include "backend/backend_registry.hpp"

namespace quantforge::cpu {

SimulationResult CPUBackend::execute(const SimulationTask& task) {
    // The CPU backend *is* the reference implementation — there is no
    // separate "optimized CPU" path, deliberately, so the trusted ground
    // truth is always exactly what production CPU runs actually execute.
    return run_monte_carlo_reference(task);
}

BackendCapabilities CPUBackend::capabilities() const {
    BackendCapabilities caps;
    caps.kind = Backend::CPU;
    caps.available = true;
    caps.parallelism_hint = static_cast<int>(std::thread::hardware_concurrency());
    caps.max_memory_bytes = 0; // unconstrained / not tracked for CPU
    return caps;
}

} // namespace quantforge::cpu

QUANTFORGE_REGISTER_BACKEND(quantforge::cpu::CPUBackend)
