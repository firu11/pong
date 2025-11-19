cmake_minimum_required(VERSION 4.0)
project(pong)

set(CMAKE_CXX_STANDARD 20)

add_executable(pong src/main.cpp
        src/paddle.cpp
        src/paddle.h
        src/ball.cpp
        src/ball.h)
