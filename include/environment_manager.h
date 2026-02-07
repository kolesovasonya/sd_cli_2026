#ifndef ENVIRONMENT_MANAGER_H
#define ENVIRONMENT_MANAGER_H

#include <map>
#include <string>

/**
 * @brief Manages environment variables (singleton)
 */
class EnvironmentManager {
public:
    /**
     * @brief Gets singleton instance
     * @return Reference to singleton instance
     */
    static EnvironmentManager& getInstance();

    /**
     * @brief Sets environment variable
     * @param name Variable name
     * @param value Variable value
     */
    void setVariable(const std::string& name, const std::string& value);

    /**
     * @brief Gets environment variable value
     * @param name Variable name
     * @return Variable value, or empty string if not found
     */
    std::string getVariable(const std::string& name) const;

    /**
     * @brief Checks if variable exists
     * @param name Variable name
     * @return true if variable exists
     */
    bool hasVariable(const std::string& name) const;

    /**
     * @brief Gets all custom variables
     * @return Map of all custom variables
     */
    std::map<std::string, std::string> getAllVariables() const;

private:
    EnvironmentManager() = default;
    EnvironmentManager(const EnvironmentManager&) = delete;
    EnvironmentManager& operator=(const EnvironmentManager&) = delete;

    std::map<std::string, std::string> variables_;
};

#endif
