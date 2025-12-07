#include "include/game.hpp"

#include "include/input.hpp"
#include "include/utils.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <array>


game::game(std::pair<int, int> canvasSize, int pointsToWin, int targetFps) : paddles{} {
    if (canvasSize.first * canvasSize.second > 1000000) std::exit(0); // nuh uh

    this->canvasSize = canvasSize;
    this->pointsToWin = pointsToWin;
    this->targetFps = targetFps;

    // create a 2d array of specified size
    this->canvas = new char *[canvasSize.second];
    for (int i = 0; i < canvasSize.second; i++) {
        this->canvas[i] = new char[canvasSize.first];
    }

    paddles[0] = new playerPaddle(3, canvasSize.second / 2 - 7 / 2, 1, 7);
    paddles[1] = new aiPaddle(canvasSize.first - 4, canvasSize.second / 2 - 7 / 2, 1, 7);

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
    if (pad == 1) score.second += 1;
    else if (pad == 2) score.first += 1;

    if (score.first > pointsToWin); // end
    else if (score.first > pointsToWin); // end

    delete ballInstance;
    ballInstance = new ball({canvasSize.first / 2, canvasSize.second / 2});

    delete paddles[0];
    delete paddles[1];
    paddles[0] = new playerPaddle(3, canvasSize.second / 2 - 7 / 2, 1, 7);
    paddles[1] = new aiPaddle(canvasSize.first - 4, canvasSize.second / 2 - 7 / 2, 1, 7);

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
    std::cout << "\r\n";

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
    std::cout << "\r\n";

    std::cout << std::flush;
}

void game::computeThreadFunc() {
    while (!stop) {
        // move paddles
        paddles[0]->update(canvasSize.second, ballInstance);
        paddles[1]->update(canvasSize.second, ballInstance);

        // check states
        ballInstance->move(canvasSize.second);
        ballInstance->checkCollision(paddles);
        auto g = ballInstance->checkGoalCollision(canvasSize.first);
        if (g != 0) goal(g);

        // draw
        clearCanvas();
        drawScore();
        drawMiddleLine();
        paddles[0]->draw(canvas);
        paddles[1]->draw(canvas);
        ballInstance->draw(canvas, canvasSize);
        render();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / targetFps));
    }
}

void game::drawScore() {
    const std::array<std::array<std::string, 5>, 10> digits{
        {
            {
                " ### ",
                "#   #",
                "#   #",
                "#   #",
                " ### "
            }, // 0
            {
                "  #  ",
                " ##  ",
                "  #  ",
                "  #  ",
                " ### "
            }, // 1
            {
                " ### ",
                "    #",
                " ### ",
                "#    ",
                " ### "
            }, // 2
            {
                " ### ",
                "    #",
                " ### ",
                "    #",
                " ### "
            }, // 3
            {
                "#  # ",
                "#  # ",
                " ####",
                "   # ",
                "   # "
            }, // 4
            {
                " ### ",
                "#    ",
                " ### ",
                "    #",
                " ### "
            }, // 5
            {
                " ### ",
                "#    ",
                " ### ",
                "#   #",
                " ### "
            }, // 6
            {
                " ####",
                "    #",
                "   # ",
                "  #  ",
                "  #  "
            }, // 7
            {
                " ### ",
                "#   #",
                " ### ",
                "#   #",
                " ### "
            }, // 8
            {
                " ### ",
                "#   #",
                " ####",
                "    #",
                " ### "
            }, // 9
        }
    };

    // convert score to digits
    std::string leftStr = std::to_string(score.first);
    std::string rightStr = std::to_string(score.second);

    // build ascii lines
    std::array<std::string, 5> lines{};
    for (int i = 0; i < 5; i++) {
        lines[i] = "";
        for (char c: leftStr) lines[i] += digits[c - '0'][i];
        lines[i] += "         ";
        for (char c: rightStr) lines[i] += digits[c - '0'][i];
    }

    // draw lines to canvas centered
    int startY = canvasSize.second / 8;
    for (int i = 0; i < 5; i++) {
        int lineLen = static_cast<int>(lines[i].size());
        int startX = canvasSize.first / 2 - lineLen / 2 - 1;
        if (startY + i < 0 || startY + i >= canvasSize.second) continue;

        for (int j = 0; j < lineLen; j++) {
            int x = startX + j;
            if (x >= 0 && x < canvasSize.first) {
                canvas[startY + i][x] = lines[i][j];
            }
        }
    }
}

void game::drawMiddleLine() {
    int middle = canvasSize.first / 2 - 1;
    for (int y = 0; y < canvasSize.second; y++) {
        if (y % 5 == 0) {
            canvas[y][middle] = '|';
        }
    }
}


