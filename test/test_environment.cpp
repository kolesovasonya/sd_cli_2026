#include <gtest/gtest.h>

#include <sstream>

#include "command_executor.h"
#include "commands/abstract_command.h"
#include "environment_manager.h"
#include "lexer.h"
#include "parser.h"

TEST(EnvironmentTest, SetAndGetVariable) {
    EnvironmentManager& env = EnvironmentManager::getInstance();

    env.setVariable("TEST_VAR", "test_value");
    EXPECT_EQ(env.getVariable("TEST_VAR"), "test_value");
}

TEST(EnvironmentTest, VariableSubstitution) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("NAME", "World");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo Hello $NAME");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "Hello World\n");
}

TEST(EnvironmentTest, AssignmentViaParser) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;

    auto tokens = lexer.tokenize("MY_VAR=my_value");
    auto command = parser.parse(tokens);

    EXPECT_EQ(command, nullptr);
    EXPECT_EQ(env.getVariable("MY_VAR"), "my_value");
}

TEST(EnvironmentTest, NonExistentVariable) {
    EnvironmentManager& env = EnvironmentManager::getInstance();

    std::string value = env.getVariable("NON_EXISTENT_VAR_12345");
    EXPECT_EQ(value, "");
}

TEST(EnvironmentTest, MultipleVariableSubstitutions) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("FIRST", "Hello");
    env.setVariable("SECOND", "World");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo $FIRST $SECOND");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "Hello World\n");
}

TEST(EnvironmentTest, ExitCodeVariable) {
    EnvironmentManager& env = EnvironmentManager::getInstance();

    env.setVariable("?", "0");
    EXPECT_EQ(env.getVariable("?"), "0");

    env.setVariable("?", "42");
    EXPECT_EQ(env.getVariable("?"), "42");

    env.setVariable("?", "1");
    EXPECT_EQ(env.getVariable("?"), "1");
}

TEST(EnvironmentTest, ExitCodeSubstitution) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("?", "123");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo $?");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "123\n");
}

TEST(EnvironmentTest, ExitCodeInDoubleQuotes) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("?", "42");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo \"Exit code: $?\"");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "Exit code: 42\n");
}

TEST(EnvironmentTest, ExitCodeInSingleQuotes) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("?", "99");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo '$?'");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "$?\n");
}

TEST(EnvironmentTest, AdjacentVariableSubstitution) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("a", "ex");
    env.setVariable("b", "it");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo $a$b");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "exit\n");
}

TEST(EnvironmentTest, VariableWithLiteralPrefix) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("EXT", "cpp");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo file.$EXT");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "file.cpp\n");
}

TEST(EnvironmentTest, VariableWithLiteralSuffix) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("BASE", "hello");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo $BASE_world");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    executor.execute(command.get(), input, output, error);

    EXPECT_EQ(output.str(), "\n");
}

TEST(EnvironmentTest, ThreeAdjacentVariables) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("x", "foo");
    env.setVariable("y", "bar");
    env.setVariable("z", "baz");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo $x$y$z");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "foobarbaz\n");
}

TEST(EnvironmentTest, AdjacentVariablesInDoubleQuotes) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("p", "hello");
    env.setVariable("q", "world");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo \"$p$q\"");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "helloworld\n");
}
