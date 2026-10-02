#pragma once

#include <cstdint>

namespace quantforge::cuda {

// Computes portfolio loss from per-asset terminal prices when the
// simulation involves more than one correlated asset (i.e. once
// SimulationTask grows beyond the current single-aggregate-position
// model). Currently unused: mc_path_kernel.cu fuses generation + loss
// for the scalar-GBM case. This kernel is the extension point for
// per-asset correlated paths — weights * (price_0 - price_T) summed per
// scenario.
__global__ void loss_kernel(const double* terminal_prices,   // [num_paths x num_assets]
                             const double* asset_quantities,  // [num_assets]
                             const double* initial_prices,    // [num_assets]
                             double* out_losses,               // [num_paths]
                             std::uint64_t num_paths,
                             std::uint64_t num_assets);

} // namespace quantforge::cuda
