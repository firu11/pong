cmake_minimum_required(VERSION 4.0)
project(pong)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        pong

        src/main.cpp
        src/ball.cpp
        src/paddle.cpp

        include/ball.hpp
        include/paddle.hpp
        include/utils.hpp
)
