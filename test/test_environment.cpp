#include <gtest/gtest.h>
#include "environment_manager.h"
#include "parser.h"
#include "lexer.h"
#include "command_executor.h"
#include "commands/abstract_command.h"
#include <sstream>

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
