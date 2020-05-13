cmake_minimum_required(VERSION 3.15)
project(lab_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")

add_subdirectory(google_tests)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(sem4lab2_tests tests.cpp ../tree.cpp ../array.cpp ../byte_buffer.cpp ../fano.cpp)
target_link_libraries(sem4lab2_tests gtest gtest_main)

