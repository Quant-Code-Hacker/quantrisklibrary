#include "cuda/random/curand_wrapper.hpp"

namespace quantforge::cuda {

__global__ void init_curand_states(curandState* states, std::uint64_t seed, std::uint64_t n) {
    std::uint64_t idx = blockIdx.x * static_cast<std::uint64_t>(blockDim.x) + threadIdx.x;
    if (idx >= n) return;
    // Distinct sequence per thread (subsequence = idx), offset 0 — this is
    // the standard curand pattern for independent per-thread streams under
    // a single run seed, matching RngManager's batch-seed derivation on
    // the host side.
    curand_init(seed, idx, 0, &states[idx]);
}

} // namespace quantforge::cuda
