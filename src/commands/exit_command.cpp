#include "commands/exit_command.h"

bool ExitCommand::exitFlag_ = false;

int ExitCommand::execute(std::istream& input, std::ostream& output,
                         std::ostream& error) {
    exitFlag_ = true;
    return 0;
}

bool ExitCommand::shouldExit() { return exitFlag_; }
