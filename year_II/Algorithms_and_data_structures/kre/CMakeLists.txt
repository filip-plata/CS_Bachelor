cmake_minimum_required(VERSION 3.8)
project(kre)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
         segment_tree.h)

add_executable(kre kre.cpp ${SOURCE_FILES})

add_executable(testing test.cpp ${SOURCE_FILES})