#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <vector>

#include "lexer.h"

class AbstractCommand;
class EnvironmentManager;

/**
 * @brief Parses tokens into executable command objects
 */
class Parser {
public:
    /**
     * @brief Constructs parser with environment manager
     * @param envManager Environment manager for variable resolution
     */
    explicit Parser(EnvironmentManager& envManager);

    /**
     * @brief Parses tokens into command object
     * @param tokens Vector of tokens to parse
     * @return Unique pointer to command, or nullptr if no command
     */
    std::unique_ptr<AbstractCommand> parse(const std::vector<Token>& tokens);

private:
    bool isAssignment(const std::vector<Token>& tokens);
    void handleAssignment(const std::vector<Token>& tokens);
    std::string resolveValue(const Token& token);
    std::string expandVariables(const std::string& input);

    /**
     * @brief Splits tokens by PIPE operator
     * @param tokens Vector of tokens to split
     * @return Vector of token groups (one per command in pipeline)
     */
    std::vector<std::vector<Token>> splitByPipe(
        const std::vector<Token>& tokens);

    /**
     * @brief Validates pipeline structure
     * @param commandTokens Vector of command token groups
     * @param errorMessage Output parameter for error message
     * @return true if pipeline is valid
     */
    bool validatePipeline(const std::vector<std::vector<Token>>& commandTokens,
                          std::string& errorMessage);

    /**
     * @brief Parses a single command (no pipes)
     * @param tokens Tokens for a single command
     * @return Unique pointer to command, or nullptr if invalid
     */
    std::unique_ptr<AbstractCommand> parseSingleCommand(
        const std::vector<Token>& tokens);

    EnvironmentManager& envManager_;
};

#endif
