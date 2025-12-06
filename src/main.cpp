#include <atomic>
#include <iostream>
#include <thread>

#include <mutex>

#include "include/paddle.hpp"
#include "include/ball.hpp"
#include "include/input.hpp"
#include "include/utils.hpp"

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
        auto [moveP1, moveP2] = get_paddle_movement();
        if (moveP1 > 0) {
            gi->paddles[0]->moveDown();
        } else if (moveP1 < 0) {
            gi->paddles[0]->moveUp();
        }
        if (moveP2 > 0) {
            gi->paddles[1]->moveDown();
        } else if (moveP2 < 0) {
            gi->paddles[1]->moveUp();
        }

        clearCanvas(gi);
        gi->paddles[0]->draw(gi->canvas);
        gi->paddles[1]->draw(gi->canvas);


        render(gi);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / targetFps));
    }
}

int main() {
    gameInfo gi = {
        .paddles = {
            new paddle(2, screenHeight / 2 - 6 / 2, 1, 6),
            new paddle(screenWidth - 2 - 1, screenHeight / 2 - 6 / 2, 1, 6)
        },
        .ballInstance = new ball(std::pair{screenWidth / 2, screenHeight / 2}),
    };

    clearTerminalScreen();
    hideCursor();

    std::thread t1(inputThreadFunc, &gi);
    std::thread t2(computeThreadFunc, &gi);

    t1.join();
    t2.join();

    showCursor();

    delete gi.paddles[0];
    delete gi.paddles[1];
    delete gi.ballInstance;
    return 0;
}
