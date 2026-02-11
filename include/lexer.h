#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

/**
 * @brief Type of lexical token
 */
enum class TokenType { WORD, QUOTED_SINGLE, QUOTED_DOUBLE, ASSIGNMENT };

/**
 * @brief Represents a single lexical token
 */
struct Token {
    TokenType type;
    std::string value;

    /**
     * @brief Constructs token with type and value
     * @param t Token type
     * @param v Token value
     */
    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};

/**
 * @brief Tokenizes input strings into sequence of tokens
 */
class Lexer {
public:
    /**
     * @brief Tokenizes input string
     * @param input String to tokenize
     * @return Vector of tokens
     */
    std::vector<Token> tokenize(const std::string& input);

private:
    void skipWhitespace();
    Token readQuotedToken(char quote);
    Token readWordToken();

    std::string input_;
    size_t pos_;
};

#endif
