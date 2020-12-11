cmake_minimum_required(VERSION 3.10)
project(cpphttpproject CXX)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS_RELEASE "-O2")

find_package(Threads REQUIRED)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../../src/*.h)

#set(CMAKE_BUILD_TYPE Debug)
include_directories(simple_server ${CMAKE_CURRENT_SOURCE_DIR}/../../src)
add_executable(simple_server simple_server.cpp ${SOURCES})
target_link_libraries(simple_server ${CMAKE_THREAD_LIBS_INIT})
