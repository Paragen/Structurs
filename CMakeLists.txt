cmake_minimum_required(VERSION 3.3)
project(Persistenttree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp set.cpp set.h)
add_executable(Persistenttree ${SOURCE_FILES})