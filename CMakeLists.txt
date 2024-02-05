cmake_minimum_required(VERSION 2.8...3.5)
set(CMAKE_CXX_STANDARD 11)

project(InfoProjC++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Program1 src/Program1/program1_main.cpp)
add_executable(Program2 src/Program2/program2_main.cpp)

