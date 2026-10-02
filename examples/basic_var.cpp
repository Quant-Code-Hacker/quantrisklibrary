// Build: this file is not wired into CMake by default — compile manually
// against the installed library, or add an add_executable() in
// examples/CMakeLists.txt if you want it built as part of the project.
#include <iostream>
#include "quantforge/quantforge.hpp"

int main() {
    using namespace quantforge;

    Portfolio portfolio;
    portfolio.add_asset("AAPL", 1000, 190.0);
    portfolio.add_asset("MSFT", 500, 420.0);

    RiskConfig config;
    config.confidence  = 0.99;
    config.simulations = 5'000'000;
    config.seed         = 42;
    config.backend       = Backend::Auto;

    RiskEngine engine;
    RiskReport var_result  = engine.calculate<VaR>(portfolio, config);
    RiskReport cvar_result = engine.calculate<CVaR>(portfolio, config);

    std::cout << "99% 1-day VaR:  " << var_result.value()  << "\n";
    std::cout << "99% 1-day CVaR: " << cvar_result.value() << "\n";
    std::cout << "Backend used:   " << to_string(var_result.backend_used) << "\n";
    std::cout << "Full report:    " << var_result.to_json() << "\n";

    return 0;
}
