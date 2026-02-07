#include "commands/echo_command.h"

EchoCommand::EchoCommand(const std::vector<std::string>& args) : args_(args) {}

int EchoCommand::execute(std::istream& input, std::ostream& output,
                         std::ostream& error) {
    for (size_t i = 0; i < args_.size(); ++i) {
        output << args_[i];
        if (i < args_.size() - 1) {
            output << " ";
        }
    }
    output << std::endl;
    return 0;
}
