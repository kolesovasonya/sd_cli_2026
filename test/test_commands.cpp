#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "commands/cat_command.h"
#include "commands/echo_command.h"
#include "commands/exit_command.h"
#include "commands/pwd_command.h"
#include "commands/wc_command.h"

TEST(CommandsTest, EchoCommand) {
    std::vector<std::string> args = {"hello", "world"};
    EchoCommand cmd(args);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = cmd.execute(input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "hello world\n");
}

TEST(CommandsTest, PwdCommand) {
    PwdCommand cmd;

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = cmd.execute(input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_FALSE(output.str().empty());
}

TEST(CommandsTest, EchoEmptyArgs) {
    std::vector<std::string> args;
    EchoCommand cmd(args);

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = cmd.execute(input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "\n");
}

TEST(CommandsTest, CatNonExistentFile) {
    CatCommand cmd("nonexistent_file_12345.txt");

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = cmd.execute(input, output, error);

    EXPECT_EQ(ret, 1);
    EXPECT_FALSE(error.str().empty());
}

TEST(CommandsTest, WcNonExistentFile) {
    WcCommand cmd("nonexistent_file_12345.txt");

    std::ostringstream output;
    std::ostringstream error;
    std::istringstream input;

    int ret = cmd.execute(input, output, error);

    EXPECT_EQ(ret, 1);
    EXPECT_FALSE(error.str().empty());
}

TEST(CommandsTest, CatFromStdin) {
    CatCommand cmd("");

    std::istringstream input("hello\nworld\n");
    std::ostringstream output;
    std::ostringstream error;

    int ret = cmd.execute(input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "hello\nworld\n");
}

TEST(CommandsTest, CatFromStdinDash) {
    CatCommand cmd("-");

    std::istringstream input("test\ndata\n");
    std::ostringstream output;
    std::ostringstream error;

    int ret = cmd.execute(input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "test\ndata\n");
}

TEST(CommandsTest, WcFromStdin) {
    WcCommand cmd("");

    std::istringstream input("line1\nline2 word2\nline3 word3 word4\n");
    std::ostringstream output;
    std::ostringstream error;

    int ret = cmd.execute(input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "3 6 36\n");
}

TEST(CommandsTest, WcFromStdinDash) {
    WcCommand cmd("-");

    std::istringstream input("hello world\n");
    std::ostringstream output;
    std::ostringstream error;

    int ret = cmd.execute(input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "1 2 12\n");
}

TEST(CommandsTest, WcEmptyStdin) {
    WcCommand cmd("");

    std::istringstream input("");
    std::ostringstream output;
    std::ostringstream error;

    int ret = cmd.execute(input, output, error);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(output.str(), "0 0 0\n");
}
