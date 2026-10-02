#pragma once

#include <cuda_runtime.h>
#include <sstream>
#include "quantforge/errors.hpp"

#define CUDA_CHECK(expr)                                                     \
    do {                                                                     \
        cudaError_t _err = (expr);                                          \
        if (_err != cudaSuccess) {                                          \
            std::ostringstream _ss;                                        \
            _ss << "CUDA error at " << __FILE__ << ":" << __LINE__          \
                << " in `" #expr "`: " << cudaGetErrorString(_err);        \
            throw quantforge::GPUError(_ss.str());                         \
        }                                                                    \
    } while (0)
