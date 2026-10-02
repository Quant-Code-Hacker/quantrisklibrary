#include "quantforge/risk_config.hpp"
#include "quantforge/errors.hpp"

namespace quantforge {

void RiskConfig::validate() const {
    if (confidence <= 0.0 || confidence >= 1.0) {
        throw InvalidConfigError("confidence must be in (0, 1).");
    }
    if (simulations == 0) {
        throw InvalidConfigError("simulations must be > 0.");
    }
    if (adaptive && target_rel_std_err <= 0.0) {
        throw InvalidConfigError("target_rel_std_err must be > 0 when adaptive=true.");
    }
    if (verify_against_cpu && verify_tolerance <= 0.0) {
        throw InvalidConfigError("verify_tolerance must be > 0.");
    }
}

} // namespace quantforge

