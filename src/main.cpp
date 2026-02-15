#include <iostream>

#include "command_executor.h"
#include "commands/exit_command.h"
#include "environment_manager.h"
#include "input_processor.h"
#include "lexer.h"
#include "parser.h"

int main() {
    EnvironmentManager& envManager = EnvironmentManager::getInstance();
    InputProcessor inputProcessor(std::cin);
    Lexer lexer;
    Parser parser(envManager);
    CommandExecutor executor;

    std::string line;
    int lastExitCode = 0;

    // Initialize $? to 0
    envManager.setVariable("?", "0");

    while (true) {
        std::cout << "> ";
        std::cout.flush();

        if (!inputProcessor.readLine(line)) {
            break;
        }

        if (line.empty()) {
            continue;
        }

        auto tokens = lexer.tokenize(line);
        auto command = parser.parse(tokens);

        if (command) {
            lastExitCode = executor.execute(command.get(), std::cin, std::cout, std::cerr);
            
            // Update $? with the last exit code
            envManager.setVariable("?", std::to_string(lastExitCode));

            if (ExitCommand::shouldExit()) {
                break;
            }
        }
    }

    return 0;
}
