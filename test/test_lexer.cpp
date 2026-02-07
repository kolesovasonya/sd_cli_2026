#include <gtest/gtest.h>
#include "lexer.h"

TEST(LexerTest, SimpleTokenization) {
    Lexer lexer;
    auto tokens = lexer.tokenize("echo hello world");
    
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].value, "echo");
    EXPECT_EQ(tokens[1].value, "hello");
    EXPECT_EQ(tokens[2].value, "world");
}

TEST(LexerTest, SingleQuotes) {
    Lexer lexer;
    auto tokens = lexer.tokenize("echo 'hello world'");
    
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].value, "echo");
    EXPECT_EQ(tokens[1].value, "hello world");
    EXPECT_EQ(tokens[1].type, TokenType::QUOTED_SINGLE);
}

TEST(LexerTest, DoubleQuotes) {
    Lexer lexer;
    auto tokens = lexer.tokenize("echo \"hello world\"");
    
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].value, "echo");
    EXPECT_EQ(tokens[1].value, "hello world");
    EXPECT_EQ(tokens[1].type, TokenType::QUOTED_DOUBLE);
}

TEST(LexerTest, Assignment) {
    Lexer lexer;
    auto tokens = lexer.tokenize("VAR=value");
    
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].value, "VAR=value");
    EXPECT_EQ(tokens[0].type, TokenType::ASSIGNMENT);
}
