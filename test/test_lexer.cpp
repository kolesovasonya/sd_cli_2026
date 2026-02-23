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

TEST(LexerTest, SimplePipe) {
    Lexer lexer;
    auto tokens = lexer.tokenize("cat file.txt | wc");

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].value, "cat");
    EXPECT_EQ(tokens[0].type, TokenType::WORD);
    EXPECT_EQ(tokens[1].value, "file.txt");
    EXPECT_EQ(tokens[1].type, TokenType::WORD);
    EXPECT_EQ(tokens[2].value, "|");
    EXPECT_EQ(tokens[2].type, TokenType::PIPE);
    EXPECT_EQ(tokens[3].value, "wc");
    EXPECT_EQ(tokens[3].type, TokenType::WORD);
}

TEST(LexerTest, MultiplePipes) {
    Lexer lexer;
    auto tokens = lexer.tokenize("cat | grep test | wc");

    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].value, "cat");
    EXPECT_EQ(tokens[1].value, "|");
    EXPECT_EQ(tokens[1].type, TokenType::PIPE);
    EXPECT_EQ(tokens[2].value, "grep");
    EXPECT_EQ(tokens[3].value, "test");
    EXPECT_EQ(tokens[4].value, "|");
    EXPECT_EQ(tokens[4].type, TokenType::PIPE);
    EXPECT_EQ(tokens[5].value, "wc");
}

TEST(LexerTest, PipeInQuotesNotOperator) {
    Lexer lexer;
    auto tokens = lexer.tokenize("echo 'hello | world'");

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].value, "echo");
    EXPECT_EQ(tokens[1].value, "hello | world");
    EXPECT_EQ(tokens[1].type, TokenType::QUOTED_SINGLE);
}

TEST(LexerTest, PipeInDoubleQuotesNotOperator) {
    Lexer lexer;
    auto tokens = lexer.tokenize("echo \"test | data\"");

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].value, "echo");
    EXPECT_EQ(tokens[1].value, "test | data");
    EXPECT_EQ(tokens[1].type, TokenType::QUOTED_DOUBLE);
}

TEST(LexerTest, PipeWithoutSpaces) {
    Lexer lexer;
    auto tokens = lexer.tokenize("cat|wc");

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].value, "cat");
    EXPECT_EQ(tokens[1].value, "|");
    EXPECT_EQ(tokens[1].type, TokenType::PIPE);
    EXPECT_EQ(tokens[2].value, "wc");
}
