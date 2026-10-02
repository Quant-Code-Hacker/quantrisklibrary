#include "cuda/kernels/loss_kernel.hpp"

namespace quantforge::cuda {

__global__ void loss_kernel(const double* terminal_prices,
                             const double* asset_quantities,
                             const double* initial_prices,
                             double* out_losses,
                             std::uint64_t num_paths,
                             std::uint64_t num_assets) {
    std::uint64_t path = blockIdx.x * static_cast<std::uint64_t>(blockDim.x) + threadIdx.x;
    if (path >= num_paths) return;

    double initial_value = 0.0;
    double terminal_value = 0.0;
    for (std::uint64_t a = 0; a < num_assets; ++a) {
        initial_value  += asset_quantities[a] * initial_prices[a];
        terminal_value += asset_quantities[a] * terminal_prices[path * num_assets + a];
    }
    out_losses[path] = initial_value - terminal_value;
}

} // namespace quantforge::cuda
