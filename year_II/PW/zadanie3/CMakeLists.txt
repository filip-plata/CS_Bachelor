cmake_minimum_required(VERSION 3.5)
project(Zadanie3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2 -std=gnu11 -Wextra -pedantic")
 
set(SOURCE_FILES main.c
            expression.h
            parse.c
            parse.h
            communication.c
            communication.h
            expression.c
            graph.c
            graph.h)
add_executable(Zadanie3 ${SOURCE_FILES})
