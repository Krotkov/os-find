//
// Created by daniil on 14.04.19.
//

#ifndef OS_FIND_SHELL_H
#define OS_FIND_SHELL_H


#include <string>
#include <vector>

class shell {
    static void print_error(const std::string& message);

    static void run(char* const argv[], char* const env[]);

public:
    static void execute(const std::vector<std::string>& args);
};


#endif //OS_FIND_SHELL_H
