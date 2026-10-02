#pragma once

#include <cstdint>

namespace quantforge::cuda {

// Sums `in[0..n)` into `out[blockIdx.x]` using warp-shuffle reduction
// within each warp, shared-memory reduction across warps in a block, and
// leaves the final global sum to be finished by a second launch (or a
// host-side sum of the small `out` array) — the standard two-pass
// hierarchical reduction pattern. Used for GPU-side running mean/variance
// (the convergence check's tail statistics) so the adaptive loop doesn't
// have to transfer every path back to host between batches.
__global__ void reduce_sum_kernel(const double* in, double* out, std::uint64_t n);

} // namespace quantforge::cuda
