#include "cuda/memory/stream_pool.hpp"

#include "cuda/diagnostics/cuda_error_check.hpp"

namespace quantforge::cuda {

StreamPool::StreamPool(std::size_t num_streams) : streams_(num_streams) {
    for (auto& s : streams_) {
        CUDA_CHECK(cudaStreamCreate(&s));
    }
}

StreamPool::~StreamPool() {
    for (auto& s : streams_) {
        cudaStreamDestroy(s); // best-effort; destructors don't throw
    }
}

cudaStream_t StreamPool::next() {
    cudaStream_t s = streams_[cursor_];
    cursor_ = (cursor_ + 1) % streams_.size();
    return s;
}

} // namespace quantforge::cuda
