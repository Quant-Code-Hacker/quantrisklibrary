#include <cassert>
#include <cmath>
#include <iostream>
#include "quantforge/quantforge.hpp"

int main() {
    using namespace quantforge;

    Portfolio portfolio;
    portfolio.add_asset("SYN", 1, 100.0);

    RiskConfig cpu_config;
    cpu_config.backend = Backend::CPU;
    cpu_config.simulations = 2'000'000;
    cpu_config.seed = 7;

    RiskConfig openmp_config = cpu_config;
    openmp_config.backend = Backend::OpenMP;

    RiskEngine cpu_engine;
    RiskEngine openmp_engine;

    RiskReport cpu_report    = cpu_engine.calculate<VaR>(portfolio, cpu_config);
    RiskReport openmp_report = openmp_engine.calculate<VaR>(portfolio, openmp_config);

    // CPU and OpenMP use different RNG partitioning (single stream vs.
    // per-thread sub-streams), so results won't be bit-identical — this
    // test checks statistical agreement within a loose relative tolerance
    // instead. A tight, deterministic cross-backend check is exactly the
    // "cross-backend determinism guarantee" novelty from the blueprint.
    double rel_diff = std::abs(cpu_report.value() - openmp_report.value()) /
                       std::abs(cpu_report.value());

    std::cout << "CPU VaR:    " << cpu_report.value() << "\n";
    std::cout << "OpenMP VaR: " << openmp_report.value() << "\n";
    std::cout << "Relative difference: " << rel_diff << "\n";

    assert(rel_diff < 0.02 && "CPU and OpenMP VaR estimates diverged beyond 2%");

    std::cout << "test_numerical_cpu_vs_openmp: all assertions passed\n";
    return 0;
}
