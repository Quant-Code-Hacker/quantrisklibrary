#include "cli/commands/benchmark_command.hpp"

#include <iostream>
#include "quantforge/quantforge.hpp"

namespace quantforge::cli {

int run_benchmark_command(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cerr << "Usage: quantforge benchmark <portfolio.csv> [--simulations 1000000]\n";
        return 1;
    }

    std::string portfolio_path = args[0];
    std::uint64_t simulations = 1'000'000;
    for (std::size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "--simulations" && i + 1 < args.size()) {
            simulations = std::stoull(args[++i]);
        }
    }

    Portfolio portfolio = Portfolio::from_csv(portfolio_path);

    const std::vector<std::pair<const char*, Backend>> backends = {
        {"cpu", Backend::CPU}, {"openmp", Backend::OpenMP}, {"cuda", Backend::CUDA},
    };

    std::cout << "backend,total_ms,compute_ms,simulations\n";
    // NOTE: RiskEngine silently falls back (CUDA -> OpenMP -> CPU) via
    // backend::FallbackChain rather than throwing when a backend isn't
    // compiled in, so an "unavailable" CUDA row here will actually show
    // whichever backend it fell back to. A benchmark command with real
    // per-backend isolation would query backend::BackendRegistry directly
    // instead of going through RiskEngine's public (fallback-aware) path.
    for (const auto& [name, backend] : backends) {
        RiskConfig config;
        config.simulations = simulations;
        config.backend = backend;

        try {
            RiskEngine engine;
            RiskReport report = engine.calculate<VaR>(portfolio, config);
            std::cout << name << "," << report.total_ms << "," << report.compute_ms
                      << "," << report.simulations_run << "\n";
        } catch (const BackendUnavailableError&) {
            std::cout << name << ",unavailable,,\n";
        }
    }
    return 0;
}

} // namespace quantforge::cli
