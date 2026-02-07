#ifndef ECHO_COMMAND_H
#define ECHO_COMMAND_H

#include "builtin_command.h"
#include <vector>
#include <string>

/**
 * @brief Built-in echo command - outputs arguments
 */
class EchoCommand : public BuiltinCommand {
public:
    /**
     * @brief Constructs echo command
     * @param args Arguments to output
     */
    explicit EchoCommand(const std::vector<std::string>& args);
    
    /**
     * @brief Executes echo command
     * @param input Input stream
     * @param output Output stream
     * @param error Error stream
     * @return Exit code (always 0)
     */
    int execute(std::istream& input, std::ostream& output, std::ostream& error) override;
    
private:
    std::vector<std::string> args_;
};

#endif
