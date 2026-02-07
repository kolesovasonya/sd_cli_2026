#ifndef WC_COMMAND_H
#define WC_COMMAND_H

#include <string>

#include "builtin_command.h"

/**
 * @brief Built-in wc command - counts lines, words and bytes in file
 */
class WcCommand : public BuiltinCommand {
public:
    /**
     * @brief Constructs wc command
     * @param filename File to analyze
     */
    explicit WcCommand(const std::string& filename);

    /**
     * @brief Executes wc command
     * @param input Input stream
     * @param output Output stream
     * @param error Error stream
     * @return Exit code (0 for success, 1 for error)
     */
    int execute(std::istream& input, std::ostream& output,
                std::ostream& error) override;

private:
    std::string filename_;
};

#endif
