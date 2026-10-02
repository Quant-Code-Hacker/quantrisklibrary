#pragma once

#include <memory>
#include <vector>
#include "quantforge/portfolio.hpp"
#include "quantforge/risk_config.hpp"
#include "quantforge/risk_report.hpp"
#include "quantforge/risk_metric.hpp"
#include "quantforge/simulation_model.hpp"

namespace quantforge {

// Forward declaration — implementation lives in src/risk/risk_engine.cpp
// and src/execution/orchestrator.hpp. Kept opaque here so the public header
// never needs to include execution/backend internals (pimpl-style).
namespace detail { class OrchestratorImpl; }

class RiskEngine {
public:
    RiskEngine();
    ~RiskEngine();

    RiskEngine(const RiskEngine&)            = delete;
    RiskEngine& operator=(const RiskEngine&) = delete;
    RiskEngine(RiskEngine&&) noexcept;
    RiskEngine& operator=(RiskEngine&&) noexcept;

    // The one call most users need. Metric is chosen at compile time via
    // the tag (VaR, CVaR, ...); model defaults to MonteCarlo.
    template <typename Metric, typename Model = MonteCarlo>
    RiskReport calculate(const Portfolio& portfolio, const RiskConfig& config) {
        auto losses = run_simulation(portfolio, config, Model::name);
        RiskReport report = build_report(Metric::name, config);
        report.value_ = compute(Metric{}, losses, config.confidence);
        return report;
    }

private:
    std::vector<double> run_simulation(const Portfolio&, const RiskConfig&, const char* model_name);
    RiskReport build_report(const char* metric_name, const RiskConfig& config);

    std::unique_ptr<detail::OrchestratorImpl> impl_;
};

} // namespace quantforge
