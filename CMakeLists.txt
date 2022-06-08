cmake_minimum_required(VERSION 3.22)
project(MineSweeper)

set(CMAKE_CXX_STANDARD 11)

add_library(lib.h SHARED IMPORTED)
add_executable(MineSweeper main.cpp)
