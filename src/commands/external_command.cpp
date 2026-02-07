#include "commands/external_command.h"
#include "process_manager.h"
#include "environment_manager.h"

ExternalCommand::ExternalCommand(const std::string& program, const std::vector<std::string>& args)
    : program_(program), args_(args) {}

int ExternalCommand::execute(std::istream& input, std::ostream& output, std::ostream& error) {
    ProcessManager manager;
    auto env = EnvironmentManager::getInstance().getAllVariables();
    return manager.executeExternal(program_, args_, env, input, output, error);
}
