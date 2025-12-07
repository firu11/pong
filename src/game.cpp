#include "include/game.hpp"

#include "include/input.hpp"
#include "include/utils.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>


game::game(std::pair<int, int> canvasSize, int pointsToWin, int targetFps) {
    if (canvasSize.first * canvasSize.second > 1000000) std::exit(0); // nuh uh

    this->canvasSize = canvasSize;
    this->pointsToWin = pointsToWin;
    this->targetFps = targetFps;

    // create an 2d array of specified size
    this->canvas = new char *[canvasSize.second];
    for (int i = 0; i < canvasSize.second; i++) {
        this->canvas[i] = new char[canvasSize.first];
    }

    paddles[0] = new paddle(2, canvasSize.second / 2 - 7 / 2, 1, 7);
    paddles[1] = new paddle(canvasSize.first - 3, canvasSize.second / 2 - 7 / 2, 1, 7);

    ballInstance = new ball({canvasSize.first / 2, canvasSize.second / 2});
}

game::~game() {
    delete paddles[0];
    delete paddles[1];
    delete ballInstance;
}

void game::run() {
    std::thread t1(inputThreadFunc, std::ref(stop));
    std::thread t2(&game::computeThreadFunc, this);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // TODO lepe
    ballInstance->startMoving(0.5);

    t1.join();
    t2.join();
}

void game::goal(int pad) {
    if (pad == 1) score.first += 1;
    else if (pad == 2) score.second += 1;

    if (score.first > 5); // end
    else if (score.first > 5); // end

    delete ballInstance;
    ballInstance = new ball({canvasSize.first / 2, canvasSize.second / 2});

    delete paddles[0];
    delete paddles[1];
    paddles[0] = new paddle(2, canvasSize.second / 2 - 7 / 2, 1, 7);
    paddles[1] = new paddle(canvasSize.first - 2 - 1, canvasSize.second / 2 - 7 / 2, 1, 7);

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    ballInstance->startMoving(0.5);
}

void game::clearCanvas() const {
    for (int y = 0; y < canvasSize.second; y++) {
        for (int x = 0; x < canvasSize.first; x++) {
            canvas[y][x] = ' ';
        }
    }
}

void game::render() const {
    // ANSI escape codes:
    // \033[2J = clear screen
    // \033[H  = move cursor to top-left
    std::cout << "\033[H";

    // build top wall
    for (int i = 0; i < canvasSize.first; i++) {
        std::cout << '#';
    }
    std::cout << '\n';

    for (int y = 0; y < canvasSize.second; y++) {
        for (int x = 0; x < canvasSize.first; x++) {
            std::cout << canvas[y][x];
        }
        std::cout << "\r\n";
    }

    // build bottom wall
    for (int i = 0; i < canvasSize.first; i++) {
        std::cout << '#';
    }
    std::cout << '\n';

    std::cout << std::flush;
}

void game::computeThreadFunc() {
    while (!stop) {
        // move paddles
        float move = get_paddle_movement();
        if (move > 0) {
            paddles[0]->moveDown(canvasSize.second);
        } else if (move < 0) {
            paddles[0]->moveUp();
        }
        paddles[1]->updateAIPaddle(ballInstance, canvasSize.second);

        // check states
        ballInstance->move(canvasSize.second);
        auto g = ballInstance->checkGoalCollision(canvasSize.first);
        if (g != 0) goal(g);

        // draw
        clearCanvas();
        paddles[0]->draw(canvas);
        paddles[1]->draw(canvas);
        ballInstance->draw(canvas, canvasSize);
        render();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / targetFps));
    }
}


