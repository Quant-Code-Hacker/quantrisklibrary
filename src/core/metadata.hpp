#pragma once

#include <string>

namespace quantforge::core {

// Everything needed to reproduce a RiskReport later, pulled from the
// build-time generated version.hpp and the CMake-injected git commit.
struct ReproMetadata {
    std::string library_version;
    std::string git_commit;
};

ReproMetadata current_metadata();

} // namespace quantforge::core
