#include "command_factory.h"

#include "commands/abstract_command.h"
#include "commands/cat_command.h"
#include "commands/echo_command.h"
#include "commands/exit_command.h"
#include "commands/external_command.h"
#include "commands/grep_command.h"
#include "commands/pwd_command.h"
#include "commands/wc_command.h"

std::unique_ptr<AbstractCommand> CommandFactory::createCommand(
    const std::string& name, const std::vector<std::string>& args) {
    if (name == "cat") {
        std::string filename = args.empty() ? "" : args[0];
        return std::make_unique<CatCommand>(filename);
    } else if (name == "wc") {
        std::string filename = args.empty() ? "" : args[0];
        return std::make_unique<WcCommand>(filename);
    } else if (name == "echo") {
        return std::make_unique<EchoCommand>(args);
    } else if (name == "pwd") {
        return std::make_unique<PwdCommand>();
    } else if (name == "exit") {
        return std::make_unique<ExitCommand>();
    } else if (name == "grep") {
        return std::make_unique<GrepCommand>(args);
    } else {
        return std::make_unique<ExternalCommand>(name, args);
    }
}

bool CommandFactory::isBuiltinCommand(const std::string& name) const {
    return name == "cat" || name == "wc" || name == "echo" || name == "pwd" ||
           name == "exit" || name == "grep";
}
