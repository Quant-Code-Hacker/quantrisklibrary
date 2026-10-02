#pragma once

#include <cstddef>

namespace quantforge::cuda {

// RAII wrapper around cudaMallocHost/cudaFreeHost. Pinned memory is
// required for truly asynchronous H2D/D2H transfers (Sec. 9) — a
// pageable std::vector silently forces the driver to stage through an
// internal pinned buffer, defeating the point of using streams at all.
class PinnedBuffer {
public:
    explicit PinnedBuffer(std::size_t num_doubles);
    ~PinnedBuffer();

    PinnedBuffer(const PinnedBuffer&) = delete;
    PinnedBuffer& operator=(const PinnedBuffer&) = delete;

    double* data() { return data_; }
    std::size_t size() const { return size_; }

private:
    double* data_ = nullptr;
    std::size_t size_ = 0;
};

} // namespace quantforge::cuda
