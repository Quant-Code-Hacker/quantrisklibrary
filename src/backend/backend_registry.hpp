#pragma once

#include <memory>
#include <vector>
#include "quantforge/backend.hpp"

namespace quantforge::backend {

// Central place every backend (CPU always, OpenMP/CUDA conditionally via
// their own translation units) registers itself at static-init time. This
// is what lets quantforge_core stay backend-agnostic: it never
// `#include`s cpu_backend.hpp/openmp_backend.hpp/cuda_backend.hpp
// directly — it just asks the registry for "whatever implements X".
class BackendRegistry {
public:
    static BackendRegistry& instance();

    void register_backend(std::unique_ptr<ExecutionBackend> backend);

    // Returns nullptr if that backend kind was never registered/compiled in.
    ExecutionBackend* get(Backend kind) const;

    std::vector<Backend> available_backends() const;

private:
    std::vector<std::unique_ptr<ExecutionBackend>> backends_;
};

#define QF_CONCAT_IMPL(a, b) a##b
#define QF_CONCAT(a, b) QF_CONCAT_IMPL(a, b)

// NOTE: ClassName may be namespace-qualified (e.g. quantforge::cpu::CPUBackend),
// so it can't be used in `##` token-pasting to build an identifier. The
// registrar struct name is instead derived from __LINE__, which only needs
// to be unique within this anonymous namespace / translation unit.
#define QUANTFORGE_REGISTER_BACKEND(ClassName)                               \
    namespace {                                                             \
        struct QF_CONCAT(BackendRegistrar_, __LINE__) {                    \
            QF_CONCAT(BackendRegistrar_, __LINE__)() {                     \
                quantforge::backend::BackendRegistry::instance()            \
                    .register_backend(std::make_unique<ClassName>());       \
            }                                                              \
        };                                                                  \
        static QF_CONCAT(BackendRegistrar_, __LINE__) QF_CONCAT(backend_registrar_instance_, __LINE__); \
    }

} // namespace quantforge::backend
