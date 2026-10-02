#include "portfolio/validation.hpp"

#include <cmath>
#include "quantforge/errors.hpp"

namespace quantforge::portfolio {

void validate(const Portfolio& p, const RiskConfig& config) {
    if (p.size() == 0) {
        throw InvalidPortfolioError("Portfolio has no assets.");
    }

    for (const auto& asset : p.assets()) {
        if (asset.symbol.empty()) {
            throw InvalidPortfolioError("Asset with empty symbol.");
        }
        if (std::isnan(asset.quantity) || std::isnan(asset.price)) {
            throw InvalidPortfolioError("NaN quantity/price for asset: " + asset.symbol);
        }
        if (asset.price < 0.0) {
            throw InvalidPortfolioError("Negative price for asset: " + asset.symbol);
        }
    }

    config.validate();
}

} // namespace quantforge::portfolio
