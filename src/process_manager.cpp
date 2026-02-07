#include "process_manager.h"
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <sstream>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#endif

int ProcessManager::executeExternal(
    const std::string& program,
    const std::vector<std::string>& args,
    const std::map<std::string, std::string>& environment,
    std::istream& input,
    std::ostream& output,
    std::ostream& error
) {
#ifdef _WIN32
    std::string cmdLine = program;
    for (const auto& arg : args) {
        cmdLine += " " + arg;
    }
    
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    
    std::string envBlock;
    for (const auto& [key, value] : environment) {
        envBlock += key + "=" + value + '\0';
    }
    if (!envBlock.empty()) {
        envBlock += '\0';
    }
    
    BOOL success = CreateProcessA(
        NULL,
        const_cast<char*>(cmdLine.c_str()),
        NULL,
        NULL,
        TRUE,
        0,
        envBlock.empty() ? NULL : const_cast<char*>(envBlock.c_str()),
        NULL,
        &si,
        &pi
    );
    
    if (!success) {
        error << "Failed to execute: " << program << std::endl;
        return 1;
    }
    
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    return static_cast<int>(exitCode);
#else
    pid_t pid = fork();
    
    if (pid < 0) {
        error << "Fork failed" << std::endl;
        return 1;
    }
    
    if (pid == 0) {
        for (const auto& [key, value] : environment) {
            setenv(key.c_str(), value.c_str(), 1);
        }
        
        std::vector<char*> argv;
        argv.push_back(const_cast<char*>(program.c_str()));
        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);
        
        execvp(program.c_str(), argv.data());
        
        std::cerr << "Failed to execute: " << program << std::endl;
        exit(127);
    }
    
    int status;
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    
    return 1;
#endif
}
