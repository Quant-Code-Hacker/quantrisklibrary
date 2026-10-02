#pragma once

#include <cstdint>
#include <curand_kernel.h>

namespace quantforge::cuda {

// One thread per simulated path. Writes terminal loss directly (fused
// path-generation + loss computation, avoiding a separate kernel launch
// and an extra global-memory round trip for the intermediate terminal
// price) — see loss_kernel.hpp for the historical/non-fused variant used
// when losses depend on multi-asset correlation instead of a scalar GBM.
__global__ void mc_path_kernel(curandState* states,
                                double drift,
                                double volatility,
                                double initial_value,
                                double* out_losses,
                                std::uint64_t n);

} // namespace quantforge::cuda
