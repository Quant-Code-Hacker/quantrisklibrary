#include "cli/commands/risk_command.hpp"

#include <iostream>
#include "quantforge/quantforge.hpp"

namespace quantforge::cli {

namespace {

Backend parse_backend(const std::string& s) {
    if (s == "cpu")    return Backend::CPU;
    if (s == "openmp") return Backend::OpenMP;
    if (s == "cuda")   return Backend::CUDA;
    return Backend::Auto;
}

} // namespace

int run_risk_command(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cerr << "Usage: quantforge risk <portfolio.csv> "
                     "[--metric var|cvar] [--confidence 0.99] "
                     "[--simulations 1000000] [--backend auto|cpu|openmp|cuda] "
                     "[--seed 42] [--adaptive] [--verify]\n";
        return 1;
    }

    std::string portfolio_path = args[0];
    std::string metric = "var";
    RiskConfig config;

    for (std::size_t i = 1; i < args.size(); ++i) {
        const std::string& arg = args[i];
        auto next = [&]() -> std::string {
            return (i + 1 < args.size()) ? args[++i] : std::string{};
        };

        if (arg == "--metric")            metric = next();
        else if (arg == "--confidence")   config.confidence = std::stod(next());
        else if (arg == "--simulations")  config.simulations = std::stoull(next());
        else if (arg == "--backend")      config.backend = parse_backend(next());
        else if (arg == "--seed")         config.seed = std::stoull(next());
        else if (arg == "--adaptive")     config.adaptive = true;
        else if (arg == "--verify")       config.verify_against_cpu = true;
        else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return 1;
        }
    }

    try {
        Portfolio portfolio = Portfolio::from_csv(portfolio_path);
        RiskEngine engine;

        RiskReport report = (metric == "cvar")
            ? engine.calculate<CVaR>(portfolio, config)
            : engine.calculate<VaR>(portfolio, config);

        std::cout << report.to_json() << "\n";
        return 0;
    } catch (const QuantForgeError& e) {
        std::cerr << "quantforge error: " << e.what() << "\n";
        return 1;
    }
}

} // namespace quantforge::cli
