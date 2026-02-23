#ifndef CAT_COMMAND_H
#define CAT_COMMAND_H

#include <string>

#include "builtin_command.h"

/**
 * @brief Built-in cat command - outputs file contents or stdin
 */
class CatCommand : public BuiltinCommand {
public:
    /**
     * @brief Constructs cat command
     * @param filename File to output (empty or "-" for stdin)
     */
    explicit CatCommand(const std::string& filename = "");

    /**
     * @brief Executes cat command
     * @param input Input stream (used when no file specified)
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
