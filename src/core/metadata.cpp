#include "core/metadata.hpp"
#include "quantforge/version.hpp"

namespace quantforge::core {

ReproMetadata current_metadata() {
    return ReproMetadata{
        .library_version = kVersion,
        .git_commit      = kGitCommit,
    };
}

} // namespace quantforge::core
