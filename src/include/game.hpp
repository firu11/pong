#ifndef PONG_GAME_H
#define PONG_GAME_H

#include "ball.hpp"
#include "paddle.hpp"

#include <atomic>
#include <queue>
#include <string>
#include <utility>


class game {
    std::pair<int, int> canvasSize;
    std::pair<int, int> score;
    int pointsToWin;
    int targetFps;

    paddle *paddles[2];
    ball *ballInstance;
    std::atomic<bool> stop;
    char **canvas;

    std::queue<std::string> keyboardInputQueue;
    std::mutex keyboardInputMutex;

public:
    game(std::pair<int, int> canvasSize, int pointsToWin, int targetFps = 60);

    ~game();

    void run();

private:
    void goal(int pad);

    void render() const;

    void computeThreadFunc();

    void drawScore();

    void drawMiddleLine();

    void clearCanvas() const;
};


#endif //PONG_GAME_H
