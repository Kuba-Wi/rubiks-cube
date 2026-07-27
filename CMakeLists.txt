cmake_minimum_required(VERSION 3.16)
project(rubiks-cube CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rubiks-cube
    main.cpp
    Cube.cpp
)

target_compile_options(rubiks-cube PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
)
