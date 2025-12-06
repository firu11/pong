#include "include/paddle.hpp"

#include <cmath>

paddle::paddle(float x, float y, int width, int height) {
    r = rect(x, y, width, height);
}

void paddle::moveUp() {
    r.y -= 1;
}

void paddle::moveDown() {
    r.y += 1;
}

void paddle::draw(char (&screen)[screenHeight][screenWidth]) {
    int rx = std::round(r.x);
    int ry = std::round(r.y);

    for (int i = 0; i < r.height; i++) {
        screen[ry + i][rx] = '|';
    }
}

paddle::rect paddle::getRect() {
    return rect{1, 1, 2, 2};
}
