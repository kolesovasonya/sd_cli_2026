#include "commands/wc_command.h"

#include <fstream>
#include <sstream>

WcCommand::WcCommand(const std::string& filename) : filename_(filename) {}

int WcCommand::execute(std::istream& input, std::ostream& output,
                       std::ostream& error) {
    std::ifstream file(filename_);

    if (!file.is_open()) {
        error << "wc: " << filename_ << ": No such file or directory"
              << std::endl;
        return 1;
    }

    size_t lines = 0;
    size_t words = 0;
    size_t bytes = 0;

    std::string line;
    while (std::getline(file, line)) {
        lines++;
        bytes += line.length() + 1;

        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            words++;
        }
    }

    output << lines << " " << words << " " << bytes << " " << filename_
           << std::endl;

    file.close();
    return 0;
}
