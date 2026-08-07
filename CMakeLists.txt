cmake_minimum_required(VERSION 3.16)
project(rubiks-cube CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(rubiks-cube
    main.cpp
    Cube.cpp
    ImagesManager.cpp
    cubeHelpers.cpp
)

target_include_directories(rubiks-cube PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(rubiks-cube PRIVATE ${OpenCV_LIBS})

target_compile_options(rubiks-cube PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
)
