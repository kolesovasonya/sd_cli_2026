#include "io_redirector.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>

#include <cstring>
#endif

bool IORedirector::createPipes(int count) {
#ifdef _WIN32
    return false;
#else
    for (int i = 0; i < count; i++) {
        int* pipefd = new int[2];
        if (pipe(pipefd) == -1) {
            for (auto p : pipes_) {
                close(p[0]);
                close(p[1]);
                delete[] p;
            }
            pipes_.clear();
            delete[] pipefd;
            return false;
        }
        pipes_.push_back(pipefd);
    }
    return true;
#endif
}

void IORedirector::setupChildPipes(int index, int totalCommands) {
#ifndef _WIN32
    if (index > 0) {
        dup2(pipes_[index - 1][0], STDIN_FILENO);
    }

    if (index < totalCommands - 1) {
        dup2(pipes_[index][1], STDOUT_FILENO);
    }
#endif
}

void IORedirector::closeAllPipes() {
#ifndef _WIN32
    for (auto p : pipes_) {
        close(p[0]);
        close(p[1]);
        delete[] p;
    }
    pipes_.clear();
#endif
}

IORedirector::~IORedirector() { closeAllPipes(); }
