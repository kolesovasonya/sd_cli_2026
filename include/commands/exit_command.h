#ifndef EXIT_COMMAND_H
#define EXIT_COMMAND_H

#include "builtin_command.h"

/**
 * @brief Built-in exit command - terminates interpreter
 */
class ExitCommand : public BuiltinCommand {
public:
    /**
     * @brief Constructs exit command
     */
    ExitCommand() = default;

    /**
     * @brief Executes exit command - sets exit flag
     * @param input Input stream
     * @param output Output stream
     * @param error Error stream
     * @return Exit code (always 0)
     */
    int execute(std::istream& input, std::ostream& output,
                std::ostream& error) override;

    /**
     * @brief Checks if exit was requested
     * @return true if exit command was executed
     */
    static bool shouldExit();

private:
    static bool exitFlag_;
};

#endif
