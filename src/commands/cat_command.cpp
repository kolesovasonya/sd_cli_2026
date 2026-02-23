#include "commands/cat_command.h"

#include <fstream>

CatCommand::CatCommand(const std::string& filename) : filename_(filename) {}

int CatCommand::execute(std::istream& input, std::ostream& output,
                        std::ostream& error) {
    if (filename_.empty() || filename_ == "-") {
        std::string line;
        while (std::getline(input, line)) {
            output << line << std::endl;
        }
        return 0;
    }

    std::ifstream file(filename_);

    if (!file.is_open()) {
        error << "cat: " << filename_ << ": No such file or directory"
              << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        output << line << std::endl;
    }

    file.close();
    return 0;
}
