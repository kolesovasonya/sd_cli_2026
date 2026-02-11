#include "command_executor.h"

#include "commands/abstract_command.h"

int CommandExecutor::execute(AbstractCommand* command, std::istream& input,
                             std::ostream& output, std::ostream& error) {
    if (!command) {
        return 0;
    }

    return command->execute(input, output, error);
}
