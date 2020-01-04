cmake_minimum_required(VERSION 3.5)
project(Zadanie2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lpthread -O2 -DNDEBUG -Wall")

set(SOURCE_FILES main.cpp)
add_executable(Zadanie2 ${SOURCE_FILES})

enable_testing()
add_test(Example1 test1)
