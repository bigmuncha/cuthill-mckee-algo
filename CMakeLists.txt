cmake_minimum_required (VERSION 3.2)

project(CuthillProject)

# Turn on if you need verbose compilation
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_BUILD_TYPE distribution)
SET(CMAKE_CPP_FLAGS_DISTRIBUTION "-Wall -O0 -g -std=gnu++20 ")
SET(CMAKE_CPP_COMPILER_ID "GNU")
add_executable(omar cuthill-mckee.cpp)
