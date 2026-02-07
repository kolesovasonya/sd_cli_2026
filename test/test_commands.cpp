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
