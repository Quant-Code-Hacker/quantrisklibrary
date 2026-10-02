# QuantForge

A production-architected, backend-agnostic financial risk computing engine.
CPU, OpenMP, and CUDA execution behind one stable API, with deterministic
seeding, adaptive convergence, numerical cross-validation, and a
plugin-extensible metric/model system.

```cpp
#include <quantforge/quantforge.hpp>

quantforge::Portfolio portfolio;
portfolio.add_asset("AAPL", 1000, 190.0);

quantforge::RiskConfig config;
config.confidence = 0.99;
config.simulations = 5'000'000;

quantforge::RiskEngine engine;
auto result = engine.calculate<quantforge::VaR>(portfolio, config);

std::cout << result.value() << "\n";
```

## Build

```bash
mkdir build && cd build
cmake .. -DQUANTFORGE_OPENMP=ON -DQUANTFORGE_CUDA=OFF
cmake --build . -j
ctest --output-on-failure
```

Enable CUDA (requires the toolkit):

```bash
cmake .. -DQUANTFORGE_CUDA=ON
```

## CLI

```bash
./quantforge_cli risk examples/sample_portfolio.csv --metric var --confidence 0.99 --simulations 1000000
./quantforge_cli benchmark examples/sample_portfolio.csv --simulations 1000000
./quantforge_cli stress examples/sample_portfolio.csv --scenario market-crash
```

## Architecture at a glance

```
RiskEngine  ->  Orchestrator  ->  WorkloadEstimator -> BackendSelector -> FallbackChain
                                        |
                                  BatchPlanner
                                        |
                              SimulationEngine.run(task, backend)
                                        |
                         CPUBackend | OpenMPBackend | CUDABackend
                                        |
                              risk::compute(VaR|CVaR, losses)
                                        |
                                   RiskReport
```

Key separations, enforced by the code layout, not just convention:

- `ExecutionBackend` never knows what a Portfolio or a risk metric is — it
  only executes a `SimulationTask` and returns raw losses.
- `SimulationEngine` never knows what metric will be computed from its
  output.
- `RiskEngine` never knows which backend produced the numbers — that's
  entirely `Orchestrator`'s job, mediated by `BackendSelector` and
  `FallbackChain`.

See `include/quantforge/*.hpp` for the full public API surface and
`src/execution/orchestrator.cpp` for the actual request path.

## What's implemented vs. stubbed

**Working end-to-end (CPU + OpenMP, no CUDA toolkit required):**
Portfolio loading/validation, GBM Monte Carlo simulation, VaR/CVaR
metrics, backend selection + fallback, adaptive convergence stopping,
reproducibility metadata, CLI, unit + numerical validation tests.

**Present as real interfaces, needs a CUDA toolkit + GPU to exercise:**
`cuda/` backend — fused path-generation kernel, hierarchical
warp/block reduction, pinned memory, stream pool. Written to be
formula-identical to the CPU reference so `tests/numerical/` can validate
it once built with `-DQUANTFORGE_CUDA=ON` on a CUDA-capable machine.

**Sketched as extension points, not yet built:**
Python bindings (`python/`), C ABI (`capi/`), plugin dynamic loading
(`src/plugins/plugin_loader.hpp` — not yet created), multi-asset
correlated simulation (currently single aggregate GBM position), the
"learned backend selection" and other novelties described in
`docs/architecture.md`.

## Novel directions worth pursuing further

1. **Learned backend selection** — replace `BackendSelector`'s static
   thresholds with a small model trained on (workload profile -> fastest
   backend) history logged from real runs.
2. **Cross-backend deterministic seeding** — `core::RngManager` already
   derives batch sub-seeds centrally; extending this so CPU and CUDA
   produce *distributionally matched* results for the same seed is an
   open, citable problem.
3. **Mixed-precision Monte Carlo** — FP32 path generation, FP64 reduction,
   with a measured error-vs-speed tradeoff.
4. **Variance-aware adaptive batching** — tie `BatchPlanner` batch sizes to
   the live convergence estimate from `risk::check_convergence`, not just
   GPU memory.
