#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "command_executor.h"
#include "commands/abstract_command.h"
#include "environment_manager.h"
#include "lexer.h"
#include "parser.h"

TEST(PipelineTest, ParserDetectsPipeline) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;

    auto tokens = lexer.tokenize("echo hello | cat");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);
}

TEST(PipelineTest, EchoToCat) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo hello | cat");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);
    EXPECT_EQ(ret, 0);
}

TEST(PipelineTest, EchoToWc) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo hello world | wc");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);
    EXPECT_EQ(ret, 0);
}

TEST(PipelineTest, MultipleCommands) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo test | cat | cat");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
}

TEST(PipelineTest, PipelineInvalidStart) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;

    std::stringstream captured_cerr;
    std::streambuf* old_cerr = std::cerr.rdbuf(captured_cerr.rdbuf());

    auto tokens = lexer.tokenize("| cat");
    auto command = parser.parse(tokens);

    std::cerr.rdbuf(old_cerr);

    EXPECT_EQ(command, nullptr);
    EXPECT_TRUE(captured_cerr.str().find("cannot start with") !=
                std::string::npos);
}

TEST(PipelineTest, PipelineInvalidEnd) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;

    std::stringstream captured_cerr;
    std::streambuf* old_cerr = std::cerr.rdbuf(captured_cerr.rdbuf());

    auto tokens = lexer.tokenize("cat |");
    auto command = parser.parse(tokens);

    std::cerr.rdbuf(old_cerr);

    EXPECT_EQ(command, nullptr);
    std::string error_msg = captured_cerr.str();
    EXPECT_TRUE(error_msg.find("cannot end with") != std::string::npos ||
                error_msg.find("empty command") != std::string::npos);
}

TEST(PipelineTest, PipelineEmptyCommand) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;

    std::stringstream captured_cerr;
    std::streambuf* old_cerr = std::cerr.rdbuf(captured_cerr.rdbuf());

    auto tokens = lexer.tokenize("cat | | wc");
    auto command = parser.parse(tokens);

    std::cerr.rdbuf(old_cerr);

    EXPECT_EQ(command, nullptr);
    EXPECT_TRUE(captured_cerr.str().find("empty command") !=
                std::string::npos);
}

TEST(PipelineTest, PipeWithVariableSubstitution) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    env.setVariable("WORD", "test");

    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo $WORD | cat");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
}

TEST(PipelineTest, PipeWithQuotes) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("echo 'hello world' | cat");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
}

TEST(PipelineTest, ExitCodeOfLastCommand) {
    EnvironmentManager& env = EnvironmentManager::getInstance();
    Parser parser(env);
    Lexer lexer;
    CommandExecutor executor;

    auto tokens = lexer.tokenize("cat nonexistent_file_xyz.txt | cat");
    auto command = parser.parse(tokens);

    ASSERT_NE(command, nullptr);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = executor.execute(command.get(), input, output, error);

    EXPECT_EQ(ret, 0);
}
