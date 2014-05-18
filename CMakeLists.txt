cmake_minimum_required(VERSION 2.8)
project(tafl-client)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}") # Set ouput to build directory
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(dep)

include(CheckCompiler)
add_subdirectory(src)