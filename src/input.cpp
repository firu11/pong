#include <iostream>
#include <mutex>
#include <queue>
#include <unistd.h>
#include <utility>

#include "include/config.hpp"
#include "include/utils.hpp"

std::queue<std::string> keyboardInputQueue;
std::mutex keyboardInputMutex;

void inputThreadFunc(gameInfo *gi) {
    set_raw(true);

    char ch;
    while (read(STDIN_FILENO, &ch, 1) == 1) {
        std::string key;
        // esc seq
        if (ch == '\x1b') {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) != 1) continue;
            if (read(STDIN_FILENO, &seq[1], 1) != 1) continue;

            // Control Sequence Introducer
            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A':
                        key = "2-up";
                        break;
                    case 'B':
                        key = "2-down";
                        break;
                }
            }
        } else if (ch == 'w' || ch == 's') {
            switch (ch) {
                case 'w':
                    key = "1-up";
                    break;
                case 's':
                    key = "1-down";
                    break;
            }
            // ctrl+c OR q
        } else if (iscntrl(ch) || ch == 'q') {
            gi->stop = true;
            break;
        } else {
            key += ch;
        }

        if (key.empty()) continue;

        keyboardInputMutex.lock();
        keyboardInputQueue.emplace(key);
        keyboardInputMutex.unlock();
    }

    set_raw(false);
}

std::pair<float, float> get_paddle_movement() {
    int paddle1move = 0;
    int paddle2move = 0;

    keyboardInputMutex.lock();
    while (!keyboardInputQueue.empty()) {
        if (keyboardInputQueue.front() == "1-up") {
            paddle1move--;
        } else if (keyboardInputQueue.front() == "1-down") {
            paddle1move++;
        } else if (keyboardInputQueue.front() == "2-up") {
            paddle2move--;
        } else if (keyboardInputQueue.front() == "2-down") {
            paddle2move++;
        }
        keyboardInputQueue.pop();
    }
    keyboardInputMutex.unlock();

    return {paddle1move, paddle2move};
}
