#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <memory>
#include <iostream>

class AbstractCommand;

/**
 * @brief Executes commands with specified I/O streams
 */
class CommandExecutor {
public:
    /**
     * @brief Executes command
     * @param command Command to execute
     * @param input Input stream
     * @param output Output stream
     * @param error Error stream
     * @return Command exit code
     */
    int execute(
        AbstractCommand* command,
        std::istream& input = std::cin,
        std::ostream& output = std::cout,
        std::ostream& error = std::cerr
    );
};

#endif
