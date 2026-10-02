#pragma once

#include <cstdint>
#include <random>

namespace quantforge::core {

// Central seed authority. Every backend derives its actual RNG state from
// here rather than seeding directly off the user's `seed`, so that:
//   1. Multiple batches of the same run get decorrelated sub-streams
//      (via seed_for_batch), instead of restarting the same sequence.
//   2. CPU and GPU backends can be given *matched* sub-seeds, which is a
//      prerequisite for the cross-backend determinism validation work
//      (see docs/numerical_accuracy.md, novelty #6 in the blueprint).
class RngManager {
public:
    explicit RngManager(std::uint64_t base_seed) : base_seed_(base_seed) {}

    // Derives a distinct, reproducible seed for batch `batch_index` of a
    // run using splitmix64-style mixing — cheap and well distributed.
    std::uint64_t seed_for_batch(std::uint64_t batch_index) const;

    std::uint64_t base_seed() const { return base_seed_; }

private:
    std::uint64_t base_seed_;
};

} // namespace quantforge::core
