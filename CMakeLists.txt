cmake_minimum_required(VERSION 3.27)
project(aoc2023 CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_GENERATOR "Ninja")
add_compile_options(-pedantic -Wall -Wextra)

add_subdirectory(day01)
add_subdirectory(day02)
