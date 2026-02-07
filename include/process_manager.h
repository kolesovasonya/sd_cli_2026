#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

/**
 * @brief Manages external process execution
 */
class ProcessManager {
public:
    /**
     * @brief Executes external program
     * @param program Program name or path
     * @param args Program arguments
     * @param environment Environment variables to pass
     * @param input Input stream
     * @param output Output stream
     * @param error Error stream
     * @return Program exit code
     */
    int executeExternal(
        const std::string& program,
        const std::vector<std::string>& args,
        const std::map<std::string, std::string>& environment,
        std::istream& input,
        std::ostream& output,
        std::ostream& error
    );
};

#endif
