#pragma once

#include <cstddef>
#include <vector>
#include <cuda_runtime.h>

namespace quantforge::cuda {

// Fixed pool of CUDA streams handed out round-robin so consecutive
// batches can overlap H2D/compute/D2H (Sec. 9's "while GPU processes
// batch 0, batch 1 transfers" pipeline). Default size of 4 matches the
// `gpu.streams` config knob in Sec. 19's example YAML.
class StreamPool {
public:
    explicit StreamPool(std::size_t num_streams = 4);
    ~StreamPool();

    StreamPool(const StreamPool&) = delete;
    StreamPool& operator=(const StreamPool&) = delete;

    cudaStream_t next();

private:
    std::vector<cudaStream_t> streams_;
    std::size_t cursor_ = 0;
};

} // namespace quantforge::cuda
