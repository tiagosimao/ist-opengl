cmake_minimum_required (VERSION 2.6)

project (cgj-math)

add_library(cgj-math src/vec.cpp include/cgj-math/vec.hpp)

target_include_directories(cgj-math PUBLIC
                           $<BUILD_INTERFACE:${cgj-math_SOURCE_DIR}/include>)
