cmake_minimum_required(VERSION 3.16)
project(project CXX)

set (CMAKE_CXX_STANDARD 17)
option(DEVELOPMENT_BUILD "Build tests" OFF)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_library(project SHARED
        ${SOURCES}
        ${HEADERS})
include_directories(project ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (DEVELOPMENT_BUILD)
    enable_testing()
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(unittest ${SOURCES} tests/sqrt_test.cpp)
    target_link_libraries(unittest ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(unittest ${CMAKE_THREAD_LIBS_INIT} )
endif()
