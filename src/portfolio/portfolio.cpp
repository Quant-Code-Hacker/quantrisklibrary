#include "quantforge/portfolio.hpp"
#include "portfolio/csv_loader.hpp"

namespace quantforge {

void Portfolio::add_asset(const std::string& symbol, double quantity, double price) {
    assets_.push_back(Asset{symbol, quantity, price, 0.0});
}

double Portfolio::total_value() const {
    double total = 0.0;
    for (const auto& a : assets_) {
        total += a.quantity * a.price;
    }
    return total;
}

Portfolio Portfolio::from_csv(const std::string& path) {
    return portfolio::load_csv(path);
}

} // namespace quantforge
