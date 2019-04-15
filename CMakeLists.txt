cmake_minimum_required(VERSION 3.9)
project(os-find)

set(CMAKE_CXX_STANDARD 14)

add_executable(os-find find.cpp shell.cpp Walker.cpp)
