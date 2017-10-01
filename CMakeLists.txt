cmake_minimum_required (VERSION 2.6)

set(APP_ID cgj)

project (${APP_ID})

add_executable (${APP_ID} src/app.cpp)

include_directories("include")
INCLUDE(FindOpenGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wextra -pedantic -std=c++11")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw-3.2.1)
target_link_libraries (${APP_ID} glfw)

add_subdirectory(libs/cgj-math)
target_link_libraries (${APP_ID} cgj-math)

find_package(OpenGL REQUIRED)
target_link_libraries(${APP_ID} ${OPENGL_gl_LIBRARY})

enable_testing()
add_test(vectorTests tests)
