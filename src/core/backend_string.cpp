#include "quantforge/backend.hpp"

namespace quantforge {

std::string to_string(Backend b) {
    switch (b) {
        case Backend::Auto:   return "Auto";
        case Backend::CPU:    return "CPU";
        case Backend::OpenMP: return "OpenMP";
        case Backend::CUDA:   return "CUDA";
    }
    return "Unknown";
}

} // namespace quantforge
