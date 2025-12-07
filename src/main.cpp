#include <atomic>
#include <iostream>
#include <thread>

#include <mutex>

#include "include/paddle.hpp"
#include "include/ball.hpp"
#include "include/input.hpp"
#include "include/utils.hpp"

void goal(gameInfo *gi, int pad) {
    if (pad == 1) gi->score.first += 1;
    else if (pad == 2) gi->score.second += 1;

    if (gi->score.first > 5); // end
    else if (gi->score.first > 5); // end

    delete gi->ballInstance;
    gi->ballInstance = new ball({screenWidth / 2, screenHeight / 2});

    delete gi->paddles[0];
    delete gi->paddles[1];
    gi->paddles[0] = new paddle(2, screenHeight / 2 - 7 / 2, 1, 7);
    gi->paddles[1] = new paddle(screenWidth - 2 - 1, screenHeight / 2 - 7 / 2, 1, 7);

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    gi->ballInstance->startMoving(0.5);
}

void clearCanvas(gameInfo *gi) {
    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            gi->canvas[y][x] = ' ';
        }
    }
}

void render(gameInfo *gi) {
    // ANSI escape codes:
    // \033[2J = clear screen
    // \033[H  = move cursor to top-left
    std::cout << "\033[H";

    // build top wall
    for (int i = 0; i < screenWidth; i++) {
        std::cout << '#';
    }
    std::cout << '\n';

    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            std::cout << gi->canvas[y][x];
        }
        std::cout << "\r\n";
    }

    // build bottom wall
    for (int i = 0; i < screenWidth; i++) {
        std::cout << '#';
    }
    std::cout << '\n';

    std::cout << std::flush;
}

void computeThreadFunc(gameInfo *gi) {
    while (!gi->stop) {
        // move paddles
        float move = get_paddle_movement();
        if (move > 0) {
            gi->paddles[0]->moveDown();
        } else if (move < 0) {
            gi->paddles[0]->moveUp();
        }
        gi->paddles[1]->updateAIPaddle(gi->ballInstance);

        // check states
        gi->ballInstance->move();
        auto g = gi->ballInstance->checkGoalCollision();
        if (g != 0) goal(gi, g);

        // draw
        clearCanvas(gi);
        gi->paddles[0]->draw(gi->canvas);
        gi->paddles[1]->draw(gi->canvas);
        gi->ballInstance->draw(gi->canvas);
        render(gi);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / targetFps));
    }
}

int main() {
    gameInfo gi = {
        .paddles = {
            new paddle(2, screenHeight / 2 - 7 / 2, 1, 7),
            new paddle(screenWidth - 2 - 1, screenHeight / 2 - 7 / 2, 1, 7)
        },
        .ballInstance = new ball({screenWidth / 2, screenHeight / 2}),
    };

    clearTerminalScreen();
    hideCursor();

    std::thread t1(inputThreadFunc, &gi);
    std::thread t2(computeThreadFunc, &gi);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // TODO lepe
    gi.ballInstance->startMoving(0.5);

    t1.join();
    t2.join();

    showCursor();

    delete gi.paddles[0];
    delete gi.paddles[1];
    delete gi.ballInstance;
    return 0;
}
