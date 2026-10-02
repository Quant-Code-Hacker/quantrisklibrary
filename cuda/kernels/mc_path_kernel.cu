#include "cuda/kernels/mc_path_kernel.hpp"

namespace quantforge::cuda {

__global__ void mc_path_kernel(curandState* states,
                                double drift,
                                double volatility,
                                double initial_value,
                                double* out_losses,
                                std::uint64_t n) {
    std::uint64_t idx = blockIdx.x * static_cast<std::uint64_t>(blockDim.x) + threadIdx.x;
    if (idx >= n) return;

    // IMPORTANT: this must stay bit-for-bit *formula*-identical to
    // cpu/reference_kernels.cpp's GBM step (same drift_term derivation,
    // same exp() application) — numerical validation (tests/numerical/)
    // compares statistical moments of the two distributions, not
    // individual paths, but any formula drift here would show up as a
    // systematic bias rather than sampling noise.
    curandState local_state = states[idx];
    double z = curand_normal_double(&local_state);
    states[idx] = local_state;

    double drift_term = drift - 0.5 * volatility * volatility;
    double terminal_value = initial_value * exp(drift_term + volatility * z);
    out_losses[idx] = initial_value - terminal_value;
}

} // namespace quantforge::cuda
