#include "input_processor.h"
#include "lexer.h"
#include "parser.h"
#include "environment_manager.h"
#include "command_executor.h"
#include "commands/exit_command.h"
#include <iostream>

int main() {
    EnvironmentManager& envManager = EnvironmentManager::getInstance();
    InputProcessor inputProcessor(std::cin);
    Lexer lexer;
    Parser parser(envManager);
    CommandExecutor executor;
    
    std::string line;
    
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
            executor.execute(command.get(), std::cin, std::cout, std::cerr);
            
            if (ExitCommand::shouldExit()) {
                break;
            }
        }
    }
    
    return 0;
}
