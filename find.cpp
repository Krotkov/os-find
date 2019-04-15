//
// Created by daniil on 14.04.19.
//
#include <exception>
#include <stdexcept>
#include <iostream>
#include "Walker.h"



int main(int argc, char* argv[]) {
    try {
        Walker walker;
        walker.run(argc, argv);
    } catch (std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}