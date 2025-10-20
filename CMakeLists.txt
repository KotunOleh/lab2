cmake_minimum_required(VERSION 3.15)
project(lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab2
    main.cpp
)

target_include_directories(lab2 PUBLIC include)