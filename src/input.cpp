#include "include/utils.hpp"

#include <iostream>
#include <mutex>
#include <queue>
#include <atomic>

std::queue<std::string> keyboardInputQueue;
std::mutex keyboardInputMutex;

void inputThreadFunc(std::atomic_bool &stop) {
    set_raw(true);

    char ch;
    while (std::cin.get(ch)) {
        std::string key;
        if (ch == '\x1b') {
            char seq[2];
            if (std::cin.get(seq[0]) && std::cin.get(seq[1])) {
                if (seq[0] == '[') {
                    switch (seq[1]) {
                        case 'A':
                            key = "up";
                            break;
                        case 'B':
                            key = "down";
                            break;
                        default: ;
                    }
                }
            }
        } else if (ch == 'w' || ch == 's') {
            key = (ch == 'w') ? "up" : "down";
        } else if ((ch >= 0 && ch <= 31) || ch == 127 || ch == 'q') {
            // ASCII 0-31 are control chars, 127 is DEL
            stop = true;
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

int get_paddle_movement() {
    int paddleMoveSum = 0;

    keyboardInputMutex.lock();
    while (!keyboardInputQueue.empty()) {
        if (keyboardInputQueue.front() == "up") {
            paddleMoveSum--;
        } else if (keyboardInputQueue.front() == "down") {
            paddleMoveSum++;
        }
        keyboardInputQueue.pop();
    }
    keyboardInputMutex.unlock();

    return paddleMoveSum;
}
