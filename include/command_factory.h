#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include <memory>
#include <string>
#include <vector>

class AbstractCommand;

/**
 * @brief Creates command objects based on command name and arguments
 */
class CommandFactory {
public:
    /**
     * @brief Creates appropriate command object
     * @param name Command name
     * @param args Command arguments
     * @return Unique pointer to created command
     */
    std::unique_ptr<AbstractCommand> createCommand(
        const std::string& name, const std::vector<std::string>& args);

private:
    bool isBuiltinCommand(const std::string& name) const;
};

#endif
