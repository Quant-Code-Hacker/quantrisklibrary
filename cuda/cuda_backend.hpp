#pragma once

#include "quantforge/backend.hpp"

namespace quantforge::cuda {

class CUDABackend final : public ExecutionBackend {
public:
    SimulationResult execute(const SimulationTask& task) override;
    BackendCapabilities capabilities() const override;
    Backend kind() const override { return Backend::CUDA; }
};

} // namespace quantforge::cuda
