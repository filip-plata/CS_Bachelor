cmake_minimum_required(VERSION 3.8)
project(kap)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        kap.cpp)

add_executable(kap ${SOURCE_FILES})