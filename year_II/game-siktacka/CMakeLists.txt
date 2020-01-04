cmake_minimum_required(VERSION 3.1)
project(zadanie2)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")
include_directories(tests)

set(SOURCE_FILES
    communicate.cc
    config.cc
    event.cc
    generator.cc
    game-state.cc
    utils.cc
    IpAddress.cc)

set(SOURCE_CLIENT
    siktacka-client.cc)

set(SOURCE_SERVER
    siktacka-server.cc)

set(TEST_FILES
    tests/test_generator.cc
    tests/test_event.cc
    tests/test_utils.cc
    tests/test_communicate.cc)

add_library(common STATIC ${SOURCE_FILES})
install(TARGETS common DESTINATION lib)

add_executable(siktacka-server ${SOURCE_SERVER} siktacka-server-main.cc)
add_executable(siktacka-client ${SOURCE_CLIENT} siktacka-client-main.cc)
add_executable(siktacka-tests ${TEST_FILES} tests/tests_main.cc)

target_link_libraries(siktacka-server common)
target_link_libraries(siktacka-client common)
target_link_libraries(siktacka-tests common)
