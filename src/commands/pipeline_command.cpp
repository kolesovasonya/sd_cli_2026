#include "commands/pipeline_command.h"

#include "io_redirector.h"
#include "process_manager.h"

#ifdef _WIN32
#include <windows.h>
#else
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

    IORedirector redirector;
    if (!redirector.createPipes(n - 1)) {
        error << "Failed to create pipes: " << strerror(errno) << std::endl;
        return 1;
    }

    ProcessManager processManager;
    std::vector<pid_t> pids;

    for (int i = 0; i < n; i++) {
        pid_t pid = processManager.forkProcess();

        if (pid < 0) {
            error << "Fork failed: " << strerror(errno) << std::endl;
            for (pid_t p : pids) {
                processManager.terminateProcess(p);
            }
            return 1;
        }

        if (pid == 0) {
            // CHILD PROCESS

            redirector.setupChildPipes(i, n);

            redirector.closeAllPipes();

            int exitCode =
                commands_[i]->execute(std::cin, std::cout, std::cerr);
            exit(exitCode);
        }

        // PARENT PROCESS
        pids.push_back(pid);
    }

    redirector.closeAllPipes();

    std::vector<int> exitCodes;
    processManager.waitForProcesses(pids, exitCodes);

    // Return exit code of the last command
    return exitCodes[n - 1];
#endif
}
