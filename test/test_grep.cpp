#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "commands/grep_command.h"

// ---------------------------------------------------------------------------
// Helper: run GrepCommand with given args and stdin content, return exit code
// ---------------------------------------------------------------------------
static int runGrep(const std::vector<std::string>& args,
                   const std::string& stdin_data, std::string& out,
                   std::string& err) {
    GrepCommand cmd(args);
    std::istringstream input(stdin_data);
    std::ostringstream output;
    std::ostringstream error;
    int code = cmd.execute(input, output, error);
    out = output.str();
    err = error.str();
    return code;
}

// ---------------------------------------------------------------------------
// Basic search
// ---------------------------------------------------------------------------

TEST(GrepTest, BasicSearchMatch) {
    std::string out, err;
    int code = runGrep({"hello"}, "hello world\nfoo bar\nhello again\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "hello world\nhello again\n");
}

TEST(GrepTest, BasicSearchNoMatch) {
    std::string out, err;
    int code = runGrep({"xyz"}, "hello world\nfoo bar\n", out, err);

    EXPECT_EQ(code, 1);
    EXPECT_TRUE(out.empty());
}

// ---------------------------------------------------------------------------
// Regex support
// ---------------------------------------------------------------------------

TEST(GrepTest, RegexPattern) {
    std::string out, err;
    int code = runGrep({"^foo"}, "foobar\nbarfoo\nfoo\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "foobar\nfoo\n");
}

TEST(GrepTest, RegexEndAnchor) {
    std::string out, err;
    int code = runGrep({"foo$"}, "foobar\nbarfoo\nfoo\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "barfoo\nfoo\n");
}

TEST(GrepTest, InvalidRegex) {
    std::string out, err;
    int code = runGrep({"["}, "hello\n", out, err);

    EXPECT_EQ(code, 1);
    EXPECT_FALSE(err.empty());
}

// ---------------------------------------------------------------------------
// -i flag: case-insensitive search
// ---------------------------------------------------------------------------

TEST(GrepTest, CaseInsensitiveMatch) {
    std::string out, err;
    int code = runGrep({"-i", "HELLO"}, "Hello World\nfoo\nhELLO\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "Hello World\nhELLO\n");
}

TEST(GrepTest, CaseInsensitiveNoMatch) {
    std::string out, err;
    int code = runGrep({"-i", "xyz"}, "Hello\nWorld\n", out, err);

    EXPECT_EQ(code, 1);
}

// ---------------------------------------------------------------------------
// -w flag: whole-word search
// ---------------------------------------------------------------------------

TEST(GrepTest, WholeWordMatchesCompleteWord) {
    std::string out, err;
    int code = runGrep({"-w", "foo"}, "foo bar\nfoobar\nprefoo\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "foo bar\n");
}

TEST(GrepTest, WholeWordNoMatch) {
    std::string out, err;
    int code = runGrep({"-w", "foo"}, "foobar\nbarfoo\nprefoopost\n", out, err);

    EXPECT_EQ(code, 1);
    EXPECT_TRUE(out.empty());
}

TEST(GrepTest, WholeWordCombinedWithCaseInsensitive) {
    std::string out, err;
    int code = runGrep({"-w", "-i", "foo"}, "FOO bar\nFOObar\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "FOO bar\n");
}

// ---------------------------------------------------------------------------
// -A flag: print N lines after each match
// ---------------------------------------------------------------------------

TEST(GrepTest, AfterContextZero) {
    std::string out, err;
    int code = runGrep({"-A", "0", "match"}, "match\nno\nno\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "match\n");
}

TEST(GrepTest, AfterContextPrintsNLines) {
    std::string out, err;
    int code = runGrep({"-A", "2", "match"},
                       "before\nmatch\nline1\nline2\nafter\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "match\nline1\nline2\n");
}

TEST(GrepTest, AfterContextDoesNotExceedEndOfFile) {
    std::string out, err;
    int code = runGrep({"-A", "3", "match"}, "foo\nmatch\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "match\n");
}

TEST(GrepTest, AfterContextOverlappingRangesNoDuplicate) {
    std::string out, err;
    int code = runGrep({"-A", "2", "match"},
                       "match\nctx1\nmatch\nctx2\nctx3\nno\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "match\nctx1\nmatch\nctx2\nctx3\n");
}

TEST(GrepTest, AfterContextSeparatorBetweenGroups) {
    std::string out, err;
    int code = runGrep({"-A", "1", "match"},
                       "match\nctx\ngap1\ngap2\nmatch\nctx\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "match\nctx\n--\nmatch\nctx\n");
}

TEST(GrepTest, AfterContextNoSeparatorWhenContextZero) {
    std::string out, err;
    int code = runGrep({"-A", "0", "match"},
                       "match\ngap\nmatch\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "match\nmatch\n");
    EXPECT_EQ(out.find("--"), std::string::npos);
}

TEST(GrepTest, AfterContextNegativeValue) {
    std::string out, err;
    int code = runGrep({"-A", "-1", "match"}, "match\n", out, err);

    EXPECT_EQ(code, 1);
    EXPECT_FALSE(err.empty());
}

// ---------------------------------------------------------------------------
// Reading from stdin vs file
// ---------------------------------------------------------------------------

TEST(GrepTest, ReadsFromStdinWhenNoFile) {
    std::string out, err;
    int code = runGrep({"hello"}, "hello\nworld\n", out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out, "hello\n");
}

TEST(GrepTest, NonExistentFileReturnsError) {
    GrepCommand cmd({"pattern", "/no/such/file_xyz_12345.txt"});
    std::istringstream input;
    std::ostringstream output;
    std::ostringstream error;

    int code = cmd.execute(input, output, error);

    EXPECT_EQ(code, 1);
    EXPECT_FALSE(error.str().empty());
    EXPECT_TRUE(output.str().empty());
}

TEST(GrepTest, ReadsFromExistingFile) {
    const std::string tmpfile = "/tmp/grep_test_tmp.txt";
    {
        std::ofstream f(tmpfile);
        f << "apple\nbanana\napricot\n";
    }

    GrepCommand cmd({"^a", tmpfile});
    std::istringstream input;
    std::ostringstream output;
    std::ostringstream error;

    int code = cmd.execute(input, output, error);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(output.str(), "apple\napricot\n");

    std::remove(tmpfile.c_str());
}

// ---------------------------------------------------------------------------
// Missing pattern argument
// ---------------------------------------------------------------------------

TEST(GrepTest, MissingPatternReturnsError) {
    GrepCommand cmd({});
    std::istringstream input("hello\n");
    std::ostringstream output;
    std::ostringstream error;

    int code = cmd.execute(input, output, error);

    EXPECT_EQ(code, 1);
    EXPECT_FALSE(error.str().empty());
}

// ---------------------------------------------------------------------------
// Empty input
// ---------------------------------------------------------------------------

TEST(GrepTest, EmptyInputNoMatch) {
    std::string out, err;
    int code = runGrep({"pattern"}, "", out, err);

    EXPECT_EQ(code, 1);
    EXPECT_TRUE(out.empty());
}
