#include "commands/pipeline_command.h"

#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#endif

PipelineCommand::PipelineCommand(
    std::vector<std::unique_ptr<AbstractCommand>> commands)
    : commands_(std::move(commands)) {}

int PipelineCommand::execute(std::istream& input, std::ostream& output,
                             std::ostream& error) {
#ifdef _WIN32
    error << "Pipelines are not supported on Windows" << std::endl;
    return 1;
#else
    if (commands_.empty()) {
        return 0;
    }

    if (commands_.size() == 1) {
        return commands_[0]->execute(input, output, error);
    }

    int n = commands_.size();

    std::vector<int*> pipes;
    for (int i = 0; i < n - 1; i++) {
        int* pipefd = new int[2];
        if (pipe(pipefd) == -1) {
            error << "Failed to create pipe: " << strerror(errno) << std::endl;
            for (auto p : pipes) {
                close(p[0]);
                close(p[1]);
                delete[] p;
            }
            return 1;
        }
        pipes.push_back(pipefd);
    }

    std::vector<pid_t> pids;
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            error << "Fork failed: " << strerror(errno) << std::endl;
            for (auto p : pipes) {
                close(p[0]);
                close(p[1]);
                delete[] p;
            }
            for (pid_t p : pids) {
                kill(p, SIGTERM);
            }
            return 1;
        }

        if (pid == 0) {
            // CHILD PROCESS

            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            if (i < n - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            for (auto p : pipes) {
                close(p[0]);
                close(p[1]);
            }

            int exitCode = commands_[i]->execute(std::cin, std::cout, std::cerr);
            exit(exitCode);
        }

        // PARENT PROCESS
        pids.push_back(pid);
    }

    for (auto p : pipes) {
        close(p[0]);
        close(p[1]);
        delete[] p;
    }

    std::vector<int> exitCodes(n);
    for (int i = 0; i < n; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status)) {
            exitCodes[i] = WEXITSTATUS(status);
        } else {
            exitCodes[i] = 1;
        }
    }

    // Return exit code of the last command
    return exitCodes[n - 1];
#endif
}
