#include <gtest/gtest.h>
#include "parser.h"
#include "environment_manager.h"
#include "lexer.h"
#include "commands/abstract_command.h"

TEST(ParserTest, SimpleCommand) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    
    auto tokens = lexer.tokenize("echo hello");
    auto command = parser.parse(tokens);
    
    ASSERT_NE(command, nullptr);
}

TEST(ParserTest, Assignment) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    
    auto tokens = lexer.tokenize("TEST=123");
    auto command = parser.parse(tokens);
    
    ASSERT_EQ(command, nullptr);
    EXPECT_EQ(env.getVariable("TEST"), "123");
}

TEST(ParserTest, VariableSubstitution) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("NAME", "World");
    
    Parser parser(env);
    Lexer lexer;
    
    auto tokens = lexer.tokenize("echo $NAME");
    auto command = parser.parse(tokens);
    
    ASSERT_NE(command, nullptr);
}
