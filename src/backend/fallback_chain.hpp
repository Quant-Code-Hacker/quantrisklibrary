#pragma once

#include "quantforge/backend.hpp"

namespace quantforge::backend {

// Resolves a *desired* backend into an actually-usable ExecutionBackend*,
// walking CUDA -> OpenMP -> CPU until it finds one that's both registered
// (compiled in) and reports itself available (Sec. 18: "CUDA unavailable
// -> OpenMP -> CPU"). Throws BackendUnavailableError only if even CPU
// isn't registered, which should never happen in a normal build.
class FallbackChain {
public:
    ExecutionBackend& resolve(Backend desired) const;
};

} // namespace quantforge::backend
