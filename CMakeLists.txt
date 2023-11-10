cmake_minimum_required(VERSION 3.23)
project(json)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(json
        json.cpp
        json.hpp
        main.cpp)
