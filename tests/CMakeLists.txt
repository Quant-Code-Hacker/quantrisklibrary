# Lightweight test setup: no external test framework dependency, each test
# binary is a plain main() that asserts and returns non-zero on failure.
# Swap in Catch2/GoogleTest here once the project has network access to
# fetch dependencies (FetchContent) in a real CI environment.

add_executable(test_portfolio unit/test_portfolio.cpp)
quantforge_link_core(test_portfolio)
add_test(NAME test_portfolio COMMAND test_portfolio)

add_executable(test_var_metric unit/test_var_metric.cpp)
quantforge_link_core(test_var_metric)
add_test(NAME test_var_metric COMMAND test_var_metric)

add_executable(test_numerical_cpu_vs_openmp numerical/test_cpu_vs_openmp.cpp)
quantforge_link_core(test_numerical_cpu_vs_openmp)
add_test(NAME test_numerical_cpu_vs_openmp COMMAND test_numerical_cpu_vs_openmp)
