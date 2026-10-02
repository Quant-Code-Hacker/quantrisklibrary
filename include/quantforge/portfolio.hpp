#pragma once

#include <string>
#include <vector>

namespace quantforge {

struct Asset {
    std::string symbol;
    double      quantity   = 0.0;   // shares/units held
    double      price      = 0.0;   // last known price
    double      volatility = 0.0;   // annualized, optional — models may estimate instead
};

// Portfolio is a plain data holder. It knows nothing about simulation
// or risk — that separation is what lets SimulationEngine and RiskEngine
// stay decoupled from each other (see docs/architecture.md).
class Portfolio {
public:
    Portfolio() = default;

    void add_asset(const std::string& symbol, double quantity, double price = 0.0);

    static Portfolio from_csv(const std::string& path);

    const std::vector<Asset>& assets() const { return assets_; }
    double total_value() const;
    std::size_t size() const { return assets_.size(); }

private:
    std::vector<Asset> assets_;
};

} // namespace quantforge
