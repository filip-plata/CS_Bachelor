cmake_minimum_required(VERSION 3.8)
project(lex)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        lex.cpp)

add_executable(lex ${SOURCE_FILES})