#include "lexer.h"

std::vector<Token> Lexer::tokenize(const std::string& input) {
    input_ = input;
    pos_ = 0;
    std::vector<Token> tokens;

    while (pos_ < input_.length()) {
        size_t posBefore = pos_;
        skipWhitespace();
        if (pos_ >= input_.length()) break;

        bool hadSpace = tokens.empty() || (pos_ > posBefore);

        char ch = input_[pos_];

        Token tok = (ch == '\'' || ch == '"') ? readQuotedToken(ch)
                  : (ch == '|')               ? readPipeToken()
                                              : readWordToken();
        tok.space_before = hadSpace;
        tokens.push_back(tok);
    }

    return tokens;
}

void Lexer::skipWhitespace() {
    while (pos_ < input_.length() && std::isspace(input_[pos_])) {
        pos_++;
    }
}

Token Lexer::readQuotedToken(char quote) {
    pos_++;
    std::string value;

    while (pos_ < input_.length() && input_[pos_] != quote) {
        value += input_[pos_++];
    }

    if (pos_ < input_.length()) {
        pos_++;
    }

    TokenType type =
        (quote == '\'') ? TokenType::QUOTED_SINGLE : TokenType::QUOTED_DOUBLE;
    return Token(type, value);
}

Token Lexer::readWordToken() {
    std::string value;

    while (pos_ < input_.length() && !std::isspace(input_[pos_]) &&
           input_[pos_] != '\'' && input_[pos_] != '"' && input_[pos_] != '|') {
        value += input_[pos_++];
    }

    if (value.find('=') != std::string::npos) {
        return Token(TokenType::ASSIGNMENT, value);
    }

    return Token(TokenType::WORD, value);
}

Token Lexer::readPipeToken() {
    pos_++;
    return Token(TokenType::PIPE, "|");
}
