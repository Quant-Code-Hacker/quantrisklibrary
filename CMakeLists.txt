cmake_minimum_required(VERSION 3.20)
project(quantforge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Options — every optional subsystem is off-by-default except CPU + tests.
# ---------------------------------------------------------------------------
option(QUANTFORGE_CUDA        "Build the CUDA backend"        OFF)
option(QUANTFORGE_OPENMP      "Build the OpenMP backend"      ON)
option(QUANTFORGE_BUILD_TESTS "Build unit/numerical tests"    ON)
option(QUANTFORGE_BUILD_CLI   "Build the CLI binary"          ON)
option(QUANTFORGE_BUILD_PYTHON "Build Python bindings"        OFF)
option(QUANTFORGE_SANITIZE    "Build with ASan/UBSan"         OFF)

include(cmake/Warnings.cmake)
if(QUANTFORGE_SANITIZE)
    include(cmake/Sanitizers.cmake)
endif()

# ---------------------------------------------------------------------------
# Version header generation (Sec. 14 reproducibility — every result carries
# library_version + git_commit).
# ---------------------------------------------------------------------------
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE QUANTFORGE_GIT_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
endif()
if(NOT QUANTFORGE_GIT_COMMIT)
    set(QUANTFORGE_GIT_COMMIT "unknown")
endif()
configure_file(
    ${CMAKE_SOURCE_DIR}/include/quantforge/version.hpp.in
    ${CMAKE_BINARY_DIR}/generated/quantforge/version.hpp
    @ONLY
)

# ---------------------------------------------------------------------------
# Core library — always built. Portfolio, simulation, risk, execution,
# backend registry, CPU backend. This alone is a complete, usable library.
# ---------------------------------------------------------------------------
add_library(quantforge_core
    src/core/metadata.cpp
    src/core/rng_manager.cpp
    src/core/risk_config.cpp
    src/core/backend_string.cpp
    src/portfolio/portfolio.cpp
    src/portfolio/csv_loader.cpp
    src/portfolio/validation.cpp
    src/simulation/simulation_engine.cpp
    src/risk/risk_engine.cpp
    src/risk/risk_report.cpp
    src/risk/metrics/var.cpp
    src/risk/metrics/cvar.cpp
    src/risk/metrics/distribution_metrics.cpp
    src/risk/metrics/drawdown_metrics.cpp
    src/risk/metrics/ratio_metrics.cpp
    src/risk/metrics/advanced_risk_metrics.cpp
    src/risk/convergence.cpp
    src/execution/orchestrator.cpp
    src/execution/workload_estimator.cpp
    src/execution/backend_selector.cpp
    src/execution/batch_planner.cpp
    src/backend/backend_registry.cpp
    src/backend/fallback_chain.cpp
    src/plugins/plugin_registry.cpp
    cpu/cpu_backend.cpp
    cpu/reference_kernels.cpp
)
target_include_directories(quantforge_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/generated>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}       # so "cpu/...", "openmp/...", "cuda/..." resolve
)
target_compile_features(quantforge_core PUBLIC cxx_std_20)
quantforge_apply_warnings(quantforge_core)

# ---------------------------------------------------------------------------
# Self-registration linkage helper.
#
# CPUBackend, OpenMPBackend, and CUDABackend register themselves via a
# static-initializer side effect (QUANTFORGE_REGISTER_BACKEND) — no other
# translation unit ever calls a symbol from cpu_backend.cpp/openmp_backend.cpp
# directly, since everything goes through BackendRegistry at runtime. When
# these live in *static* libraries, a standard one-pass linker sees nothing
# referencing that object file's symbols and drops it entirely, silently
# deleting the registration and leaving BackendRegistry empty at runtime.
#
# Fix: any executable linking quantforge_core must force-include the whole
# archive rather than let the linker prune "unused" object files. This
# helper centralizes that so every add_executable() doesn't have to know
# about the internal self-registration mechanism. Must be defined before
# any add_executable() below that calls it.
# ---------------------------------------------------------------------------
function(quantforge_link_core target)
    set(_whole_archive_libs quantforge_core)
    if(QUANTFORGE_OPENMP)
        list(APPEND _whole_archive_libs quantforge_openmp)
    endif()
    if(QUANTFORGE_CUDA)
        list(APPEND _whole_archive_libs quantforge_cuda)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_link_libraries(${target} PRIVATE
            -Wl,--whole-archive ${_whole_archive_libs} -Wl,--no-whole-archive)
    elseif(MSVC)
        target_link_libraries(${target} PRIVATE ${_whole_archive_libs})
        foreach(_lib IN LISTS _whole_archive_libs)
            target_link_options(${target} PRIVATE /WHOLEARCHIVE:$<TARGET_FILE:${_lib}>)
        endforeach()
    else()
        target_link_libraries(${target} PRIVATE ${_whole_archive_libs})
    endif()
endfunction()

# ---------------------------------------------------------------------------
# OpenMP backend — optional, registers itself with backend_registry at
# static-init time so quantforge_core never needs to know it exists.
# ---------------------------------------------------------------------------
if(QUANTFORGE_OPENMP)
    find_package(OpenMP REQUIRED)
    add_library(quantforge_openmp openmp/openmp_backend.cpp)
    target_link_libraries(quantforge_openmp PUBLIC quantforge_core OpenMP::OpenMP_CXX)
    target_include_directories(quantforge_openmp PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
    target_compile_definitions(quantforge_core PUBLIC QUANTFORGE_HAS_OPENMP=1)
    target_link_libraries(quantforge_core PUBLIC quantforge_openmp)
endif()

# ---------------------------------------------------------------------------
# CUDA backend — optional, isolated as its own translation unit set so the
# rest of the project builds on machines with no CUDA toolkit at all.
# ---------------------------------------------------------------------------
if(QUANTFORGE_CUDA)
    enable_language(CUDA)
    add_library(quantforge_cuda
        cuda/cuda_backend.cu
        cuda/kernels/mc_path_kernel.cu
        cuda/kernels/loss_kernel.cu
        cuda/kernels/reduction_kernel.cu
        cuda/random/curand_wrapper.cu
        cuda/memory/pinned_allocator.cu
        cuda/memory/stream_pool.cu
    )
    set_target_properties(quantforge_cuda PROPERTIES CUDA_ARCHITECTURES "70;80;90")
    target_link_libraries(quantforge_cuda PUBLIC quantforge_core CUDA::cudart CUDA::curand)
    target_include_directories(quantforge_cuda PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
    target_compile_definitions(quantforge_core PUBLIC QUANTFORGE_HAS_CUDA=1)
    target_link_libraries(quantforge_core PUBLIC quantforge_cuda)
endif()

# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------
if(QUANTFORGE_BUILD_CLI)
    add_executable(quantforge_cli
        cli/main.cpp
        cli/commands/risk_command.cpp
        cli/commands/benchmark_command.cpp
        cli/commands/stress_command.cpp
    )
    quantforge_link_core(quantforge_cli)
    target_include_directories(quantforge_cli PRIVATE ${CMAKE_SOURCE_DIR})
    set_target_properties(quantforge_cli PROPERTIES OUTPUT_NAME quantforge)
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
if(QUANTFORGE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(QUANTFORGE_BUILD_PYTHON)
    add_subdirectory(python)
endif()

install(TARGETS quantforge_core EXPORT quantforge-targets)
install(DIRECTORY include/quantforge DESTINATION include)
