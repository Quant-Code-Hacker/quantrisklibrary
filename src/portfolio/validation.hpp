#pragma once

#include "quantforge/portfolio.hpp"
#include "quantforge/risk_config.hpp"

namespace quantforge::portfolio {

// Throws InvalidPortfolioError / InvalidConfigError. Called first thing
// inside RiskEngine::calculate before any simulation work happens.
void validate(const Portfolio& portfolio, const RiskConfig& config);

} // namespace quantforge::portfolio
