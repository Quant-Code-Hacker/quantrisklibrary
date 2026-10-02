#pragma once

#include <cstdint>
#include <curand_kernel.h>

namespace quantforge::cuda {

// Initializes one curandState per thread. Called once per kernel launch
// grid; states are cheap enough to re-init per batch rather than persist,
// which keeps batches independent and reproducible.
__global__ void init_curand_states(curandState* states, std::uint64_t seed, std::uint64_t n);

} // namespace quantforge::cuda
