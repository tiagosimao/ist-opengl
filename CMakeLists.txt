cmake_minimum_required (VERSION 2.6)
project (app)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wextra -pedantic -std=c++11")

add_executable (app app.cpp)
