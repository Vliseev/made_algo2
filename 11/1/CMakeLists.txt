cmake_minimum_required(VERSION 3.16)
project(1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address")
set(CMAKE_CXX_STANDARD 17)


add_executable(1 main.cpp)
