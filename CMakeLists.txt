cmake_minimum_required(VERSION 3.12)
project(col671_assgn2)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-Ofast -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

include_directories(.)

add_executable(yinsh_ai
        Agent.cpp
        Agent.h
        Board.cpp
        Board.h
        main.cpp)

add_executable(test
        Agent.cpp
        Agent.h
        Board.cpp
        Board.h
        test.cpp)
