#ifndef PIPELINE_COMMAND_H
#define PIPELINE_COMMAND_H

#include <memory>
#include <vector>

#include "abstract_command.h"

/**
 * @brief Command that executes a pipeline of commands
 *
 * Connects stdout of each command to stdin of the next command using pipes.
 * All commands run in parallel in separate processes.
 * Returns the exit code of the last command in the pipeline.
 */
class PipelineCommand : public AbstractCommand {
public:
    /**
     * @brief Constructs a pipeline command
     * @param commands Vector of commands to execute in pipeline
     */
    explicit PipelineCommand(
        std::vector<std::unique_ptr<AbstractCommand>> commands);

    /**
     * @brief Executes the pipeline
     * @param input Input stream for the first command
     * @param output Output stream for the last command
     * @param error Error stream (shared by all commands)
     * @return Exit code of the last command
     */
    int execute(std::istream& input, std::ostream& output,
                std::ostream& error) override;

private:
    std::vector<std::unique_ptr<AbstractCommand>> commands_;
};

#endif
