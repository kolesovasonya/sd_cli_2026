#ifndef EXTERNAL_COMMAND_H
#define EXTERNAL_COMMAND_H

#include "abstract_command.h"
#include <string>
#include <vector>

/**
 * @brief Represents external program command
 */
class ExternalCommand : public AbstractCommand {
public:
    /**
     * @brief Constructs external command
     * @param program Program name or path
     * @param args Program arguments
     */
    ExternalCommand(const std::string& program, const std::vector<std::string>& args);
    
    /**
     * @brief Executes external program
     * @param input Input stream
     * @param output Output stream
     * @param error Error stream
     * @return Program exit code
     */
    int execute(std::istream& input, std::ostream& output, std::ostream& error) override;
    
private:
    std::string program_;
    std::vector<std::string> args_;
};

#endif
