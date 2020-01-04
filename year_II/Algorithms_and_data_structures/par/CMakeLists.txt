cmake_minimum_required(VERSION 3.8)
project(par)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        par.cpp)

add_executable(par ${SOURCE_FILES})