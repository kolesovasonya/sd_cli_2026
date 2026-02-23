#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

/**
 * @brief Type of lexical token
 */
enum class TokenType { WORD, QUOTED_SINGLE, QUOTED_DOUBLE, ASSIGNMENT, PIPE };

/**
 * @brief Represents a single lexical token
 */
struct Token {
    TokenType type;
    std::string value;
    bool space_before;

    /**
     * @brief Constructs token with type and value
     * @param t Token type
     * @param v Token value
     * @param sb Whether whitespace preceded this token
     */
    Token(TokenType t, const std::string& v, bool sb = true)
        : type(t), value(v), space_before(sb) {}
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
    Token readPipeToken();

    std::string input_;
    size_t pos_;
};

#endif
