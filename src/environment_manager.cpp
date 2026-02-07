#include "environment_manager.h"

#include <cstdlib>

EnvironmentManager& EnvironmentManager::getInstance() {
    static EnvironmentManager instance;
    return instance;
}

void EnvironmentManager::setVariable(const std::string& name,
                                     const std::string& value) {
    variables_[name] = value;
}

std::string EnvironmentManager::getVariable(const std::string& name) const {
    auto it = variables_.find(name);
    if (it != variables_.end()) {
        return it->second;
    }

    const char* envValue = std::getenv(name.c_str());
    return envValue ? std::string(envValue) : "";
}

bool EnvironmentManager::hasVariable(const std::string& name) const {
    return variables_.find(name) != variables_.end() ||
           std::getenv(name.c_str()) != nullptr;
}

std::map<std::string, std::string> EnvironmentManager::getAllVariables() const {
    return variables_;
}
