cmake_minimum_required (VERSION 2.6)
project (app)

include_directories(include)

INCLUDE(FindOpenGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wextra -pedantic -std=c++11")

add_executable (app app.cpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw-3.2.1)

find_package(OpenGL REQUIRED)

target_link_libraries(app ${OPENGL_gl_LIBRARY})

target_link_libraries (app glfw)