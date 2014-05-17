cmake_minimum_required(VERSION 2.8)
project(tafl-client)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")

include(CheckCompiler)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(src)