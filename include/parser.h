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

    EnvironmentManager& envManager_;
};

#endif
