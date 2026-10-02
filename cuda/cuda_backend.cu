#include "cuda/cuda_backend.hpp"

#include <chrono>
#include <cuda_runtime.h>
#include "cuda/diagnostics/cuda_error_check.hpp"
#include "cuda/random/curand_wrapper.hpp"
#include "cuda/kernels/mc_path_kernel.hpp"
#include "cuda/memory/pinned_allocator.hpp"
#include "backend/backend_registry.hpp"

namespace quantforge::cuda {

namespace {
constexpr int kThreadsPerBlock = 256;
}

SimulationResult CUDABackend::execute(const SimulationTask& task) {
    auto wall_start = std::chrono::steady_clock::now();

    SimulationResult result;
    result.backend_used = Backend::CUDA;

    const std::uint64_t n = task.num_paths;
    const int blocks = static_cast<int>((n + kThreadsPerBlock - 1) / kThreadsPerBlock);

    curandState* d_states = nullptr;
    double* d_losses = nullptr;
    CUDA_CHECK(cudaMalloc(&d_states, n * sizeof(curandState)));
    CUDA_CHECK(cudaMalloc(&d_losses, n * sizeof(double)));

    init_curand_states<<<blocks, kThreadsPerBlock>>>(d_states, task.seed, n);
    CUDA_CHECK(cudaGetLastError());

    mc_path_kernel<<<blocks, kThreadsPerBlock>>>(
        d_states, task.drift, task.volatility, task.initial_value, d_losses, n);
    CUDA_CHECK(cudaGetLastError());

    // Pinned host buffer for a genuinely async D2H copy. For a single
    // batch this still synchronizes below; the StreamPool + multi-batch
    // overlap described in Sec. 9 applies once Orchestrator issues
    // consecutive batches on different streams rather than one big task.
    PinnedBuffer host_buffer(n);
    CUDA_CHECK(cudaMemcpy(host_buffer.data(), d_losses, n * sizeof(double), cudaMemcpyDeviceToHost));

    result.terminal_losses.assign(host_buffer.data(), host_buffer.data() + n);

    cudaFree(d_states);
    cudaFree(d_losses);

    auto wall_end = std::chrono::steady_clock::now();
    result.elapsed_ms = std::chrono::duration<double, std::milli>(wall_end - wall_start).count();
    return result;
}

BackendCapabilities CUDABackend::capabilities() const {
    BackendCapabilities caps;
    caps.kind = Backend::CUDA;

    int device_count = 0;
    cudaError_t err = cudaGetDeviceCount(&device_count);
    caps.available = (err == cudaSuccess && device_count > 0);

    if (caps.available) {
        cudaDeviceProp props{};
        cudaGetDeviceProperties(&props, 0);
        caps.max_memory_bytes = props.totalGlobalMem;
        caps.parallelism_hint = props.multiProcessorCount;
    }
    return caps;
}

} // namespace quantforge::cuda

QUANTFORGE_REGISTER_BACKEND(quantforge::cuda::CUDABackend)
