#ifndef PONG_UTILS_H
#define PONG_UTILS_H

#include <cstdlib>

// Stolen from https://gitlab.fel.cvut.cz/nagyoing/mujprojekt/-/blob/master/main.cpp?ref_type=heads
inline void set_raw(bool set) {
    if (set) {
        system("stty raw"); // enable raw
    } else {
        system("stty -raw"); // disable raw
    }
}

inline void clearTerminalScreen() {
    system("clear");
}

inline void hideCursor() {
    std::cout << "\033[?25l"; // ANSI code to hide cursor
}

inline void showCursor() {
    std::cout << "\033[?25h"; // ANSI code to show cursor
}

#endif //PONG_UTILS_H
