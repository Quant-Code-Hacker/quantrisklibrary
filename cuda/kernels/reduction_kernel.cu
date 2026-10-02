#include "cuda/kernels/reduction_kernel.hpp"

namespace quantforge::cuda {

namespace {
constexpr int kWarpSize = 32;

__device__ double warp_reduce_sum(double val) {
    for (int offset = kWarpSize / 2; offset > 0; offset /= 2) {
        val += __shfl_down_sync(0xffffffff, val, offset);
    }
    return val;
}
} // namespace

__global__ void reduce_sum_kernel(const double* in, double* out, std::uint64_t n) {
    extern __shared__ double warp_sums[]; // one slot per warp in the block

    std::uint64_t idx = blockIdx.x * static_cast<std::uint64_t>(blockDim.x) + threadIdx.x;
    double val = (idx < n) ? in[idx] : 0.0;

    // 1. Warp-level reduction via shuffle — no shared memory needed here.
    val = warp_reduce_sum(val);

    int lane = threadIdx.x % kWarpSize;
    int warp_id = threadIdx.x / kWarpSize;
    if (lane == 0) warp_sums[warp_id] = val;
    __syncthreads();

    // 2. Block-level reduction: first warp reduces the per-warp partials.
    int num_warps = (blockDim.x + kWarpSize - 1) / kWarpSize;
    if (warp_id == 0) {
        double block_val = (lane < num_warps) ? warp_sums[lane] : 0.0;
        block_val = warp_reduce_sum(block_val);
        if (lane == 0) {
            // 3. Global reduction is finished on the host (or a second
            // kernel launch) by summing this small per-block array —
            // avoids a full second full-size pass for a handful of blocks.
            out[blockIdx.x] = block_val;
        }
    }
}

} // namespace quantforge::cuda
