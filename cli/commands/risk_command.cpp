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
                     "[--metric var|cvar|volatility|downsidedeviation|semideviation|"
                     "skewness|kurtosis|maxdrawdown|averagedrawdown|ulcerindex|"
                     "sharperatio|sortinoratio|calmarratio|informationratio|omegaratio|"
                     "marginalvar|componentvar|incrementalvar|beta|trackingerror|"
                     "sterlingratio|burkeratio|diversificationratio|hhi] "
                     "[--confidence 0.99] "
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
        RiskReport report;

        // Dispatch based on metric name
        if (metric == "var" || metric == "VaR") {
            report = engine.calculate<VaR>(portfolio, config);
        } else if (metric == "cvar" || metric == "CVaR") {
            report = engine.calculate<CVaR>(portfolio, config);
        } else if (metric == "volatility" || metric == "Volatility") {
            report = engine.calculate<Volatility>(portfolio, config);
        } else if (metric == "downsidedeviation" || metric == "DownsideDeviation") {
            report = engine.calculate<DownsideDeviation>(portfolio, config);
        } else if (metric == "semideviation" || metric == "SemiDeviation") {
            report = engine.calculate<SemiDeviation>(portfolio, config);
        } else if (metric == "skewness" || metric == "Skewness") {
            report = engine.calculate<Skewness>(portfolio, config);
        } else if (metric == "kurtosis" || metric == "Kurtosis") {
            report = engine.calculate<Kurtosis>(portfolio, config);
        } else if (metric == "maxdrawdown" || metric == "MaxDrawdown") {
            report = engine.calculate<MaxDrawdown>(portfolio, config);
        } else if (metric == "averagedrawdown" || metric == "AverageDrawdown") {
            report = engine.calculate<AverageDrawdown>(portfolio, config);
        } else if (metric == "ulcerindex" || metric == "UlcerIndex") {
            report = engine.calculate<UlcerIndex>(portfolio, config);
        } else if (metric == "sharperatio" || metric == "SharpeRatio") {
            report = engine.calculate<SharpeRatio>(portfolio, config);
        } else if (metric == "sortinoratio" || metric == "SortinoRatio") {
            report = engine.calculate<SortinoRatio>(portfolio, config);
        } else if (metric == "calmarratio" || metric == "CalmarRatio") {
            report = engine.calculate<CalmarRatio>(portfolio, config);
        } else if (metric == "informationratio" || metric == "InformationRatio") {
            report = engine.calculate<InformationRatio>(portfolio, config);
        } else if (metric == "omegaratio" || metric == "OmegaRatio") {
            report = engine.calculate<OmegaRatio>(portfolio, config);
        } else if (metric == "marginalvar" || metric == "MarginalVaR") {
            report = engine.calculate<MarginalVaR>(portfolio, config);
        } else if (metric == "componentvar" || metric == "ComponentVaR") {
            report = engine.calculate<ComponentVaR>(portfolio, config);
        } else if (metric == "incrementalvar" || metric == "IncrementalVaR") {
            report = engine.calculate<IncrementalVaR>(portfolio, config);
        } else if (metric == "beta" || metric == "Beta") {
            report = engine.calculate<Beta>(portfolio, config);
        } else if (metric == "trackingerror" || metric == "TrackingError") {
            report = engine.calculate<TrackingError>(portfolio, config);
        } else if (metric == "sterlingratio" || metric == "SterlingRatio") {
            report = engine.calculate<SterlingRatio>(portfolio, config);
        } else if (metric == "burkeratio" || metric == "BurkeRatio") {
            report = engine.calculate<BurkeRatio>(portfolio, config);
        } else if (metric == "diversificationratio" || metric == "DiversificationRatio") {
            report = engine.calculate<DiversificationRatio>(portfolio, config);
        } else if (metric == "hhi" || metric == "HHI") {
            report = engine.calculate<HHI>(portfolio, config);
        } else {
            std::cerr << "Unknown metric: " << metric << "\n";
            return 1;
        }

        std::cout << report.to_json() << "\n";
        return 0;
    } catch (const QuantForgeError& e) {
        std::cerr << "quantforge error: " << e.what() << "\n";
        return 1;
    }
}

} // namespace quantforge::cli
