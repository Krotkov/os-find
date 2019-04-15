//
// Created by daniil on 14.04.19.
//

#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <wait.h>

#include "shell.h"

void shell::run(char *const *argv, char *const *env) {
    pid_t pid = fork();
    if (pid == -1) {
        print_error("Fork failed: ");
    } else if (pid == 0) {
        if (execve(argv[0], argv, env) == -1) {
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            print_error("failed: ");
        } else {
            std::cout << "Execution return code: " << WEXITSTATUS(status) << std::endl;
        }
    }
}

void shell::print_error(const std::string &message) {
    if (errno != 0) std::cerr << message << strerror(errno) << std::endl;
    else std::cerr << message << std::endl;
    std::cerr.flush();
}

void shell::execute(const std::vector<std::string> &args) {
    std::vector<char *> argv;
    argv.reserve(args.size());
    for (const auto &arg: args) {
        argv.push_back(const_cast<char *>(arg.data()));
    }
    argv.push_back(nullptr);
    run(argv.data(), nullptr);
}
