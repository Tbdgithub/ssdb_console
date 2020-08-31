cmake_minimum_required(VERSION 3.16)
project(ssdb_console)

set(CMAKE_CXX_STANDARD 14)

add_executable(ssdb_console main.cpp tool/Hello.cpp)