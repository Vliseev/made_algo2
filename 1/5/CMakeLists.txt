cmake_minimum_required(VERSION 3.10)
project(1)

set(CMAKE_CXX_FLAGS "-O0 -g -fsanitize=address")
set(CMAKE_CXX_STANDARD 17)

add_executable(1 main2.cpp)