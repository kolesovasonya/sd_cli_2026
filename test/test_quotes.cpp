#include <gtest/gtest.h>

#include <sstream>

#include "command_executor.h"
#include "command_factory.h"
#include "commands/abstract_command.h"
#include "environment_manager.h"
#include "lexer.h"
#include "parser.h"

TEST(QuotesTest, SingleQuotesPreserveSpaces) {
    Lexer lexer;
    auto tokens = lexer.tokenize("echo 'hello world'");

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].value, "echo");
    EXPECT_EQ(tokens[1].value, "hello world");
    EXPECT_EQ(tokens[1].type, TokenType::QUOTED_SINGLE);
}

TEST(QuotesTest, DoubleQuotesPreserveSpaces) {
    Lexer lexer;
    auto tokens = lexer.tokenize("echo \"hello world\"");

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].value, "echo");
    EXPECT_EQ(tokens[1].value, "hello world");
    EXPECT_EQ(tokens[1].type, TokenType::QUOTED_DOUBLE);
}

TEST(QuotesTest, SingleQuotedStringAsOneArgument) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo 'hello world'");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "hello world\n");
}

TEST(QuotesTest, DoubleQuotedStringAsOneArgument) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo \"hello world\"");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "hello world\n");
}

TEST(QuotesTest, MultipleQuotedArguments) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo 'hello world' \"foo bar\"");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "hello world foo bar\n");
}

TEST(QuotesTest, MixedQuotedAndUnquotedArguments) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo hello 'my world' test");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "hello my world test\n");
}

TEST(QuotesTest, EmptyQuotedString) {
    Lexer lexer;
    auto tokens = lexer.tokenize("echo ''");

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].value, "echo");
    EXPECT_EQ(tokens[1].value, "");
    EXPECT_EQ(tokens[1].type, TokenType::QUOTED_SINGLE);
}

TEST(QuotesTest, SingleQuotesNoVariableSubstitution) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("VAR", "substituted");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo '$VAR'");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "$VAR\n");
}

TEST(QuotesTest, DoubleQuotesWithVariableSubstitution) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("VAR", "substituted");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo \"$VAR\"");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "substituted\n");
}
