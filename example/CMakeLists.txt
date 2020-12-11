cmake_minimum_required(VERSION 3.10)
project(cpphttpexample CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS_RELEASE "-O2")
set (CMAKE_INSTALL_RPATH "/usr/local/lib")
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include_directories(/usr/local/include/cpphttp)
add_executable(simple_server simple_server.cpp)

# Static Linking
find_package(Threads REQUIRED)
target_link_libraries(simple_server libcpphttpstatic.a)
target_link_libraries(simple_server ${CMAKE_THREAD_LIBS_INIT})

# Dynamic Linking
#target_link_libraries(simple_server cpphttp)
