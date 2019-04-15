//
// Created by daniil on 14.04.19.
//

#include <stdexcept>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include  <iostream>
#include "Walker.h"
#include "shell.h"

void Walker::parse_arguments(int argc, char **argv) {
    if (argc < 2) {
        throw std::invalid_argument("Path argument expected");
    }
    directory = argv[1];
    if (directory.back() == '/') {
        directory.pop_back();
    }
    if (argc % 2 == 1) {
        throw std::invalid_argument("Wrong number of arguments");
    }
    size_limit = SizeLimit::UNDEFINED;
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-inum") == 0) {
            try {
                inode = std::stoull(argv[++i]);
                has_inode_param = true;
            } catch (std::logic_error &) {
                throw std::invalid_argument("Wrong -inum argument");
            }
        } else if (strcmp(argv[i], "-name") == 0) {
            file_name = std::string(argv[++i]);
        } else if (strcmp(argv[i], "-nlinks") == 0) {
            try {
                has_nlinks_param = true;
                nlinks = std::stoull(argv[++i]);
            } catch (std::logic_error &) {
                throw std::invalid_argument("Wrong -nlinks argument");
            }
        } else if (strcmp(argv[i], "-size") == 0) {
            std::string arg_size;
            auto arg = argv[++i];
            switch (arg[0]) {
                case '+':
                    size_limit = SizeLimit::HIGHER;
                    break;
                case '-':
                    size_limit = SizeLimit::LOWER;
                    break;
                case '=':
                    size_limit = SizeLimit::EQUAL;
                    break;
                default:
                    throw std::invalid_argument("Wrong -size argument");
            }
            arg_size = std::string(arg + 1);
            try {
                size = std::stoull(arg_size);
            } catch (std::logic_error &) {
                throw std::invalid_argument("Wrong -size argument");
            }
        } else if (strcmp(argv[i], "-exec") == 0) {
            execution = std::string(argv[++i]);
        } else {
            throw std::invalid_argument(std::string("Wrong argument: ") + argv[i]);
        }
    }
}

void Walker::run(int argc, char **argv) {
    parse_arguments(argc, argv);
    walk(directory);
}

void Walker::walk(const std::string &path) {
    auto dir = opendir(path.data());
    if (dir == nullptr) {
        return;
    }
    struct stat buffer;
    while (auto file = readdir(dir)) {
        char *name = file->d_name;
        if (!name || strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            continue;
        }
        std::string new_path = path + "/" + name;
        switch (file->d_type) {
            case DT_DIR:
                walk(new_path);
                break;
            case DT_REG:
                if (stat(new_path.data(), &buffer) != 0) {
                    std::cerr << "Can't get info about " << new_path << ": " << strerror(errno) << std::endl;
                    break;
                }
                work_with_file(new_path, std::string(name), buffer);
                break;
            default:
                break;
        }
    }
}

void Walker::work_with_file(const std::string &path, const std::string& name, const struct stat &file_stat) {
    if ((has_inode_param && inode != file_stat.st_ino) ||
        (has_nlinks_param && nlinks != file_stat.st_nlink) ||
        (!file_name.empty() && name != file_name) ||
        !check_file_size(file_stat)) {
        return;
    }
    if (!execution.empty()) {
        shell::execute({execution, path});
    } else {
        std::cout << path << std::endl;
    }
}

bool Walker::check_file_size(const struct stat &file_stat) {
    auto file_size = file_stat.st_size;
    return ((size_limit == SizeLimit::EQUAL && file_size == size) ||
            (size_limit == SizeLimit::LOWER && file_size < size) ||
            (size_limit == SizeLimit::HIGHER && file_size > size) ||
            (size_limit == SizeLimit::UNDEFINED));
}
