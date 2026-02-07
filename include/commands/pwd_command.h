#ifndef PWD_COMMAND_H
#define PWD_COMMAND_H

#include "builtin_command.h"

/**
 * @brief Built-in pwd command - prints current working directory
 */
class PwdCommand : public BuiltinCommand {
public:
    /**
     * @brief Constructs pwd command
     */
    PwdCommand() = default;

    /**
     * @brief Executes pwd command
     * @param input Input stream
     * @param output Output stream
     * @param error Error stream
     * @return Exit code (0 for success, 1 for error)
     */
    int execute(std::istream& input, std::ostream& output,
                std::ostream& error) override;
};

#endif
