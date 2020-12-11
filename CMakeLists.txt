cmake_minimum_required(VERSION 3.10)
project(cpphttp CXX)
project(cpphttp VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS_RELEASE "-O2")

option(BUILD_TESTS "Build google tests" OFF)
option(BUILD_LIBS "Build both libraries" ON)

# Library part
find_package(Threads REQUIRED)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

if (BUILD_LIBS)
    add_library(cpphttp SHARED
            ${SOURCES}
            ${HEADERS})
    include_directories(cpphttp ${CMAKE_CURRENT_SOURCE_DIR}/src)
    set_target_properties(cpphttp PROPERTIES VERSION ${cpphttp_VERSION})
    target_link_libraries(cpphttp ${CMAKE_THREAD_LIBS_INIT})

    add_library(cpphttpstatic STATIC
            ${SOURCES}
            ${HEADERS})
    include_directories(cpphttpstatic ${CMAKE_CURRENT_SOURCE_DIR}/src)
    set_target_properties(cpphttpstatic PROPERTIES VERSION ${cpphttp_VERSION})
    target_link_libraries(cpphttpstatic ${CMAKE_THREAD_LIBS_INIT})

    # Installation part
    file(GLOB PUBLIC_HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/request/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/response/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/server/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

    install(TARGETS cpphttp 
        LIBRARY DESTINATION /usr/local/lib)

    install(TARGETS cpphttpstatic 
        DESTINATION /usr/local/lib)
        
    foreach ( file ${PUBLIC_HEADER_FILES} )
        get_filename_component( dir ${file} DIRECTORY )
        string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/src "" dir ${dir})
        install( FILES ${file} DESTINATION /usr/local/include/cpphttp/${dir} )
    endforeach()
endif()

# Test part
if (BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    find_package(CURL REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    include_directories(${CURL_INCLUDE_DIRS})
    file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/*.cpp)
    add_executable(unittest ${SOURCES} ${TEST_SOURCES})
    include_directories(unittest ${CMAKE_CURRENT_SOURCE_DIR}/src)
    include_directories(unittest ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit)
    target_link_libraries(unittest ${GTEST_BOTH_LIBRARIES} gmock curl)
    target_link_libraries(unittest ${CMAKE_THREAD_LIBS_INIT})
endif()
