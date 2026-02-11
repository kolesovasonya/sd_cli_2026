#ifndef BUILTIN_COMMAND_H
#define BUILTIN_COMMAND_H

#include "abstract_command.h"

/**
 * @brief Base class for built-in commands
 */
class BuiltinCommand : public AbstractCommand {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~BuiltinCommand() = default;
};

#endif
