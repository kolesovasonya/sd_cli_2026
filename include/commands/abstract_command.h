#ifndef ABSTRACT_COMMAND_H
#define ABSTRACT_COMMAND_H

#include <iostream>

/**
 * @brief Abstract base class for all commands
 */
class AbstractCommand {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~AbstractCommand() = default;
    
    /**
     * @brief Executes command
     * @param input Input stream
     * @param output Output stream
     * @param error Error stream
     * @return Exit code (0 for success)
     */
    virtual int execute(
        std::istream& input,
        std::ostream& output,
        std::ostream& error
    ) = 0;
};

#endif
