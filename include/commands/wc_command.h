#ifndef WC_COMMAND_H
#define WC_COMMAND_H

#include <string>

#include "builtin_command.h"

/**
 * @brief Built-in wc command - counts lines, words and bytes in file or stdin
 */
class WcCommand : public BuiltinCommand {
public:
    /**
     * @brief Constructs wc command
     * @param filename File to analyze (empty or "-" for stdin)
     */
    explicit WcCommand(const std::string& filename = "");

    /**
     * @brief Executes wc command
     * @param input Input stream (used when no file specified)
     * @param output Output stream
     * @param error Error stream
     * @return Exit code (0 for success, 1 for error)
     */
    int execute(std::istream& input, std::ostream& output,
                std::ostream& error) override;

private:
    std::string filename_;

    /**
     * @brief Helper method to count lines, words, and bytes from a stream
     * @param stream Input stream to count from
     * @param lines Output: number of lines
     * @param words Output: number of words
     * @param bytes Output: number of bytes
     */
    void countFromStream(std::istream& stream, size_t& lines, size_t& words,
                         size_t& bytes);
};

#endif
