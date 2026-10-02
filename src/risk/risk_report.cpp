#include "quantforge/risk_report.hpp"

#include <sstream>

namespace quantforge {

std::string RiskReport::to_json() const {
    std::ostringstream ss;
    ss << "{"
       << "\"metric\":\"" << metric_name << "\","
       << "\"value\":" << value_ << ","
       << "\"confidence\":" << confidence << ","
       << "\"simulations_run\":" << simulations_run << ","
       << "\"seed\":" << seed << ","
       << "\"backend\":\"" << to_string(backend_used) << "\","
       << "\"library_version\":\"" << library_version << "\","
       << "\"git_commit\":\"" << git_commit << "\","
       << "\"total_ms\":" << total_ms << ","
       << "\"compute_ms\":" << compute_ms
       << "}";
    return ss.str();
}

} // namespace quantforge
