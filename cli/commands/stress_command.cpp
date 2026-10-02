#include "cli/commands/stress_command.hpp"

#include <iostream>
#include "quantforge/quantforge.hpp"

namespace quantforge::cli {

int run_stress_command(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cerr << "Usage: quantforge stress <portfolio.csv> --scenario market-crash\n";
        return 1;
    }

    std::string portfolio_path = args[0];
    std::string scenario = "market-crash";
    for (std::size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "--scenario" && i + 1 < args.size()) scenario = args[++i];
    }

    Portfolio portfolio = Portfolio::from_csv(portfolio_path);

    // Scenario library is intentionally tiny for now — this is the
    // extension point docs/architecture.md points to for adding real
    // named shock scenarios (rate shock, credit spread widening, etc.)
    // via a data-driven scenario registry instead of hardcoded factors.
    double shock_factor = 1.0;
    if (scenario == "market-crash")      shock_factor = 0.70; // -30%
    else if (scenario == "mild-correction") shock_factor = 0.90; // -10%
    else {
        std::cerr << "Unknown scenario: " << scenario << " (known: market-crash, mild-correction)\n";
        return 1;
    }

    Portfolio shocked;
    for (const auto& asset : portfolio.assets()) {
        shocked.add_asset(asset.symbol, asset.quantity, asset.price * shock_factor);
    }

    RiskConfig config;
    RiskEngine engine;
    RiskReport report = engine.calculate<VaR>(shocked, config);

    std::cout << "Scenario: " << scenario << " (shock factor " << shock_factor << ")\n";
    std::cout << report.to_json() << "\n";
    return 0;
}

} // namespace quantforge::cli
