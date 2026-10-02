#include "execution/backend_selector.hpp"

namespace quantforge::execution {

Backend BackendSelector::select(Backend requested, const WorkloadProfile& profile) const {
    if (requested != Backend::Auto) {
        return requested; // explicit user choice always wins; availability is
                           // checked downstream by backend::FallbackChain.
    }

    // Static heuristic per the design doc (Sec. 4):
    //   ~10k sims  -> CPU is fastest (parallel overhead not worth it)
    //   ~100k sims -> OpenMP wins (embarrassingly parallel, no transfer cost)
    //   ~10M+ sims -> CUDA wins (transfer cost amortized over massive parallelism)
    //
    // This is intentionally a simple, replaceable heuristic. The "learned
    // backend selection" novelty replaces this function body with a lookup
    // into a small model trained on (profile -> fastest backend) history
    // without changing this class's interface.
    if (profile.num_simulations < 50'000) {
        return Backend::CPU;
    }
    if (profile.num_simulations < 1'000'000) {
        return Backend::OpenMP;
    }
    return Backend::CUDA;
}

} // namespace quantforge::execution
