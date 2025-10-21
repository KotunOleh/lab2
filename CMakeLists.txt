cmake_minimum_required(VERSION 3.16)

project(lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab2 main.cpp)

find_package(Threads REQUIRED)

add_compile_options(/O3)

target_link_libraries(lab2 PRIVATE Threads::Threads)