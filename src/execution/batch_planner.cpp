#include "execution/batch_planner.hpp"

#include <algorithm>

namespace quantforge::execution {

std::vector<std::uint64_t> BatchPlanner::plan(std::uint64_t total_simulations,
                                               std::uint64_t max_batch_size) const {
    std::vector<std::uint64_t> batches;

    if (max_batch_size == 0 || max_batch_size >= total_simulations) {
        batches.push_back(total_simulations);
        return batches;
    }

    std::uint64_t remaining = total_simulations;
    while (remaining > 0) {
        std::uint64_t this_batch = std::min(remaining, max_batch_size);
        batches.push_back(this_batch);
        remaining -= this_batch;
    }
    return batches;
}

} // namespace quantforge::execution
