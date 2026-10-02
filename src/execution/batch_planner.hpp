#pragma once

#include <cstdint>
#include <vector>
#include "quantforge/backend.hpp"

namespace quantforge::execution {

// Splits a total simulation count into batches. For CPU/OpenMP this is
// mostly a no-op (one batch), but it's the seam that makes adaptive
// simulation (stop early once converged) and future GPU memory-aware
// batching possible without changing the orchestrator's control flow.
class BatchPlanner {
public:
    // Returns a sequence of batch sizes summing to `total_simulations`.
    // `max_batch_size` caps any individual batch (e.g. to fit GPU memory);
    // pass 0 for "no cap" (single batch).
    std::vector<std::uint64_t> plan(std::uint64_t total_simulations,
                                     std::uint64_t max_batch_size) const;
};

} // namespace quantforge::execution
