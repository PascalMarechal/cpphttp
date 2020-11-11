cmake_minimum_required(VERSION 3.16)
project(cpphttp CXX)
project(cpphttp VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 17)
option(DEVELOPMENT_BUILD "Build tests" OFF)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_library(cpphttp SHARED
        ${SOURCES}
        ${HEADERS})
include_directories(cpphttp ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(cpphttp PROPERTIES VERSION ${cpphttp_VERSION})

if (DEVELOPMENT_BUILD)
    enable_testing()
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
    add_executable(unittest ${SOURCES} ${TEST_SOURCES})
    include_directories(unittest ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    target_link_libraries(unittest ${GTEST_BOTH_LIBRARIES} gmock)
    target_link_libraries(unittest ${CMAKE_THREAD_LIBS_INIT} )
endif()
