#pragma once

#include "quantforge/backend.hpp"

namespace quantforge::cpu {

// Single-threaded, straightforward GBM terminal-loss simulation. This is
// the trusted implementation everything else (OpenMP, CUDA) is numerically
// validated against — see tests/numerical/ and Sec. 15 of the design doc.
// Deliberately simple: no micro-optimization here, only correctness.
SimulationResult run_monte_carlo_reference(const SimulationTask& task);

} // namespace quantforge::cpu
