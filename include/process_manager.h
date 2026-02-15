#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#ifndef _WIN32
#include <sys/types.h>
#endif

/**
 * @brief Manages process creation and synchronization
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
    int executeExternal(const std::string& program,
                        const std::vector<std::string>& args,
                        const std::map<std::string, std::string>& environment,
                        std::istream& input, std::ostream& output,
                        std::ostream& error);

#ifndef _WIN32
    /**
     * @brief Forks a new child process
     * @return pid_t of child process (0 in child, >0 in parent, <0 on error)
     */
    pid_t forkProcess();

    /**
     * @brief Waits for multiple processes and collects their exit codes
     * @param pids Vector of process IDs to wait for
     * @param exitCodes Output vector to store exit codes
     */
    void waitForProcesses(const std::vector<pid_t>& pids,
                          std::vector<int>& exitCodes);

    /**
     * @brief Terminates a process
     * @param pid Process ID to terminate
     * @return true if successful
     */
    bool terminateProcess(pid_t pid);
#endif
};

#endif
