cmake_minimum_required(VERSION 3.14)
project(4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address")
set(CMAKE_CXX_STANDARD 17)

add_executable(4 main.cpp)