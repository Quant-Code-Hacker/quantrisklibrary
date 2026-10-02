#pragma once

#include "quantforge/backend.hpp"

namespace quantforge::openmp_backend {

class OpenMPBackend final : public ExecutionBackend {
public:
    SimulationResult execute(const SimulationTask& task) override;
    BackendCapabilities capabilities() const override;
    Backend kind() const override { return Backend::OpenMP; }
};

} // namespace quantforge::openmp_backend
