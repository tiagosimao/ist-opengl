cmake_minimum_required (VERSION 2.6)

set(APP_ID cgj)

project (${APP_ID})

file(GLOB_RECURSE mainSources "src/*.cpp")
add_executable (${APP_ID} ${mainSources})

include_directories("include")
include_directories("libs/glew-2.1.0/include")
include_directories("src")
INCLUDE(FindOpenGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wextra -pedantic -std=c++11")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


add_subdirectory(libs/glfw-3.2.1)
target_link_libraries (${APP_ID} glfw)

add_subdirectory(libs/glew-2.1.0/build/cmake)
target_link_libraries (${APP_ID} glew)

add_subdirectory(libs/cgj-math)
target_link_libraries (${APP_ID} cgj-math)

find_package(OpenGL REQUIRED)
target_link_libraries(${APP_ID} ${OPENGL_gl_LIBRARY})

set(GLEW_LIBRARY, "/bin/lib")

#include_directories(${GLEW_INCLUDE_DIRS})
#link_libraries(${GLEW_LIBRARIES})

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE unit-tests)

add_executable(main-tests unit-tests/main-tests.cpp)
target_link_libraries(main-tests Catch)

target_link_libraries (main-tests cgj-math)

enable_testing()
add_test(NAME vectorTests COMMAND main-tests)
