#include "backend/backend_registry.hpp"

#include <algorithm>

namespace quantforge::backend {

BackendRegistry& BackendRegistry::instance() {
    static BackendRegistry registry;
    return registry;
}

void BackendRegistry::register_backend(std::unique_ptr<ExecutionBackend> backend) {
    backends_.push_back(std::move(backend));
}

ExecutionBackend* BackendRegistry::get(Backend kind) const {
    for (const auto& b : backends_) {
        if (b->kind() == kind) return b.get();
    }
    return nullptr;
}

std::vector<Backend> BackendRegistry::available_backends() const {
    std::vector<Backend> kinds;
    kinds.reserve(backends_.size());
    for (const auto& b : backends_) kinds.push_back(b->kind());
    return kinds;
}

} // namespace quantforge::backend
