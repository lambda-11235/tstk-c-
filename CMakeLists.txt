
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

cmake_minimum_required(VERSION 3.5)
project(tstk)

add_subdirectory(src bin)

set_property(TARGET tstk PROPERTY CXX_STANDARD 11)
