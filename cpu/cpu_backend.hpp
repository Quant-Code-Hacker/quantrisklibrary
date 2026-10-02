#pragma once

#include "quantforge/backend.hpp"

namespace quantforge::cpu {

class CPUBackend final : public ExecutionBackend {
public:
    SimulationResult execute(const SimulationTask& task) override;
    BackendCapabilities capabilities() const override;
    Backend kind() const override { return Backend::CPU; }
};

} // namespace quantforge::cpu
