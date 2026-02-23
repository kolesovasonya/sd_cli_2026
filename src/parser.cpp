#include "parser.h"

#include <iostream>

#include "command_factory.h"
#include "commands/abstract_command.h"
#include "commands/pipeline_command.h"
#include "environment_manager.h"

Parser::Parser(EnvironmentManager& envManager) : envManager_(envManager) {}

std::unique_ptr<AbstractCommand> Parser::parse(
    const std::vector<Token>& tokens) {
    if (tokens.empty()) {
        return nullptr;
    }

    if (isAssignment(tokens)) {
        handleAssignment(tokens);
        return nullptr;
    }

    auto commandTokens = splitByPipe(tokens);

    std::string errorMessage;
    if (!validatePipeline(commandTokens, errorMessage)) {
        std::cerr << "Syntax error: " << errorMessage << std::endl;
        return nullptr;
    }

    if (commandTokens.size() == 1) {
        return parseSingleCommand(commandTokens[0]);
    }

    std::vector<std::unique_ptr<AbstractCommand>> commands;
    for (const auto& cmdTokens : commandTokens) {
        auto cmd = parseSingleCommand(cmdTokens);
        if (!cmd) {
            return nullptr;
        }
        commands.push_back(std::move(cmd));
    }

    return std::make_unique<PipelineCommand>(std::move(commands));
}

std::unique_ptr<AbstractCommand> Parser::parseSingleCommand(
    const std::vector<Token>& tokens) {
    if (tokens.empty()) {
        return nullptr;
    }

    std::vector<std::string> args;
    std::string current;

    for (size_t i = 0; i < tokens.size(); i++) {
        if (i > 0 && tokens[i].space_before) {
            args.push_back(current);
            current.clear();
        }
        current += resolveValue(tokens[i]);
    }
    args.push_back(current);

    if (args.empty()) {
        return nullptr;
    }

    std::string commandName = args[0];
    args.erase(args.begin());

    CommandFactory factory;
    return factory.createCommand(commandName, args);
}

std::vector<std::vector<Token>> Parser::splitByPipe(
    const std::vector<Token>& tokens) {
    std::vector<std::vector<Token>> result;
    std::vector<Token> currentCommand;

    for (const auto& token : tokens) {
        if (token.type == TokenType::PIPE) {
            result.push_back(currentCommand);
            currentCommand.clear();
        } else {
            currentCommand.push_back(token);
        }
    }

    result.push_back(currentCommand);

    return result;
}

bool Parser::validatePipeline(
    const std::vector<std::vector<Token>>& commandTokens,
    std::string& errorMessage) {
    if (commandTokens.empty()) {
        errorMessage = "empty pipeline";
        return false;
    }

    for (size_t i = 0; i < commandTokens.size(); i++) {
        if (commandTokens[i].empty()) {
            if (i == 0) {
                errorMessage = "pipeline cannot start with '|'";
            } else if (i == commandTokens.size() - 1) {
                errorMessage = "pipeline cannot end with '|'";
            } else {
                errorMessage = "empty command in pipeline";
            }
            return false;
        }
    }

    return true;
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

std::string Parser::expandVariables(const std::string& input) {
    std::string result = input;
    size_t pos = 0;
    while ((pos = result.find('$', pos)) != std::string::npos) {
        size_t end = pos + 1;

        if (end < result.length() && result[end] == '?') {
            end++;
            std::string varValue = envManager_.getVariable("?");
            result.replace(pos, end - pos, varValue);
            pos += varValue.length();
            continue;
        }

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

std::string Parser::resolveValue(const Token& token) {
    if (token.type == TokenType::QUOTED_SINGLE) {
        return token.value;
    }

    if (token.type == TokenType::QUOTED_DOUBLE ||
        token.type == TokenType::WORD) {
        return expandVariables(token.value);
    }

    return token.value;
}
