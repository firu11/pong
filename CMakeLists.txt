cmake_minimum_required(VERSION 4.0)
project(pong)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        pong

        src/main.cpp
        src/game.cpp
        src/ball.cpp
        src/paddle.cpp
        src/input.cpp

        src/include/game.hpp
        src/include/ball.hpp
        src/include/paddle.hpp
        src/include/input.hpp
        src/include/utils.hpp
)
