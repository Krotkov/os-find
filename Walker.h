//
// Created by daniil on 14.04.19.
//

#ifndef OS_FIND_WALKER_H
#define OS_FIND_WALKER_H


#include <string>

class Walker {
    enum class SizeLimit {
        UNDEFINED,
        LOWER,
        HIGHER,
        EQUAL
    };
    std::string directory;
    std::string file_name;
    std::string execution;
    size_t size = 0;
    ino_t inode;
    nlink_t nlinks;
    bool has_inode_param = false;
    bool has_nlinks_param = false;
    SizeLimit size_limit;

    void parse_arguments(int argc, char **argv);

    void walk(const std::string &path);

    void work_with_file(const std::string &path, const std::string& name, const struct stat &file_stat);

    bool check_file_size(const struct stat &file_stat);

public:

    void run(int argc, char *argv[]);
};


#endif //OS_FIND_WALKER_H
