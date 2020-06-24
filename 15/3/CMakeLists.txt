cmake_minimum_required(VERSION 3.16)
project(3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fsanitize=address")

add_executable(3_sh main_sh_ptr.cpp)
add_executable(3 main.cpp)