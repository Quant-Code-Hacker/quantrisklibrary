#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace quantforge {

// Base class for a user-supplied risk model/metric plugin. Loaded either
// statically (linked in) or dynamically (dlopen'd .so, see
// src/plugins/plugin_loader.hpp).
class RiskPlugin {
public:
    virtual ~RiskPlugin() = default;
    virtual std::string name() const = 0;
    virtual double compute(const std::vector<double>& losses, double confidence) = 0;
};

// Global registry — plugins register themselves at static-init time via
// QUANTFORGE_REGISTER_PLUGIN(ClassName), or dynamically at runtime.
class PluginRegistry {
public:
    static PluginRegistry& instance();

    void register_plugin(std::unique_ptr<RiskPlugin> plugin);
    RiskPlugin* find(const std::string& name) const;
    std::vector<std::string> registered_names() const;

private:
    std::vector<std::unique_ptr<RiskPlugin>> plugins_;
};

#define QF_PLUGIN_CONCAT_IMPL(a, b) a##b
#define QF_PLUGIN_CONCAT(a, b) QF_PLUGIN_CONCAT_IMPL(a, b)

// See backend_registry.hpp for why this doesn't paste ClassName directly:
// plugin classes may be namespace-qualified.
#define QUANTFORGE_REGISTER_PLUGIN(ClassName)                                \
    namespace {                                                             \
        struct QF_PLUGIN_CONCAT(PluginRegistrar_, __LINE__) {              \
            QF_PLUGIN_CONCAT(PluginRegistrar_, __LINE__)() {                \
                quantforge::PluginRegistry::instance().register_plugin(     \
                    std::make_unique<ClassName>());                        \
            }                                                              \
        };                                                                  \
        static QF_PLUGIN_CONCAT(PluginRegistrar_, __LINE__)                 \
            QF_PLUGIN_CONCAT(plugin_registrar_instance_, __LINE__);        \
    }

} // namespace quantforge
