#include "quantforge/risk_engine.hpp"

#include "execution/orchestrator.hpp"
#include "core/metadata.hpp"

namespace quantforge {

namespace detail {

class OrchestratorImpl {
public:
    execution::Orchestrator orchestrator;
    execution::OrchestrationOutcome last_outcome; // populated by run_simulation,
                                                   // consumed by build_report
};

} // namespace detail

RiskEngine::RiskEngine() : impl_(std::make_unique<detail::OrchestratorImpl>()) {}
RiskEngine::~RiskEngine() = default;
RiskEngine::RiskEngine(RiskEngine&&) noexcept = default;
RiskEngine& RiskEngine::operator=(RiskEngine&&) noexcept = default;

std::vector<double> RiskEngine::run_simulation(const Portfolio& portfolio,
                                                const RiskConfig& config,
                                                const char* /*model_name*/) {
    impl_->last_outcome = impl_->orchestrator.run(portfolio, config);
    return impl_->last_outcome.losses;
}

RiskReport RiskEngine::build_report(const char* metric_name, const RiskConfig& config) {
    RiskReport report;
    auto meta = core::current_metadata();

    report.metric_name      = metric_name;
    report.confidence       = config.confidence;
    report.simulations_run  = impl_->last_outcome.simulations_run;
    report.seed              = config.seed;
    report.backend_used      = impl_->last_outcome.backend_used;
    report.library_version   = meta.library_version;
    report.git_commit        = meta.git_commit;
    report.total_ms          = impl_->last_outcome.total_ms;
    report.compute_ms        = impl_->last_outcome.compute_ms;

    return report;
}

} // namespace quantforge
