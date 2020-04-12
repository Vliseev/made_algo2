cmake_minimum_required(VERSION 3.14)
project(5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_STANDARD 17)

add_executable(5 main.cpp)
add_executable(5_1 main1.cpp)
