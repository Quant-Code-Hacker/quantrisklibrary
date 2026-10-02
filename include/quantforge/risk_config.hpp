#pragma once

#include <cstdint>
#include "quantforge/backend.hpp"

namespace quantforge {

struct RiskConfig {
    double        confidence         = 0.99;
    std::uint64_t simulations        = 1'000'000;
    std::uint64_t seed               = 42;
    Backend       backend            = Backend::Auto;

    // Adaptive simulation (Sec. 13) — if enabled, `simulations` becomes an
    // upper bound and convergence.hpp decides when to stop early.
    bool          adaptive           = false;
    double        target_rel_std_err = 0.01;  // relative std error of tail estimate

    // Numerical validation (Sec. 15) — expensive, so off by default; CI
    // turns this on for every merge.
    bool          verify_against_cpu = false;
    double        verify_tolerance   = 1e-6;

    void validate() const; // throws InvalidConfigError
};

} // namespace quantforge
