#include <atomic>
#include <iostream>
#include <thread>

#include <termios.h> // For terminal control
#include <unistd.h>  // For POSIX functions like STDIN_FILENO
#include <cstdio>    // For getchar()
#include <mutex>
#include <queue>

#include "../include/utils.hpp"

std::atomic_bool stop = false;

std::queue<std::string> keyboardInputQueue;

std::mutex keyboardInputMutex;

void inputThreadFunc() {
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
                        key = "up";
                        break;
                    case 'B':
                        key = "down";
                        break;
                    case 'C':
                        key = "right";
                        break;
                    case 'D':
                        key = "left";
                        break;
                }
            }
            // ctrl+c OR q
        } else if (iscntrl(ch) || ch == 'q') {
            stop = true;
            return;
        } else {
            key += ch;
        }

        if (key.empty()) continue;

        keyboardInputMutex.lock();
        keyboardInputQueue.emplace(key);
        keyboardInputMutex.unlock();
    }

    set_raw(true);
}

void renderThreadFunc() {
    while (!stop) {
    }
}

void computeThreadFunc() {
    while (!stop) {
        keyboardInputMutex.lock();
        if (!keyboardInputQueue.empty()) {
            std::cout << keyboardInputQueue.front() << ' ' << std::flush;
            keyboardInputQueue.pop();
        }
        keyboardInputMutex.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::thread t1(inputThreadFunc);
    std::thread t2(renderThreadFunc);

    t1.join();
    t2.join();
    return 0;
}
