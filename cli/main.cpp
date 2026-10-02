#include <iostream>
#include <vector>
#include <string>

#include "cli/commands/risk_command.hpp"
#include "cli/commands/benchmark_command.hpp"
#include "cli/commands/stress_command.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: quantforge <risk|benchmark|stress> [args...]\n";
        return 1;
    }

    std::string command = argv[1];
    std::vector<std::string> args(argv + 2, argv + argc);

    if (command == "risk")      return quantforge::cli::run_risk_command(args);
    if (command == "benchmark") return quantforge::cli::run_benchmark_command(args);
    if (command == "stress")    return quantforge::cli::run_stress_command(args);

    std::cerr << "Unknown command: " << command << "\n";
    return 1;
}
