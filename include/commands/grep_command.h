#ifndef GREP_COMMAND_H
#define GREP_COMMAND_H

#include <string>
#include <vector>

#include "builtin_command.h"

/**
 * @brief Built-in grep command - searches for patterns in files or stdin
 *
 * Supports regular expressions, whole-word matching (-w),
 * case-insensitive search (-i), and trailing context lines (-A N).
 */
class GrepCommand : public BuiltinCommand {
public:
    /**
     * @brief Constructs grep command with raw argument list
     * @param args Arguments as passed by the user (pattern, optional file,
     *             and flags in any order)
     */
    explicit GrepCommand(std::vector<std::string> args);

    /**
     * @brief Executes grep: parses args, compiles regex, scans input lines
     * @param input  Input stream used when no file argument is provided
     * @param output Output stream for matching lines
     * @param error  Error stream for diagnostic messages
     * @return 0 if at least one match was found, 1 otherwise
     */
    int execute(std::istream& input, std::ostream& output,
                std::ostream& error) override;

private:
    /**
     * @brief Prints collected lines to output, inserting "--" separators
     *        between non-adjacent groups produced by -A context expansion
     * @param lines         All input lines
     * @param should_print  Per-line flag indicating whether to print
     * @param after_context Number of context lines after each match (controls
     *                      whether separators are emitted)
     * @param output        Destination stream
     */
    void printWithSeparators(const std::vector<std::string>& lines,
                             const std::vector<bool>& should_print,
                             int after_context, std::ostream& output) const;

    std::vector<std::string> args_;
};

#endif
