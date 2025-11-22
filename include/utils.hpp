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


#endif //PONG_UTILS_H
