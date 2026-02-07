#include "lexer.h"

std::vector<Token> Lexer::tokenize(const std::string& input) {
    input_ = input;
    pos_ = 0;
    std::vector<Token> tokens;
    
    while (pos_ < input_.length()) {
        skipWhitespace();
        if (pos_ >= input_.length()) break;
        
        char ch = input_[pos_];
        
        if (ch == '\'' || ch == '"') {
            tokens.push_back(readQuotedToken(ch));
        } else {
            tokens.push_back(readWordToken());
        }
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
    
    TokenType type = (quote == '\'') ? TokenType::QUOTED_SINGLE : TokenType::QUOTED_DOUBLE;
    return Token(type, value);
}

Token Lexer::readWordToken() {
    std::string value;
    
    while (pos_ < input_.length() && !std::isspace(input_[pos_]) && 
           input_[pos_] != '\'' && input_[pos_] != '"') {
        value += input_[pos_++];
    }
    
    if (value.find('=') != std::string::npos) {
        return Token(TokenType::ASSIGNMENT, value);
    }
    
    return Token(TokenType::WORD, value);
}
