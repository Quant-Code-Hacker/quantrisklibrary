#pragma once

#include <cstdint>
#include <string>
#include "quantforge/backend.hpp"

namespace quantforge {

// Every number this library ever hands back to a user is wrapped in this.
// The point: six months from now, someone should be able to look at a
// RiskReport and reproduce it exactly, or at least know why they can't.
class RiskReport {
public:
    double value() const { return value_; }

    // Reproducibility metadata
    std::string   metric_name;
    double        confidence      = 0.0;
    std::uint64_t simulations_run = 0;   // may differ from config.simulations if adaptive
    std::uint64_t seed            = 0;
    Backend       backend_used    = Backend::CPU;
    std::string   library_version;
    std::string   git_commit;

    // Observability (Sec. 17)
    double total_ms      = 0.0;
    double compute_ms     = 0.0;
    double h2d_ms         = 0.0;
    double d2h_ms          = 0.0;
    double reduction_ms    = 0.0;

    double value_ = 0.0;

    std::string to_json() const;
};

} // namespace quantforge
