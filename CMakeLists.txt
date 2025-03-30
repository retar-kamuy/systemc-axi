cmake_minimum_required(VERSION 3.14)
# set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_MODULE_PATH})
set(CMAKE_CC_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(main)

# find_package( <LIBRARY> <VERSION> REQUIRED )
# if( <LIBRARY>_FOUND )
    include_directories($ENV{SYSTEMC_HOME}/include)
    find_library(systemc systemc $ENV{SYSTEMC_HOME}/lib-linux64)
    link_directories($ENV{SYSTEMC_HOME}/lib-linux64)
# endif()

add_executable(main design.cpp testbench.cpp)
target_link_libraries(main systemc m)

