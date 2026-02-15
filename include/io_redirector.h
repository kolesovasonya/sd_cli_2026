#ifndef IO_REDIRECTOR_H
#define IO_REDIRECTOR_H

#include <vector>

/**
 * @brief Manages I/O redirection for pipeline commands
 *
 * Creates and manages pipes between commands in a pipeline.
 * Handles file descriptor setup for child processes.
 */
class IORedirector {
public:
    /**
     * @brief Creates pipes for pipeline
     * @param count Number of pipes to create (n-1 for n commands)
     * @return true if successful, false on error
     */
    bool createPipes(int count);

    /**
     * @brief Sets up pipes for a child process at given index
     * @param index Index of the command in the pipeline (0-based)
     * @param totalCommands Total number of commands in pipeline
     */
    void setupChildPipes(int index, int totalCommands);

    /**
     * @brief Closes all pipe file descriptors
     *
     * Must be called in parent process after all children are forked
     * and in each child process after pipe setup
     */
    void closeAllPipes();

    /**
     * @brief Destructor - ensures all pipes are closed
     */
    ~IORedirector();

private:
    std::vector<int*> pipes_;
};

#endif
