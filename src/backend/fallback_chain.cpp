#include "backend/fallback_chain.hpp"

#include <array>
#include "backend/backend_registry.hpp"
#include "quantforge/errors.hpp"

namespace quantforge::backend {

namespace {
// Degradation order once we start from `desired`.
constexpr std::array<Backend, 3> kDegradationOrder = {
    Backend::CUDA, Backend::OpenMP, Backend::CPU
};
}

ExecutionBackend& FallbackChain::resolve(Backend desired) const {
    auto& registry = BackendRegistry::instance();

    // Try the exact requested backend first.
    if (auto* b = registry.get(desired)) {
        if (b->capabilities().available) return *b;
    }

    // Walk the degradation order starting after `desired`'s position,
    // wrapping so any starting point still reaches CPU last.
    bool started = (desired == Backend::Auto);
    for (Backend candidate : kDegradationOrder) {
        if (!started) {
            if (candidate == desired) started = true;
            continue;
        }
        if (auto* b = registry.get(candidate)) {
            if (b->capabilities().available) return *b;
        }
    }

    // Final safety net: CPU must always be registered.
    if (auto* cpu = registry.get(Backend::CPU)) {
        return *cpu;
    }

    throw BackendUnavailableError(
        "No execution backend is available — even the CPU backend failed to register. "
        "This indicates a broken build.");
}

} // namespace quantforge::backend
