#pragma once

#include "quantforge/backend.hpp"
#include "execution/workload_estimator.hpp"

namespace quantforge::execution {

// Turns (requested backend, workload profile, what's actually registered
// and available) into a concrete Backend to run on. This is the seam
// where the "learned backend selection" novelty (blueprint Sec. 3 #1)
// would plug in: swap select() for a call into a trained model instead
// of the heuristic below, without touching anything upstream.
class BackendSelector {
public:
    Backend select(Backend requested, const WorkloadProfile& profile) const;
};

} // namespace quantforge::execution
