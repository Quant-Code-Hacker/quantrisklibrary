#include "core/rng_manager.hpp"

namespace quantforge::core {

namespace {
// splitmix64 — fast, well-distributed seed mixer. Used only to derive
// sub-seeds, never as the simulation RNG itself.
std::uint64_t splitmix64(std::uint64_t x) {
    x += 0x9E3779B97f4A7C15ULL;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
    return x ^ (x >> 31);
}
} // namespace

std::uint64_t RngManager::seed_for_batch(std::uint64_t batch_index) const {
    return splitmix64(base_seed_ ^ splitmix64(batch_index));
}

} // namespace quantforge::core
