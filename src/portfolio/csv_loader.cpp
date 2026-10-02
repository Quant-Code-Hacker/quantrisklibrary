#include "portfolio/csv_loader.hpp"

#include <fstream>
#include <sstream>
#include "quantforge/errors.hpp"

namespace quantforge::portfolio {

namespace {
std::vector<std::string> split(const std::string& line, char delim) {
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, delim)) out.push_back(field);
    return out;
}
} // namespace

Portfolio load_csv(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw InvalidPortfolioError("Could not open portfolio CSV: " + path);
    }

    Portfolio portfolio;
    std::string line;
    bool first = true;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (first) { first = false; continue; } // skip header row

        auto fields = split(line, ',');
        if (fields.size() < 3) {
            throw InvalidPortfolioError("Malformed row in portfolio CSV: " + line);
        }

        try {
            const std::string& symbol = fields[0];
            double quantity = std::stod(fields[1]);
            double price    = std::stod(fields[2]);
            portfolio.add_asset(symbol, quantity, price);
        } catch (const std::exception&) {
            throw InvalidPortfolioError("Non-numeric quantity/price in row: " + line);
        }
    }

    return portfolio;
}

} // namespace quantforge::portfolio
