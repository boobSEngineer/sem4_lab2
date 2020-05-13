cmake_minimum_required(VERSION 3.15)
project(lab1)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tests)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(sem4lab2 main.cpp tree.cpp array.cpp byte_buffer.cpp fano.cpp)

