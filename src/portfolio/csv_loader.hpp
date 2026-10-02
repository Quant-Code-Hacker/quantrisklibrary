#pragma once

#include <string>
#include "quantforge/portfolio.hpp"

namespace quantforge::portfolio {

// Expects columns: symbol,quantity,price[,volatility]
Portfolio load_csv(const std::string& path);

} // namespace quantforge::portfolio
