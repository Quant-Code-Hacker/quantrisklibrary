#include "quantforge/plugin.hpp"

namespace quantforge {

PluginRegistry& PluginRegistry::instance() {
    static PluginRegistry registry;
    return registry;
}

void PluginRegistry::register_plugin(std::unique_ptr<RiskPlugin> plugin) {
    plugins_.push_back(std::move(plugin));
}

RiskPlugin* PluginRegistry::find(const std::string& name) const {
    for (const auto& p : plugins_) {
        if (p->name() == name) return p.get();
    }
    return nullptr;
}

std::vector<std::string> PluginRegistry::registered_names() const {
    std::vector<std::string> names;
    names.reserve(plugins_.size());
    for (const auto& p : plugins_) names.push_back(p->name());
    return names;
}

} // namespace quantforge
