cmake_minimum_required(VERSION 3.15)
project(2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
set(CMAKE_CXX_STANDARD 14)

add_executable(2 main.cpp)
add_executable(1 m.cpp)
