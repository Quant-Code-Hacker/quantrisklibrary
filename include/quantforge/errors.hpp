#pragma once

#include <stdexcept>
#include <string>

namespace quantforge {

// Base of every exception this library throws. Application code can catch
// this alone if it doesn't care about the distinction.
class QuantForgeError : public std::runtime_error {
public:
    explicit QuantForgeError(const std::string& msg) : std::runtime_error(msg) {}
};

// Portfolio failed validation (bad weights, missing prices, NaNs, etc.)
class InvalidPortfolioError : public QuantForgeError {
public:
    explicit InvalidPortfolioError(const std::string& msg) : QuantForgeError(msg) {}
};

// RiskConfig has an inconsistent or out-of-range parameter.
class InvalidConfigError : public QuantForgeError {
public:
    explicit InvalidConfigError(const std::string& msg) : QuantForgeError(msg) {}
};

// A requested backend is unavailable (no GPU, driver mismatch, etc.)
// The fallback chain catches this internally; it only escapes to the user
// if every backend in the chain failed.
class BackendUnavailableError : public QuantForgeError {
public:
    explicit BackendUnavailableError(const std::string& msg) : QuantForgeError(msg) {}
};

// GPU-specific runtime failure (OOM, kernel launch failure, driver error).
class GPUError : public QuantForgeError {
public:
    explicit GPUError(const std::string& msg) : QuantForgeError(msg) {}
};

// Result failed numerical validation against the CPU reference within
// tolerance — this should never be silently downgraded to a warning.
class NumericalValidationError : public QuantForgeError {
public:
    explicit NumericalValidationError(const std::string& msg) : QuantForgeError(msg) {}
};

// Requested metric/model/plugin isn't registered.
class UnknownComponentError : public QuantForgeError {
public:
    explicit UnknownComponentError(const std::string& msg) : QuantForgeError(msg) {}
};

} // namespace quantforge
