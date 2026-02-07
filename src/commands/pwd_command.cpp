#include "commands/pwd_command.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

int PwdCommand::execute(std::istream& input, std::ostream& output, std::ostream& error) {
#ifdef _WIN32
    char buffer[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, buffer)) {
        output << buffer << std::endl;
        return 0;
    }
#else
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer))) {
        output << buffer << std::endl;
        return 0;
    }
#endif
    
    error << "pwd: failed to get current directory" << std::endl;
    return 1;
}
