#include "cuda/memory/pinned_allocator.hpp"

#include <cuda_runtime.h>
#include "cuda/diagnostics/cuda_error_check.hpp"

namespace quantforge::cuda {

PinnedBuffer::PinnedBuffer(std::size_t num_doubles) : size_(num_doubles) {
    CUDA_CHECK(cudaMallocHost(reinterpret_cast<void**>(&data_), num_doubles * sizeof(double)));
}

PinnedBuffer::~PinnedBuffer() {
    if (data_) {
        cudaFreeHost(data_); // best-effort in destructor; never throws
    }
}

} // namespace quantforge::cuda
