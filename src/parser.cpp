#include "parser.h"
#include "environment_manager.h"
#include "command_factory.h"
#include "commands/abstract_command.h"

Parser::Parser(EnvironmentManager& envManager) : envManager_(envManager) {}

std::unique_ptr<AbstractCommand> Parser::parse(const std::vector<Token>& tokens) {
    if (tokens.empty()) {
        return nullptr;
    }
    
    if (isAssignment(tokens)) {
        handleAssignment(tokens);
        return nullptr;
    }
    
    std::vector<std::string> args;
    
    for (const auto& token : tokens) {
        args.push_back(resolveValue(token));
    }
    
    if (args.empty()) {
        return nullptr;
    }
    
    std::string commandName = args[0];
    args.erase(args.begin());
    
    CommandFactory factory;
    return factory.createCommand(commandName, args);
}

bool Parser::isAssignment(const std::vector<Token>& tokens) {
    return tokens.size() == 1 && tokens[0].type == TokenType::ASSIGNMENT;
}

void Parser::handleAssignment(const std::vector<Token>& tokens) {
    std::string assignment = tokens[0].value;
    size_t eqPos = assignment.find('=');
    
    if (eqPos != std::string::npos) {
        std::string name = assignment.substr(0, eqPos);
        std::string value = assignment.substr(eqPos + 1);
        envManager_.setVariable(name, value);
    }
}

std::string Parser::resolveValue(const Token& token) {
    if (token.type == TokenType::QUOTED_SINGLE) {
        return token.value;
    }
    
    if (token.type == TokenType::QUOTED_DOUBLE) {
        std::string result = token.value;
        size_t pos = 0;
        while ((pos = result.find('$', pos)) != std::string::npos) {
            size_t end = pos + 1;
            while (end < result.length() && 
                   (std::isalnum(result[end]) || result[end] == '_')) {
                end++;
            }
            
            if (end > pos + 1) {
                std::string varName = result.substr(pos + 1, end - pos - 1);
                std::string varValue = envManager_.getVariable(varName);
                result.replace(pos, end - pos, varValue);
                pos += varValue.length();
            } else {
                pos++;
            }
        }
        return result;
    }
    
    if (token.type == TokenType::WORD && !token.value.empty() && token.value[0] == '$') {
        std::string varName = token.value.substr(1);
        return envManager_.getVariable(varName);
    }
    
    return token.value;
}
